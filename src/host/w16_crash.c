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
static char log_path[MAX_PATH + 32], dump_path[MAX_PATH + 32];
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
    }
    return "fault";
}

void w16_crash_install(void);

/* the whole report: `tag` names its first lines ("crash", "unhandled") */
static void report(EXCEPTION_POINTERS *e, const char *tag, const char *frame_tag)
{
    DWORD code = e->ExceptionRecord->ExceptionCode;
    uintptr_t at = (uintptr_t)e->ExceptionRecord->ExceptionAddress;
    NT_TIB *tib = (NT_TIB *)NtCurrentTeb();
    uintptr_t *bp, *sp, lo, hi;
    char t[64];
    unsigned msg;
    void *hwnd;
    int i, n;
    SYSTEMTIME now;
    GetLocalTime(&now);
    say("%s: %s, thread %lu, %02u:%02u:%02u\n", tag, code_name(code), GetCurrentThreadId(), now.wHour,
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
    lo = (uintptr_t)tib->StackLimit;
    hi = (uintptr_t)tib->StackBase;
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
    report(e, "crash", "frame ");
    return EXCEPTION_CONTINUE_SEARCH;
}

/* a fault nothing handled: the process ends here. Said (again, if it is not the first one),
   and a minidump written beside the log */
static LONG WINAPI on_unhandled(EXCEPTION_POINTERS *e)
{
    static volatile LONG once;
    typedef BOOL (WINAPI *dump_fn)(HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, PMINIDUMP_EXCEPTION_INFORMATION,
                                   PMINIDUMP_USER_STREAM_INFORMATION, PMINIDUMP_CALLBACK_INFORMATION);
    if (log_file != INVALID_HANDLE_VALUE && !InterlockedExchange(&once, 1)) {
        HMODULE dbg;
        dump_fn dump;
        if (e->ExceptionRecord->ExceptionCode == first_code && e->ExceptionRecord->ExceptionAddress == first_at)
            say("unhandled: the crash above ends the program\n");
        else
            report(e, "unhandled", "unhandled frame ");
        if (dump_path[0] && (dbg = LoadLibraryA("dbghelp.dll")) &&
            (dump = (dump_fn)(void (*)(void))GetProcAddress(dbg, "MiniDumpWriteDump"))) {
            HANDLE f = CreateFileA(dump_path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
            MINIDUMP_EXCEPTION_INFORMATION x;
            x.ThreadId = GetCurrentThreadId();
            x.ExceptionPointers = e;
            x.ClientPointers = FALSE;
            if (f != INVALID_HANDLE_VALUE) {
                BOOL ok = dump(GetCurrentProcess(), GetCurrentProcessId(), f,
                               (MINIDUMP_TYPE)(MiniDumpWithDataSegs | MiniDumpWithIndirectlyReferencedMemory |
                                               MiniDumpWithUnloadedModules), &x, 0, 0);
                CloseHandle(f);
                say("unhandled: minidump %s %s\n", dump_path, ok ? "written" : "failed");
            } else
                say("unhandled: minidump %s could not be created (%lu)\n", dump_path, GetLastError());
        }
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

/* At startup (a constructor) on Windows; on Winelib an ELF constructor runs before Wine's C
   runtime has the environment, so the Linux build's start calls it (w16_winelib.c) */
#ifndef __WINE__
__attribute__((constructor))
#endif
void w16_crash_install(void)
{
    const char *log = w16_log_path();
    if (log)
        log_file = CreateFileA(log, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL, 0);
    if (log_file != INVALID_HANDLE_VALUE)
        header();
    AddVectoredExceptionHandler(1, on_fault);
    prev_filter = SetUnhandledExceptionFilter(on_unhandled);
    atexit(on_exit_);
    if (getenv("PORT_CRASH_TEST"))   /* the report's own test: a write to that address */
        *(volatile int *)(uintptr_t)atoi(getenv("PORT_CRASH_TEST")) = 1;
}
