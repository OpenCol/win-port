/* A crash report for the port under development (roadmap W4): on an access violation or other
 * fault, the faulting address, the frame-pointer chain and -- in an instrumented build
 * (PORT_INSTRUMENT=1) -- the last game functions entered go to $PORT_LOG as `crash`, `frame`
 * and `entered` lines; tools/run_port.py resolves them to functions and lines.
 *
 * The handler allocates nothing: the log is opened at startup and written with WriteFile from a
 * buffer on the stack, because the fault may be a corrupt heap (a bad free reaching ntdll).
 * It runs once (a fault while reporting is not reported), and walks frames only inside the
 * thread's own stack, so it never faults itself.
 */
#include <stdlib.h>
#include "w16.h"

static HANDLE log_file = INVALID_HANDLE_VALUE;

/* the instrumented build's ring of entered functions (w16_instrument.c); absent otherwise */
extern void *w16_ring_entry(unsigned i) __attribute__((weak));

static void put(const char *s)
{
    DWORD n = 0, w;
    while (s[n])
        n++;
    WriteFile(log_file, s, n, &w, 0);
}

static void line(const char *tag, uintptr_t v)
{
    char b[9];
    int i;
    for (i = 7; i >= 0; i--, v >>= 4)
        b[i] = "0123456789abcdef"[v & 15];
    b[8] = 0;
    put(tag);
    put(b);
    put("\n");
}

static LONG CALLBACK on_fault(EXCEPTION_POINTERS *e)
{
    static volatile LONG busy;
    DWORD code = e->ExceptionRecord->ExceptionCode;
    NT_TIB *tib = (NT_TIB *)NtCurrentTeb();
    uintptr_t *bp, lo, hi;
    int i;
    if ((code & 0xF0000000u) != 0xC0000000u)   /* errors only: not a debug print, not a breakpoint */
        return EXCEPTION_CONTINUE_SEARCH;
    if (log_file == INVALID_HANDLE_VALUE || InterlockedExchange(&busy, 1))
        return EXCEPTION_CONTINUE_SEARCH;
    line("crash code ", code);
    line("crash at ", (uintptr_t)e->ExceptionRecord->ExceptionAddress);
    if (code == EXCEPTION_ACCESS_VIOLATION)
        line("crash address ", (uintptr_t)e->ExceptionRecord->ExceptionInformation[1]);
    lo = (uintptr_t)tib->StackLimit;
    hi = (uintptr_t)tib->StackBase;
    bp = (uintptr_t *)e->ContextRecord->Ebp;
    for (i = 0; i < 24 && (uintptr_t)bp >= lo && (uintptr_t)bp + 8 <= hi; i++) {
        line("frame ", bp[1]);
        if ((uintptr_t *)bp[0] <= bp)
            break;
        bp = (uintptr_t *)bp[0];
    }
    if (w16_ring_entry)
        for (i = 0; i < 64; i++)
            line("entered ", (uintptr_t)w16_ring_entry((unsigned)i));
    return EXCEPTION_CONTINUE_SEARCH;
}

/* the program ends without a fault: said, so a run that stops is not silent */
static void on_exit_(void)
{
    if (log_file != INVALID_HANDLE_VALUE)
        put("exit: the program returned or called exit()\n");
}

__attribute__((constructor)) static void install(void)
{
    const char *log = getenv("PORT_LOG");
    if (log)
        log_file = CreateFileA(log, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL, 0);
    AddVectoredExceptionHandler(1, on_fault);
    atexit(on_exit_);
}
