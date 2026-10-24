/* The port's log and crash report (roadmap W4, W10).
 *
 * The log is $PORT_LOG; without it (a player's run, started without the kit's launcher) it is
 * colonize-port.log beside the program, else in %TEMP%. Each start writes a header: the
 * Windows version, the program and its link stamp, the working directory and its game files,
 * the screen. A player's report is then readable without a second try.
 *
 * On an access violation or other fault, the faulting address, the registers, the
 * frame-pointer chain, the code addresses found on the stack, the window messages in progress
 * and -- in an instrumented build (PORT_INSTRUMENT=1) -- the last game functions entered go
 * to the log as `crash`, `frame`, `stack`, `message` and `entered` lines; tools/run_port.py
 * and tools/crash_log.py resolve them to functions. Each address names its module
 * ("ntdll.dll+0x2e3be"). A fault no handler takes is reported again if it differs, and a
 * minidump goes beside the log (colonize-port.dmp), for a debugger.
 *
 * A hang is not a fault: Windows marks the window "Not Responding" after five seconds without a
 * message taken, and a player who then closes it ends the program with 0xCFFFFFFF
 * (STATUS_APPLICATION_HANG) and no report. So a watch thread asks IsHungAppWindow of the
 * program's windows each second; on a hang, the stalled thread's registers and stack go to the
 * log as `hang` lines, as a crash's do, and a minidump beside it (colonize-port-hang.dmp). Not
 * under Wine (its IsHungAppWindow never says a window is hung) nor in a lockstep run, unless
 * $PORT_HANG_TEST (milliseconds) asks: then the game also stops that long in one PeekMessage.
 *
 * The handler allocates nothing: the log is opened at startup and written with WriteFile from
 * buffers on the stack, because the fault may be a corrupt heap (a bad free reaching ntdll).
 * The full report runs once (a fault while reporting is not reported); later faults a handler
 * may still take get one line each, up to 20. It reads memory only after VirtualQuery says it
 * is there, so it never faults itself.
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "w16.h"
#include <dbghelp.h>

static HANDLE log_file = INVALID_HANDLE_VALUE;
static char log_path[MAX_PATH + 32], dump_path[MAX_PATH + 32], hang_dump_path[MAX_PATH + 32];
static LPTOP_LEVEL_EXCEPTION_FILTER prev_filter;
static DWORD first_code;
static PVOID first_at;

/* the instrumented build's ring of entered functions (w16_instrument.c); absent otherwise */
extern void *w16_ring_entry(unsigned i) __attribute__((weak));
extern int w16_initial_loaded;                               /* w16_globals_init.c */
int w16_message_at(int i, unsigned *msg, void **hwnd);       /* w16_user.c */

static void put(const char *s)
{
    DWORD n = 0, w;
    while (s[n])
        n++;
    WriteFile(log_file, s, n, &w, 0);
}

static void say(const char *fmt, ...)
{
    char b[1024];
    va_list ap;
    va_start(ap, fmt);
    _vsnprintf(b, sizeof b - 1, fmt, ap);
    va_end(ap);
    b[sizeof b - 1] = 0;
    put(b);
}

static void hex(char *b, uintptr_t v)
{
    int i;
    for (i = 2 * sizeof v - 1; i >= 0; i--, v >>= 4)
        b[i] = "0123456789abcdef"[v & 15];
    b[2 * sizeof v] = 0;
}

/* " (module+0x1234)" for an address inside a loaded image, "" otherwise */
static void module_of(char *out, size_t n, uintptr_t a)
{
    MEMORY_BASIC_INFORMATION m;
    char p[MAX_PATH];
    const char *base;
    out[0] = 0;
    if (!VirtualQuery((void *)a, &m, sizeof m) || m.State != MEM_COMMIT || m.Type != MEM_IMAGE ||
        !GetModuleFileNameA((HMODULE)m.AllocationBase, p, sizeof p))
        return;
    base = strrchr(p, '\\');
    _snprintf(out, n - 1, " (%s+%#lx)", base ? base + 1 : p, (unsigned long)(a - (uintptr_t)m.AllocationBase));
    out[n - 1] = 0;
}

