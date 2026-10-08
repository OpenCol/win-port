/* A crash report for the port under development (roadmap W4): on an access violation or other
   fault, the faulting address and the frame-pointer chain go to $PORT_LOG as `crash` and
   `frame` lines; tools/run_port.py resolves them to functions and lines. The build keeps
   frame pointers (tools/undefined.py) so the chain can be walked. */
#include <stdio.h>
#include <stdlib.h>
#include "w16.h"

static LONG CALLBACK on_fault(EXCEPTION_POINTERS *e)
{
    const char *log = getenv("PORT_LOG");
    FILE *f;
    DWORD code = e->ExceptionRecord->ExceptionCode;
    uintptr_t *bp;
    int i;
    if (code != EXCEPTION_ACCESS_VIOLATION && code != EXCEPTION_INT_DIVIDE_BY_ZERO &&
        code != EXCEPTION_ILLEGAL_INSTRUCTION && code != EXCEPTION_STACK_OVERFLOW)
        return EXCEPTION_CONTINUE_SEARCH;
    if (!log || !(f = fopen(log, "a")))
        return EXCEPTION_CONTINUE_SEARCH;
    fprintf(f, "crash %08lx at %p", (unsigned long)code, e->ExceptionRecord->ExceptionAddress);
    if (code == EXCEPTION_ACCESS_VIOLATION)
        fprintf(f, " (%s %p)", e->ExceptionRecord->ExceptionInformation[0] ? "write" : "read",
                (void *)e->ExceptionRecord->ExceptionInformation[1]);
    fputc('\n', f);
    bp = (uintptr_t *)e->ContextRecord->Ebp;
    for (i = 0; i < 24 && bp && !IsBadReadPtr(bp, 8); i++) {
        fprintf(f, "frame %p\n", (void *)bp[1]);
        if ((uintptr_t *)bp[0] <= bp)
            break;
        bp = (uintptr_t *)bp[0];
    }
    fclose(f);
    return EXCEPTION_CONTINUE_SEARCH;
}

__attribute__((constructor)) static void install(void)
{
    AddVectoredExceptionHandler(1, on_fault);
}
