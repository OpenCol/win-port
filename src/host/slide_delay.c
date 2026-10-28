/* slide_delay.c -- paces MapWin_SlideIcon's steps (issue #6).
 *
 * The 1995 slide has no timer: each step is drawn as fast as the machine can, which on a 1995 PC
 * was slow enough to watch and on a modern one is instant, with or without Fast Piece Slide. Each
 * step now waits until PORT_SLIDE_MS (default 12) after the previous one; Fast Piece Slide takes
 * half as many steps of twice the size, so it stays twice as fast. PORT_SLIDE_MS=0 restores the
 * unpaced slide.
 */
#include <windows.h>
#include <stdlib.h>

static int slide_ms = -1;
static LARGE_INTEGER freq;
static LARGE_INTEGER last;

#ifdef __cplusplus
extern "C"
#endif
void port_slide_step(void)
{
    LARGE_INTEGER now;
    LONGLONG due;

    if (slide_ms < 0) {
        const char *env = getenv("PORT_SLIDE_MS");
        slide_ms = env != NULL ? atoi(env) : 12;
        if (slide_ms < 0)
            slide_ms = 0;
        QueryPerformanceFrequency(&freq);
    }
    if (slide_ms == 0)
        return;
    QueryPerformanceCounter(&now);
    due = last.QuadPart + freq.QuadPart * slide_ms / 1000;
    /* a slide's first step, or one after a long gap, starts the clock instead of waiting */
    if (now.QuadPart < due && due - now.QuadPart <= freq.QuadPart * slide_ms / 1000) {
        while (now.QuadPart < due) {
            Sleep(1);
            QueryPerformanceCounter(&now);
        }
    }
    last = now;
}
