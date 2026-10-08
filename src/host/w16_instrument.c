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

void w16_trace_call(void *fn, uintptr_t *frame);

__attribute__((no_instrument_function)) void __cyg_profile_func_enter(void *fn, void *site)
{
    (void)site;
    w16_trace_call(fn, (uintptr_t *)__builtin_frame_address(1));
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

/* The framework trace (tests/trace.py, $PORT_MSTRACE): for each call to a function in
   port_trace_fns -- the game's MS* framework -- its name and arguments, 4 bytes each, read from
   its frame; after $PORT_MSTRACE_N calls the program stops. */
struct PortTraceFn { void *fn; const char *name; const char *kinds; };
extern const struct PortTraceFn port_trace_fns[] __attribute__((weak));
extern const unsigned port_trace_fn_count __attribute__((weak));
static const struct PortTraceFn **sorted;
static unsigned traced, trace_n;
static FILE *trace_file;
static int trace_state = -1;

__attribute__((no_instrument_function)) static int by_fn(const void *a, const void *b)
{
    uintptr_t x = (uintptr_t)(*(const struct PortTraceFn **)a)->fn, y = (uintptr_t)(*(const struct PortTraceFn **)b)->fn;
    return x < y ? -1 : x > y;
}

__attribute__((no_instrument_function)) static void trace_init(void)
{
    const char *path = getenv("PORT_MSTRACE"), *n = getenv("PORT_MSTRACE_N");
    unsigned i;
    trace_state = 0;
    if (!path || !&port_trace_fn_count || !port_trace_fn_count)
        return;
    trace_file = fopen(path, "w");
    if (!trace_file)
        return;
    trace_n = n ? (unsigned)atoi(n) : 10000;
    sorted = malloc(port_trace_fn_count * sizeof *sorted);
    for (i = 0; i < port_trace_fn_count; i++)
        sorted[i] = &port_trace_fns[i];
    qsort(sorted, port_trace_fn_count, sizeof *sorted, by_fn);
    trace_state = 1;
}

__attribute__((no_instrument_function)) void w16_trace_call(void *fn, uintptr_t *frame)
{
    unsigned lo = 0, hi, k;
    const struct PortTraceFn *t;
    if (trace_state < 0)
        trace_init();
    if (trace_state != 1)
        return;
    hi = port_trace_fn_count;
    while (lo < hi) {
        unsigned mid = (lo + hi) / 2;
        if ((uintptr_t)sorted[mid]->fn < (uintptr_t)fn)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo >= port_trace_fn_count || sorted[lo]->fn != fn)
        return;
    t = sorted[lo];
    fputs(t->name, trace_file);
    for (k = 0; t->kinds[k]; k++)
        fprintf(trace_file, " %lx", (unsigned long)frame[2 + k]);
    fputc('\n', trace_file);
    {
        /* $PORT_MSTRACE_STOP=k: at the k-th traced call, the functions entered before it go to
           $PORT_LOG (tools/run_port.py resolves them) and the program stops -- where a divergence
           from the oracle comes from */
        static int stop = -2;
        if (stop == -2) {
            const char *s = getenv("PORT_MSTRACE_STOP");
            stop = s ? atoi(s) : -1;
        }
        if ((int)traced == stop) {
            const char *log = getenv("PORT_LOG");
            FILE *f = log ? fopen(log, "a") : 0;
            unsigned i;
            if (f) {
                for (i = 0; i < 64; i++)
                    fprintf(f, "entered %08lx\n", (unsigned long)(uintptr_t)ring[(head - 1 - i) & 63]);
                fclose(f);
            }
            fclose(trace_file);
            ExitProcess(0);
        }
    }
    if (++traced >= trace_n) {
        fclose(trace_file);
        ExitProcess(0);
    }
}