/* tag, 8 or 16 hex digits, the module: one line ("crash at 0040a1b2 (colonize.exe+0xa1b2)") */
static void line(const char *tag, uintptr_t v)
{
    char b[2 * sizeof v + 1], m[MAX_PATH + 32];
    hex(b, v);
    module_of(m, sizeof m, v);
    put(tag);
    put(b);
    put(m);
    put("\n");
}

static int readable(const void *p, size_t n)
{
    MEMORY_BASIC_INFORMATION m;
    if (!VirtualQuery(p, &m, sizeof m) || m.State != MEM_COMMIT ||
        (m.Protect & (PAGE_NOACCESS | PAGE_GUARD)) || !m.Protect)
        return 0;
    return (uintptr_t)p + n <= (uintptr_t)m.BaseAddress + m.RegionSize;
}

static int is_code(uintptr_t a)
{
    MEMORY_BASIC_INFORMATION m;
    return a > 0x10000 && VirtualQuery((void *)a, &m, sizeof m) && m.State == MEM_COMMIT && m.Type == MEM_IMAGE &&
           (m.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY));
}

static const char *code_name(DWORD c)
{
    switch (c) {
    case 0xC0000005: return "access violation";
    case 0xC000001D: return "illegal instruction";
    case 0xC0000094: return "integer divide by zero";
    case 0xC0000095: return "integer overflow";
    case 0xC00000FD: return "stack overflow";
    case 0xC0000374: return "heap corruption";
    case 0xC0000409: return "stack buffer overrun";
    case 0xC0000417: return "invalid C runtime parameter";
    case 0xC0000096: return "privileged instruction";
    case 0xC0000006: return "in-page error (the file went away?)";
    case 0xC000008E: case 0xC0000090: case 0xC0000091: case 0xC0000093: return "floating point";
    case 0xCFFFFFFF: return "not responding";
    }
    return "fault";
}

void w16_crash_install(void);

/* the whole report: `tag` names its first lines ("crash", "unhandled", "hang"); the stack is
   `thread`'s, lo..hi */
