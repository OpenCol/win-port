/* The Win16 platform layer on Win32 (roadmap W4): what its files share.
 *
 * The game calls the Win16 API by its Win16 names, with Win16 arguments: handles that fit a
 * 16-bit field (`u16 hwnd` in its records), RECT and POINT of 16-bit coordinates, messages
 * packed the Win16 way. src/host/w16_*.c provides each of those functions on top of Win32.
 * The C names here are w16_<Name>; W16(Name) gives each the symbol the game links against
 * (`_SetRect`), which <windows.h>'s own `SetRect@20` does not collide with.
 */
#ifndef PORT_W16_H
#define PORT_W16_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>

#define W16_STR2(x) #x
#define W16_STR(x) W16_STR2(x)
#if defined(__WINE__) || defined(_WIN64)
/* Winelib (W8) and Win64 (W9): an ELF or Win64 stdcall symbol is undecorated, so `GlobalAlloc` would be both this
   function and the Win32 one it calls -- it would call itself. The function keeps its C name
   here (w16_GlobalAlloc), and the game's objects have their references renamed to it
   (tools/undefined.py: objcopy --redefine-syms, from these W16() lines). */
#define W16(name)
#else
#define W16(name) __asm__(W16_STR(__USER_LABEL_PREFIX__) #name)
#endif

/* the game's geometry: 16-bit, as include/gen/w16types.h has them */
typedef struct { int16_t left, top, right, bottom; } RECT16;
typedef struct { int16_t x, y; } POINT16;

/* Handles. The game keeps window, DC and GDI handles in 16-bit fields, and Win32's do not
   fit; every handle crossing into the game is a 16-bit token, 0 for NULL. */
unsigned w16_token(HANDLE h);          /* the token for h, made on first sight */
HANDLE w16_handle(unsigned token);     /* the Win32 handle behind a token */
void w16_report(const char *fmt, ...);  /* a line in $PORT_LOG, traced or not */
const char *w16_log_path(void);        /* $PORT_LOG, or the player's default (w16_crash.c) */
void w16_crash_install(void);          /* the log and crash report: first thing at startup */
void w16_forget(unsigned token);       /* the handle is gone (DeleteObject, DestroyWindow) */
#define W16_HWND(t) ((HWND)w16_handle(t))
#define W16_HDC(t) ((HDC)w16_handle(t))

/* $PORT_TRACE: one line per platform call, to $PORT_LOG */
void w16_trace(const char *fmt, ...);
/* the autoplay lockstep (w16_lockstep.c, roadmap W6): all 0 / no-ops unless $PORT_LOCKSTEP is set */
int w16_lockstep(void);
int w16_lockstep_silent(void);          /* MCI and sndPlaySound do nothing */
int w16_lockstep_screen(int y);         /* its screen: width (0) or height (1) */
int w16_script(void);                   /* a lockstep script runs, in lockstep or realtime */
const char *w16_lockstep_save(void);
const char *w16_lockstep_saveas(void);
int w16_snapshot_write(const char *path);
unsigned long w16_lockstep_ticks(void);
unsigned w16_lockstep_set_timer(HWND h, unsigned id, unsigned ms);
int w16_lockstep_kill_timer(HWND h, unsigned id);
void w16_lockstep_idle(void);
void w16_lockstep_message(const MSG *m);
int w16_lockstep_input(const MSG *m);
unsigned w16_lockstep_srand(unsigned seed);
extern int w16_tracing;

/* the 1995 display, in software (w16_display.c) */
int w16_palette_device(void);
int w16_device_caps(HDC dc, int i);
unsigned w16_system_palette(unsigned start, unsigned n, PALETTEENTRY *out);
void w16_realize(HPALETTE pal);
void w16_animate(unsigned start, unsigned n, const PALETTEENTRY *e);
void w16_palette_quiet(unsigned start, unsigned n, const void *entries);  /* no re-blit */
int w16_display_blit(HDC dst, int x, int y, int w, int h, HDC src, int sx, int sy, int sw, int sh);

static inline void w16_rect_in(RECT *d, const RECT16 *s)
{
    d->left = s->left; d->top = s->top; d->right = s->right; d->bottom = s->bottom;
}

static inline void w16_rect_out(RECT16 *d, const RECT *s)
{
    d->left = (int16_t)s->left; d->top = (int16_t)s->top;
    d->right = (int16_t)s->right; d->bottom = (int16_t)s->bottom;
}

#endif
