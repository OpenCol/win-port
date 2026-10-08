/* Function-entry hooks for an instrumented build (PORT_INSTRUMENT=1 tools/undefined.py, which
   compiles the game with -finstrument-functions): the call depth, and the last 64 functions
   entered. Past MAX_DEPTH -- a recursion that would overflow the stack -- the ring goes to
   $PORT_LOG as `frame` lines (tools/run_port.py resolves them) and the program stops. */
#include <stdio.h>
#include <stdlib.h>
#include "w16.h"

#define MAX_DEPTH 3000
static void *ring[64];
static unsigned head, depth;

__attribute__((no_instrument_function)) void __cyg_profile_func_enter(void *fn, void *site)
{
    (void)site;
    ring[head++ & 63] = fn;
    if (++depth == MAX_DEPTH) {
        const char *log = getenv("PORT_LOG");
        FILE *f = log ? fopen(log, "a") : 0;
        unsigned i;
        if (f) {
            fprintf(f, "crash recursion deeper than %d calls; the last functions entered:\n", MAX_DEPTH);
            for (i = 0; i < 64; i++)
                fprintf(f, "frame %p\n", ring[(head - 1 - i) & 63]);
            fclose(f);
        }
        ExitProcess(4);
    }
}

/* for the crash report (w16_crash.c): the i-th most recent function entered */
__attribute__((no_instrument_function)) void *w16_ring_entry(unsigned i)
{
    return ring[(head - 1 - i) & 63];
}

__attribute__((no_instrument_function)) void __cyg_profile_func_exit(void *fn, void *site)
{
    (void)fn;
    (void)site;
    depth--;
}