static void report(EXCEPTION_POINTERS *e, const char *tag, const char *frame_tag, DWORD thread, uintptr_t lo,
                   uintptr_t hi)
{
    DWORD code = e->ExceptionRecord->ExceptionCode;
    uintptr_t at = (uintptr_t)e->ExceptionRecord->ExceptionAddress;
    uintptr_t *bp, *sp;
    char t[64];
    unsigned msg;
    void *hwnd;
    int i, n;
    SYSTEMTIME now;
    GetLocalTime(&now);
    say("%s: %s, thread %lu, %02u:%02u:%02u\n", tag, code_name(code), thread, now.wHour,
        now.wMinute, now.wSecond);
    _snprintf(t, sizeof t, "%s code ", tag);
    line(t, code);
    _snprintf(t, sizeof t, "%s at ", tag);
    line(t, at);
    _snprintf(t, sizeof t, "%s anchor ", tag);
    line(t, (uintptr_t)w16_crash_install);   /* where the program was loaded: a Winelib build's
                                                 addresses less nm's for this */
    if (code == EXCEPTION_ACCESS_VIOLATION || code == 0xC0000006) {
        ULONG_PTR rw = e->ExceptionRecord->ExceptionInformation[0];
        _snprintf(t, sizeof t, "%s address ", tag);
        line(t, (uintptr_t)e->ExceptionRecord->ExceptionInformation[1]);
        say("%s access %s\n", tag, rw == 0 ? "read" : rw == 1 ? "write" : rw == 8 ? "execute (DEP)" : "?");
    }
    if (readable((void *)at, 16)) {
        const unsigned char *c = (const unsigned char *)at;
        say("%s bytes", tag);
        for (i = 0; i < 16; i++)
            say(" %02x", c[i]);
        put("\n");
    }
#ifdef __i386__
    say("%s registers eax %08lx ebx %08lx ecx %08lx edx %08lx esi %08lx edi %08lx\n", tag,
        e->ContextRecord->Eax, e->ContextRecord->Ebx, e->ContextRecord->Ecx, e->ContextRecord->Edx,
        e->ContextRecord->Esi, e->ContextRecord->Edi);
    say("%s registers ebp %08lx esp %08lx eip %08lx eflags %08lx\n", tag, e->ContextRecord->Ebp,
        e->ContextRecord->Esp, e->ContextRecord->Eip, e->ContextRecord->EFlags);
    _snprintf(t, sizeof t, "%s eax ", tag); line(t, e->ContextRecord->Eax);
    _snprintf(t, sizeof t, "%s ecx ", tag); line(t, e->ContextRecord->Ecx);
    _snprintf(t, sizeof t, "%s edx ", tag); line(t, e->ContextRecord->Edx);
    _snprintf(t, sizeof t, "%s esp ", tag); line(t, e->ContextRecord->Esp);
    sp = (uintptr_t *)e->ContextRecord->Esp;
#elif defined(_WIN64)
    say("%s registers rax %llx rbx %llx rcx %llx rdx %llx rsi %llx rdi %llx\n", tag,
        e->ContextRecord->Rax, e->ContextRecord->Rbx, e->ContextRecord->Rcx, e->ContextRecord->Rdx,
        e->ContextRecord->Rsi, e->ContextRecord->Rdi);
    say("%s registers r8 %llx r9 %llx r10 %llx r11 %llx rbp %llx rsp %llx rip %llx\n", tag,
        e->ContextRecord->R8, e->ContextRecord->R9, e->ContextRecord->R10, e->ContextRecord->R11,
        e->ContextRecord->Rbp, e->ContextRecord->Rsp, e->ContextRecord->Rip);
    sp = (uintptr_t *)e->ContextRecord->Rsp;
#else
    sp = 0;
#endif
#ifdef _WIN64
    {   /* Win64: unwind by the tables (a frame pointer is not a chain here), from the fault */
        CONTEXT ctx = *e->ContextRecord;
        for (i = 0; i < 24 && ctx.Rip; i++) {
            DWORD64 base = 0, frame = 0;
            void *data = 0;
            PRUNTIME_FUNCTION f = RtlLookupFunctionEntry(ctx.Rip, &base, 0);
            if (i)
                line(frame_tag, (uintptr_t)ctx.Rip);
            if (f)
                RtlVirtualUnwind(UNW_FLAG_NHANDLER, base, ctx.Rip, f, &ctx, &data, &frame, 0);
            else {                        /* a leaf: its return address is on top of the stack */
                if (ctx.Rsp < lo || ctx.Rsp + 8 > hi)
                    break;
                ctx.Rip = *(DWORD64 *)ctx.Rsp;
                ctx.Rsp += 8;
            }
        }
        bp = 0;
    }
#else
    bp = (uintptr_t *)e->ContextRecord->Ebp;
#endif
    for (i = 0; i < 24 && (uintptr_t)bp >= lo && (uintptr_t)bp + 2 * sizeof *bp <= hi; i++) {
        line(frame_tag, bp[1]);
        if ((uintptr_t *)bp[0] <= bp)
            break;
        bp = (uintptr_t *)bp[0];
    }
    /* every code address on the stack, nearest first: the calls a frame chain may miss (a
       system DLL without frame pointers between the game and the fault) */
    _snprintf(t, sizeof t, "%s stack ", tag);
    for (i = n = 0; sp && i < 4096 && n < 64 && (uintptr_t)(sp + i + 1) <= hi && (uintptr_t)(sp + i) >= lo; i++)
        if (is_code(sp[i])) {
            line(t, sp[i]);
            n++;
        }
    for (i = 0; w16_message_at(i, &msg, &hwnd); i++)
        say("%s message [%d] %#x to %p\n", tag, i, msg, hwnd);
    say("%s initial data %d globals\n", tag, w16_initial_loaded);
    if (w16_ring_entry)
        for (i = 0; i < 64; i++)
            if (w16_ring_entry((unsigned)i))
                line("entered ", (uintptr_t)w16_ring_entry((unsigned)i));
}

