/* 16-bit tokens for Win32 handles (roadmap W4; see w16.h). */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "w16.h"

#define SLOTS 65536
static HANDLE by_token[SLOTS];          /* token -> handle; token 0 is NULL */
static unsigned short hash_tok[SLOTS * 2];  /* open addressing: handle -> token */
static unsigned next_token = 0x100;     /* below 0x100: left free, small ints the game tests */

static unsigned hash(HANDLE h)
{
    uintptr_t v = (uintptr_t)h;
    return (unsigned)((v * 2654435761u) >> 15) & (SLOTS * 2 - 1);
}

unsigned w16_token(HANDLE h)
{
    unsigned i, t;
    if (!h)
        return 0;
    for (i = hash(h);; i = (i + 1) & (SLOTS * 2 - 1)) {
        t = hash_tok[i];
        if (!t)
            break;
        if (by_token[t] == h)
            return t;
    }
    for (t = next_token; by_token[t] || t == 0; t = (t + 1) & (SLOTS - 1))
        if (t == next_token - 1)
            abort();                    /* 65,280 live handles: never in this game */
    by_token[t] = h;
    hash_tok[i] = (unsigned short)t;
    next_token = (t + 1) & (SLOTS - 1);
    return t;
}

HANDLE w16_handle(unsigned token)
{
    return by_token[token & 0xFFFF];
}

void w16_forget(unsigned token)
{
    unsigned i;
    HANDLE h = by_token[token & 0xFFFF];
    if (!h)
        return;
    for (i = hash(h);; i = (i + 1) & (SLOTS * 2 - 1))
        if (hash_tok[i] == token) {
            /* leave a tombstone-free table: re-insert the run after this slot */
            unsigned j;
            hash_tok[i] = 0;
            by_token[token & 0xFFFF] = 0;
            for (j = (i + 1) & (SLOTS * 2 - 1); hash_tok[j]; j = (j + 1) & (SLOTS * 2 - 1)) {
                unsigned short t = hash_tok[j];
                unsigned k;
                hash_tok[j] = 0;
                for (k = hash(by_token[t]); hash_tok[k]; k = (k + 1) & (SLOTS * 2 - 1))
                    ;
                hash_tok[k] = t;
            }
            return;
        } else if (!hash_tok[i])
            return;
}

int w16_tracing = -1;

void w16_trace(const char *fmt, ...)
{
    static FILE *f;
    va_list ap;
    if (w16_tracing < 0) {
        const char *log = getenv("PORT_LOG");
        w16_tracing = getenv("PORT_TRACE") && log;
        if (w16_tracing)
            f = fopen(log, "a");
    }
    if (!w16_tracing || !f)
        return;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fputc('\n', f);
    fflush(f);
}

/* a line in $PORT_LOG whether or not the run traces: what a player's run that stops must say */
void w16_report(const char *fmt, ...)
{
    const char *log = getenv("PORT_LOG");
    FILE *f = log ? fopen(log, "a") : 0;
    va_list ap;
    if (!f)
        return;
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
    fputc('\n', f);
    fclose(f);
}