/* every fault, first chance: the first gets the whole report, the next 20 a line each */
static LONG CALLBACK on_fault(EXCEPTION_POINTERS *e)
{
    static volatile LONG busy, later;
    DWORD code = e->ExceptionRecord->ExceptionCode;
    if ((code & 0xF0000000u) != 0xC0000000u)   /* errors only: not a debug print, not a breakpoint */
        return EXCEPTION_CONTINUE_SEARCH;
    if (log_file == INVALID_HANDLE_VALUE)
        return EXCEPTION_CONTINUE_SEARCH;
    if (InterlockedExchange(&busy, 1)) {
        if (InterlockedIncrement(&later) <= 20) {
            char m[MAX_PATH + 32];
            module_of(m, sizeof m, (uintptr_t)e->ExceptionRecord->ExceptionAddress);
            say("exception %08lx at %p%s, thread %lu (first chance: a handler may take it)\n", code,
                e->ExceptionRecord->ExceptionAddress, m, GetCurrentThreadId());
        }
        return EXCEPTION_CONTINUE_SEARCH;
    }
    first_code = code;
    first_at = e->ExceptionRecord->ExceptionAddress;
    report(e, "crash", "frame ", GetCurrentThreadId(), (uintptr_t)((NT_TIB *)NtCurrentTeb())->StackLimit,
           (uintptr_t)((NT_TIB *)NtCurrentTeb())->StackBase);
    return EXCEPTION_CONTINUE_SEARCH;
}

/* a minidump at `path`, for a debugger: the fault's, or (e null) the whole process's */
static void minidump(const char *path, const char *tag, EXCEPTION_POINTERS *e)
{
    typedef BOOL (WINAPI *dump_fn)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
                                   PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
    HMODULE dbg;
    dump_fn dump;
    if (path[0] && (dbg = LoadLibraryA("dbghelp.dll")) &&
        (dump = (dump_fn)(void (*)(void))GetProcAddress(dbg, "MiniDumpWriteDump"))) {
        HANDLE f = CreateFileA(path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        MINIDUMP_EXCEPTION_INFORMATION x;
        x.ThreadId = GetCurrentThreadId();
        x.ExceptionPointers = e;
        x.ClientPointers = FALSE;
        if (f != INVALID_HANDLE_VALUE) {
            BOOL ok = dump(GetCurrentProcess(), GetCurrentProcessId(), f,
                           (MINIDUMP_TYPE)(MiniDumpWithDataSegs | MiniDumpWithIndirectlyReferencedMemory |
                                           MiniDumpWithUnloadedModules), e ? &x : 0, 0, 0);
            CloseHandle(f);
            say("%s: minidump %s %s\n", tag, path, ok ? "written" : "failed");
        } else
            say("%s: minidump %s could not be created (%lu)\n", tag, path, GetLastError());
    }
}

/* a fault nothing handled: the process ends here. Said (again, if it is not the first one),
   and a minidump written beside the log */
static LONG WINAPI on_unhandled(EXCEPTION_POINTERS *e)
{
    static volatile LONG once;
    if (log_file != INVALID_HANDLE_VALUE && !InterlockedExchange(&once, 1)) {
        if (e->ExceptionRecord->ExceptionCode == first_code && e->ExceptionRecord->ExceptionAddress == first_at)
            say("unhandled: the crash above ends the program\n");
        else
            report(e, "unhandled", "unhandled frame ", GetCurrentThreadId(),
                   (uintptr_t)((NT_TIB *)NtCurrentTeb())->StackLimit, (uintptr_t)((NT_TIB *)NtCurrentTeb())->StackBase);
        minidump(dump_path, "unhandled", e);
    }
    return prev_filter ? prev_filter(e) : EXCEPTION_CONTINUE_SEARCH;
}

/* the program ends without a fault: said, so a run that stops is not silent */
static void on_exit_(void)
{
    if (log_file != INVALID_HANDLE_VALUE)
        say("exit: the program returned or called exit() (initial data %d globals)\n", w16_initial_loaded);
}

/* $PORT_LOG, or the player's default; set in the environment so every writer (w16_report,
   the trap lines, the RTL) finds it */
const char *w16_log_path(void)
{
    static int done;
    const char *env;
    char *s;
    if (done)
        return log_path[0] ? log_path : 0;
    done = 1;
    if ((env = getenv("PORT_LOG")) != 0)
        _snprintf(log_path, sizeof log_path - 1, "%s", env);
#ifndef __WINE__   /* Winelib: its module path is Wine's; the kit's script sets $PORT_LOG */
    else {
        char dir[MAX_PATH];
        static char assign[MAX_PATH + 48];
        int k;
        for (k = 0; k < 2 && !log_path[0]; k++) {
            DWORD n = k == 0 ? GetModuleFileNameA(0, dir, sizeof dir) : GetTempPathA(sizeof dir, dir);
            HANDLE f;
            if (!n || n >= sizeof dir)
                continue;
            if (k == 0 && (s = strrchr(dir, '\\')) != 0)
                s[1] = 0;
            else if (k == 0)
                continue;
            _snprintf(log_path, sizeof log_path - 1, "%s%scolonize-port.log", dir,
                      dir[strlen(dir) - 1] == '\\' ? "" : "\\");
            f = CreateFileA(log_path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL, 0);
            if (f == INVALID_HANDLE_VALUE)
                log_path[0] = 0;
            else
                CloseHandle(f);
        }
        if (log_path[0]) {
            _snprintf(assign, sizeof assign - 1, "PORT_LOG=%s", log_path);
            _putenv(assign);
        }
    }
#endif
    if (log_path[0]) {
        _snprintf(dump_path, sizeof dump_path - 5, "%s", log_path);
        s = strrchr(dump_path, '.');
        if (s && !strchr(s, '\\') && !strchr(s, '/'))
            *s = 0;
        _snprintf(hang_dump_path, sizeof hang_dump_path - 1, "%s-hang.dmp", dump_path);
        strcat(dump_path, ".dmp");
    }
    return log_path[0] ? log_path : 0;
}

/* the start: what a report from another machine needs to be read */
static void header(void)
{
    typedef LONG (WINAPI *rtl_version)(OSVERSIONINFOEXW *);
    typedef const char *(CDECL *wine_version)(void);
    typedef BOOL (WINAPI *wow64)(HANDLE, BOOL *);
    HMODULE ntdll = GetModuleHandleA("ntdll.dll"), k32 = GetModuleHandleA("kernel32.dll");
    rtl_version rv = ntdll ? (rtl_version)(void (*)(void))GetProcAddress(ntdll, "RtlGetVersion") : 0;
    wine_version wv = ntdll ? (wine_version)(void (*)(void))GetProcAddress(ntdll, "wine_get_version") : 0;
    wow64 w64 = k32 ? (wow64)(void (*)(void))GetProcAddress(k32, "IsWow64Process") : 0;
    IMAGE_DOS_HEADER *dos = (IMAGE_DOS_HEADER *)GetModuleHandleA(0);
    IMAGE_NT_HEADERS *nt = (IMAGE_NT_HEADERS *)((char *)dos + dos->e_lfanew);
    OSVERSIONINFOEXW v;
    MEMORYSTATUSEX mem;
    SYSTEMTIME now;
    WIN32_FIND_DATAA fd;
    HANDLE h;
    HDC dc;
    BOOL is_wow = FALSE;
    char p[MAX_PATH];
    int dlls = 0, waves = 0;
    GetLocalTime(&now);
    say("==== colonize.exe started %04u-%02u-%02u %02u:%02u:%02u, the %s build linked %08lx, at %p (anchor %p)\n",
        now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
#ifdef _WIN64
        "x64",
#else
        "x86",
#endif
        (unsigned long)nt->FileHeader.TimeDateStamp, (void *)dos, (void *)w16_crash_install);
    memset(&v, 0, sizeof v);
    v.dwOSVersionInfoSize = sizeof v;
    if (w64)
        w64(GetCurrentProcess(), &is_wow);
    if (rv && !rv(&v))
        say("windows %lu.%lu build %lu, service pack %u.%u, %s%s%s%s\n", v.dwMajorVersion, v.dwMinorVersion,
            v.dwBuildNumber, v.wServicePackMajor, v.wServicePackMinor,
            v.wProductType == VER_NT_WORKSTATION ? "workstation" : "server",
            is_wow ? ", 32-bit program on 64-bit Windows" : "", wv ? ", wine " : "", wv ? wv() : "");
    if (GetModuleFileNameA(0, p, sizeof p))
        say("program %s\n", p);
    if (GetCurrentDirectoryA(sizeof p, p))
        say("folder %s\n", p);
    say("command line %s\n", GetCommandLineA());
    h = FindFirstFileA("COLONIZE.EXE", &fd);
    if (h != INVALID_HANDLE_VALUE) {
        say("game COLONIZE.EXE %lu bytes\n", fd.nFileSizeLow);
        FindClose(h);
    } else
        say("game COLONIZE.EXE is NOT in the folder\n");
    if ((h = FindFirstFileA("*.DLL", &fd)) != INVALID_HANDLE_VALUE) {
        do dlls++; while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    if ((h = FindFirstFileA("*.WAV", &fd)) != INVALID_HANDLE_VALUE) {
        do waves++; while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    say("game files: %d .DLL, %d .WAV\n", dlls, waves);
    {   /* the music is CD audio, and the game asks the drive while it waits: which drives there are
           (only their letters: asking for the disc could spin one up) */
        char drives[128] = "", *d, list[64] = "";
        if (GetLogicalDriveStringsA(sizeof drives - 1, drives) < sizeof drives)
            for (d = drives; *d; d += strlen(d) + 1)
                if (GetDriveTypeA(d) == DRIVE_CDROM && strlen(list) + 4 < sizeof list) {
                    strncat(list, d, 2);
                    strcat(list, " ");
                }
        say("cd drives: %s\n", list[0] ? list : "none");
    }
    if ((dc = GetDC(0)) != 0) {
        say("screen %dx%d, %d bits per pixel, %d planes, %s, %d dpi\n", GetDeviceCaps(dc, HORZRES),
            GetDeviceCaps(dc, VERTRES), GetDeviceCaps(dc, BITSPIXEL), GetDeviceCaps(dc, PLANES),
            GetDeviceCaps(dc, RASTERCAPS) & RC_PALETTE ? "a palette device" : "no palette",
            GetDeviceCaps(dc, LOGPIXELSX));
        ReleaseDC(0, dc);
    }
    mem.dwLength = sizeof mem;
    if (GlobalMemoryStatusEx(&mem))
        say("memory %lu MB free of %lu MB\n", (unsigned long)(mem.ullAvailPhys >> 20),
            (unsigned long)(mem.ullTotalPhys >> 20));
}

/* ------------------------------------------------------------------ hangs */

static int hang_test_ms;

/* $PORT_HANG_TEST's stall: the game stops in one PeekMessage, once, two seconds in (w16_user.c) */
void w16_hang_test(void)
{
    static DWORD start;
    if (!hang_test_ms)
        return;
    if (!start)
        start = GetTickCount();
    else if (GetTickCount() - start > 2000) {
        int ms = hang_test_ms;
        hang_test_ms = 0;
        say("hang test: the game stops %d ms\n", ms);
        Sleep(ms);
    }
}

typedef struct { HWND hwnd; DWORD thread; } HUNG;

/* Windows' own test, the one behind "Not Responding". Wine's never says so: under it, the test
   run asks the window instead (a WM_NULL that is not answered in 2 seconds) */
static int hung(HWND h)
{
    DWORD_PTR r;
    if (!getenv("PORT_HANG_TEST"))
        return IsHungAppWindow(h);
    return !SendMessageTimeoutA(h, WM_NULL, 0, 0, SMTO_ABORTIFHUNG | SMTO_BLOCK, 2000, &r) &&
           GetLastError() == ERROR_TIMEOUT;
}

static BOOL CALLBACK find_hung(HWND h, LPARAM lp)
{
    HUNG *x = (HUNG *)lp;
    DWORD pid = 0, tid = GetWindowThreadProcessId(h, &pid);
    if (pid != GetCurrentProcessId() || !IsWindowVisible(h) || !hung(h))
        return TRUE;
    x->hwnd = h;
    x->thread = tid;
    return FALSE;
}

/* the stalled thread's registers and stack, read while it is suspended: a crash's report */
static void report_hang(DWORD tid)
{
    HANDLE t = OpenThread(THREAD_SUSPEND_RESUME | THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION, FALSE, tid);
    CONTEXT c;
    EXCEPTION_RECORD r;
    EXCEPTION_POINTERS e;
    MEMORY_BASIC_INFORMATION m;
    uintptr_t sp;
    if (!t) {
        say("hang: thread %lu could not be opened (%lu)\n", tid, GetLastError());
        return;
    }
    if (SuspendThread(t) == (DWORD)-1) {
        say("hang: thread %lu could not be suspended (%lu)\n", tid, GetLastError());
        CloseHandle(t);
        return;
    }
    memset(&c, 0, sizeof c);
    c.ContextFlags = CONTEXT_FULL;
    if (GetThreadContext(t, &c)) {
        memset(&r, 0, sizeof r);
        r.ExceptionCode = 0xCFFFFFFF;
#ifdef _WIN64
        sp = (uintptr_t)c.Rsp;
        r.ExceptionAddress = (PVOID)(uintptr_t)c.Rip;
#else
        sp = (uintptr_t)c.Esp;
        r.ExceptionAddress = (PVOID)(uintptr_t)c.Eip;
#endif
        e.ExceptionRecord = &r;
        e.ContextRecord = &c;
        /* the committed stack from the stack pointer up is one region: its top is the stack's base */
        if (VirtualQuery((void *)sp, &m, sizeof m))
            report(&e, "hang", "hang frame ", tid, sp, (uintptr_t)m.BaseAddress + m.RegionSize);
    } else
        say("hang: thread %lu's registers could not be read (%lu)\n", tid, GetLastError());
    ResumeThread(t);
    CloseHandle(t);
}

/* each second: is a window of the program "Not Responding"? Said once a hang, with the stalled
   thread's stack (the first three) and a minidump (the first) */
static DWORD WINAPI watch_hangs(void *unused)
{
    DWORD since = 0;
    int hangs = 0;
    (void)unused;
    for (;;) {
        HUNG x = {0, 0};
        Sleep(1000);
        EnumWindows(find_hung, (LPARAM)&x);
        if (x.hwnd && !since) {
            since = GetTickCount();
            if (++hangs <= 3) {
                say("hang: window %p (thread %lu) is not responding: no message taken for 5 seconds\n",
                    (void *)x.hwnd, x.thread);
                report_hang(x.thread);
                if (hangs == 1)
                    minidump(hang_dump_path, "hang", 0);
            }
        } else if (!x.hwnd && since) {
            if (hangs <= 3)
                say("hang: responding again after %lu s more\n", (GetTickCount() - since) / 1000);
            since = 0;
        }
    }
    return 0;
}

static void start_hang_watch(void)
{
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    const char *test = getenv("PORT_HANG_TEST");
    HANDLE t;
    if (test)
        hang_test_ms = atoi(test);
    else if ((ntdll && GetProcAddress(ntdll, "wine_get_version")) || getenv("PORT_LOCKSTEP"))
        return;
    if ((t = CreateThread(0, 0, watch_hangs, 0, 0, 0)) != 0)
        CloseHandle(t);
}

/* At startup, before anything else of the port's runs: the initial data's constructor calls it
   first (w16_globals_init.c) -- v0.1.1's x64 program died in the initial data on Windows 7
   (0xC0000374) before this ran, and its log said nothing; MinGW does not order constructor
   priorities between objects, so this is a call, not a priority. It is also a constructor of its
   own, and runs once. On Winelib an ELF constructor runs before Wine's C runtime has the
   environment, so the Linux build's start calls it (w16_winelib.c) */
#ifndef __WINE__
__attribute__((constructor))
#endif
void w16_crash_install(void)
{
    static int done;
    if (done)
        return;
    done = 1;
    const char *log = w16_log_path();
    if (log)
        log_file = CreateFileA(log, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL, 0);
    if (log_file != INVALID_HANDLE_VALUE) {
        header();
        if (w16_initial_loaded >= 0)   /* the order above went wrong: say so */
            say("note: the initial data was loaded before this report was set up\n");
    }
    AddVectoredExceptionHandler(1, on_fault);
    if (log_file != INVALID_HANDLE_VALUE)
        start_hang_watch();
    prev_filter = SetUnhandledExceptionFilter(on_unhandled);
    atexit(on_exit_);
    if (getenv("PORT_CRASH_TEST"))   /* the report's own test: a write to that address */
        *(volatile int *)(uintptr_t)atoi(getenv("PORT_CRASH_TEST")) = 1;
}
