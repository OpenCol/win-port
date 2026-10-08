/* USER: window classes, windows, messages and the message loop, Win16 on Win32 (roadmap W4).
 *
 * The game's window procedures are Win16-shaped: `long proc(unsigned hwnd, unsigned msg,
 * unsigned wParam, long lParam)`, cdecl, with a 16-bit window token and the Win16 packing of
 * WM_COMMAND, WM_*SCROLL, WM_MENUSELECT, WM_PARENTNOTIFY, WM_CTLCOLOR and WM_ACTIVATE. Every
 * class the game registers with one of its own procedures gets w16_wndproc, which converts
 * each Win32 message to the Win16 form and calls the game's procedure; SendMessage converts
 * the other way. A Win32 procedure (a system control's, which the game superclasses or
 * subclasses) crosses into the game as a cookie that only RegisterClass, SetWindowLong and
 * CallWindowProc understand.
 *
 * A message the game passes on to DefWindowProc unchanged is answered from the Win32
 * message it came from (the thunk keeps it), so messages whose Win32 lParam has no Win16
 * form (WM_NCCALCSIZE, WM_GETMINMAXINFO, ...) still reach Windows intact.
 */
#include <stdio.h>
#include <string.h>
#include "w16.h"

typedef long (*GAMEPROC)(unsigned, unsigned, unsigned, long);

/* The game's own structures, as its files declare them (the port's layout, not Win16's). */
typedef struct {
    unsigned style; GAMEPROC lpfnWndProc; int cbClsExtra, cbWndExtra;
    unsigned hInstance, hIcon, hCursor, hbrBackground; const char *lpszMenuName; const char *lpszClassName;
} WNDCLASS_G;
typedef struct { unsigned hwnd, message, wParam; long lParam; unsigned long time; int pt, y; } MSG_G;

/* ------------------------------------------------------------------ procedure cookies */

static char cookie_base[64];             /* &cookie_base[i] stands for win32_procs[i] */
static WNDPROC win32_procs[64];
static int n_procs;

static GAMEPROC cookie_for(WNDPROC p)
{
    int i;
    for (i = 0; i < n_procs; i++)
        if (win32_procs[i] == p)
            return (GAMEPROC)(void *)&cookie_base[i];
    if (n_procs == 64)
        return 0;
    win32_procs[n_procs] = p;
    return (GAMEPROC)(void *)&cookie_base[n_procs++];
}

static WNDPROC win32_of(GAMEPROC p)
{
    char *c = (char *)(void *)p;
    return c >= cookie_base && c < cookie_base + 64 ? win32_procs[c - cookie_base] : 0;
}

/* ------------------------------------------------------------------ which game proc */

LRESULT CALLBACK w16_wndproc(HWND, UINT, WPARAM, LPARAM);
static const char PROP_PROC[] = "w16.proc";      /* a game procedure set on one window */

static struct { ATOM atom; GAMEPROC proc; } classes[64];
static int n_classes;

static GAMEPROC game_proc(HWND h)
{
    int i;
    GAMEPROC p = (GAMEPROC)GetPropA(h, PROP_PROC);
    ATOM a;
    if (p)
        return p;
    a = (ATOM)GetClassWord(h, GCW_ATOM);
    for (i = 0; i < n_classes; i++)
        if (classes[i].atom == a)
            return classes[i].proc;
    return 0;
}

/* ------------------------------------------------------------------ message conversion */

#define TOK(h) w16_token((HANDLE)(h))

/* Win32 -> Win16, for the game's procedure. Returns 0 when the message has no Win16 form
   the game could use; it then goes to the Win32 default. */
static int to16(UINT msg, WPARAM wp, LPARAM lp, unsigned *m16, unsigned *w16, long *l16)
{
    *m16 = msg;
    *w16 = (unsigned)wp;
    *l16 = (long)lp;
    switch (msg) {
    case WM_COMMAND:
        *w16 = LOWORD(wp);
        *l16 = MAKELONG(TOK(lp), HIWORD(wp));
        break;
    case WM_HSCROLL: case WM_VSCROLL:
        *w16 = LOWORD(wp);
        *l16 = MAKELONG(HIWORD(wp), TOK(lp));
        break;
    case WM_MENUSELECT:
        *w16 = LOWORD(wp);
        *l16 = MAKELONG(HIWORD(wp), TOK(lp));
        break;
    case WM_MENUCHAR:
        *w16 = LOWORD(wp);
        *l16 = MAKELONG(HIWORD(wp), TOK(lp));
        break;
    case WM_PARENTNOTIFY:
        *w16 = LOWORD(wp);
        if (LOWORD(wp) == WM_CREATE || LOWORD(wp) == WM_DESTROY)
            *l16 = MAKELONG(TOK(lp), HIWORD(wp));
        break;
    case WM_ACTIVATE:
        *w16 = LOWORD(wp);
        *l16 = MAKELONG(TOK(lp), HIWORD(wp));
        break;
    case WM_CTLCOLORMSGBOX: case WM_CTLCOLOREDIT: case WM_CTLCOLORLISTBOX: case WM_CTLCOLORBTN:
    case WM_CTLCOLORDLG: case WM_CTLCOLORSCROLLBAR: case WM_CTLCOLORSTATIC:
        *m16 = 0x0019;                   /* Win16 WM_CTLCOLOR, the type in lParam's high word */
        *w16 = TOK(wp);
        *l16 = MAKELONG(TOK(lp), msg - WM_CTLCOLORMSGBOX);
        break;
    case WM_SETFOCUS: case WM_KILLFOCUS: case WM_PALETTECHANGED: case WM_PALETTEISCHANGING:
    case WM_SETCURSOR: case WM_MOUSEACTIVATE: case WM_ERASEBKGND: case WM_ICONERASEBKGND:
    case WM_INITMENU: case WM_INITMENUPOPUP:
        *w16 = TOK(wp);
        break;
    case WM_ACTIVATEAPP:
        *l16 = 0;                        /* Win16: a task handle; Win32: a thread id */
        break;
    case WM_GETMINMAXINFO: case WM_WINDOWPOSCHANGING: case WM_WINDOWPOSCHANGED:
    case WM_NCCALCSIZE: case WM_CREATE: case WM_NCCREATE:
        break;                           /* lParam handled by the caller */
    default:
        break;
    }
    return 1;
}

/* Win16 -> Win32, for SendMessage and DefWindowProc called by the game */
static void to32(unsigned m16, unsigned w16, long l16, UINT *msg, WPARAM *wp, LPARAM *lp)
{
    *msg = m16;
    *wp = w16;
    *lp = l16;
    switch (m16) {
    case WM_COMMAND:
        *wp = MAKEWPARAM(w16, HIWORD(l16));
        *lp = (LPARAM)w16_handle(LOWORD(l16));
        break;
    case WM_HSCROLL: case WM_VSCROLL:
        *wp = MAKEWPARAM(w16, LOWORD(l16));
        *lp = (LPARAM)w16_handle(HIWORD(l16));
        break;
    case WM_MENUSELECT: case WM_MENUCHAR:
        *wp = MAKEWPARAM(w16, LOWORD(l16));
        *lp = (LPARAM)w16_handle(HIWORD(l16));
        break;
    case WM_ACTIVATE:
        *wp = MAKEWPARAM(w16, HIWORD(l16));
        *lp = (LPARAM)w16_handle(LOWORD(l16));
        break;
    case 0x0019:
        *msg = WM_CTLCOLORMSGBOX + HIWORD(l16);
        *wp = (WPARAM)w16_handle(w16);
        *lp = (LPARAM)w16_handle(LOWORD(l16));
        break;
    case WM_SETFOCUS: case WM_KILLFOCUS: case WM_PALETTECHANGED: case WM_PALETTEISCHANGING:
    case WM_SETCURSOR: case WM_MOUSEACTIVATE: case WM_ERASEBKGND: case WM_ICONERASEBKGND:
    case WM_INITMENU: case WM_INITMENUPOPUP:
        *wp = (WPARAM)w16_handle(w16);
        break;
    default:
        break;
    }
}

/* ------------------------------------------------------------------ the thunk */

/* the Win32 message being handled, innermost first: DefWindowProc answers from it */
static struct { HWND hwnd; UINT msg; WPARAM wp; LPARAM lp; unsigned m16, w16; long l16; } stack[64];
static int depth;

typedef struct {                         /* Win16 CREATESTRUCT, as the game reads it */
    void *lpCreateParams; uint16_t hInstance, hMenu, hwndParent; int16_t cy, cx, y, x;
    int32_t style; const char *lpszName; const char *lpszClass; uint32_t dwExStyle;
} __attribute__((packed)) CREATESTRUCT16;

typedef struct { POINT16 ptReserved, ptMaxSize, ptMaxPosition, ptMinTrackSize, ptMaxTrackSize; } MINMAXINFO16;

LRESULT CALLBACK w16_wndproc(HWND h, UINT msg, WPARAM wp, LPARAM lp)
{
    GAMEPROC proc = game_proc(h);
    unsigned m16, w16;
    long l16, r;
    CREATESTRUCT16 cs16;
    MINMAXINFO16 mm16;
    if (!proc || depth == 64)
        return DefWindowProcA(h, msg, wp, lp);
    to16(msg, wp, lp, &m16, &w16, &l16);
    if (msg == WM_SIZE || msg == WM_MOVE)
        w16_trace("%s(%#x) %d,%d wp %u", msg == WM_SIZE ? "WM_SIZE" : "WM_MOVE", w16_token(h),
                  (int16_t)LOWORD(lp), (int16_t)HIWORD(lp), (unsigned)wp);
    if (msg == WM_CREATE || msg == WM_NCCREATE) {
        CREATESTRUCTA *cs = (CREATESTRUCTA *)lp;
        cs16.lpCreateParams = cs->lpCreateParams;
        cs16.hInstance = 0;
        cs16.hMenu = (uint16_t)TOK(cs->hMenu);
        cs16.hwndParent = (uint16_t)TOK(cs->hwndParent);
        cs16.cy = (int16_t)cs->cy; cs16.cx = (int16_t)cs->cx; cs16.y = (int16_t)cs->y; cs16.x = (int16_t)cs->x;
        cs16.style = cs->style; cs16.lpszName = cs->lpszName; cs16.lpszClass = cs->lpszClass;
        cs16.dwExStyle = cs->dwExStyle;
        l16 = (long)(intptr_t)&cs16;
    } else if (msg == WM_GETMINMAXINFO) {
        MINMAXINFO *mm = (MINMAXINFO *)lp;
        POINT *src = &mm->ptReserved;
        POINT16 *dst = &mm16.ptReserved;
        int i;
        if (w16_lockstep()) {
            /* the lockstep's 640 x 480 screen (W6), as winsim fills it: maximized, the window's
               frame just off it; tracked up to the screen and 12 */
            mm->ptMaxSize.x = 640 - 2 * mm->ptMaxPosition.x;
            mm->ptMaxSize.y = 480 - 2 * mm->ptMaxPosition.y;
            mm->ptMaxTrackSize.x = 640 + 12;
            mm->ptMaxTrackSize.y = 480 + 12;
            mm->ptMinTrackSize.x = 112;
            mm->ptMinTrackSize.y = 27;
            mm->ptReserved.x = mm->ptReserved.y = 0;
        }
        w16_trace("WM_GETMINMAXINFO(%#x) max %ldx%ld at %ld,%ld, track %ldx%ld..%ldx%ld", w16_token(h),
                  mm->ptMaxSize.x, mm->ptMaxSize.y, mm->ptMaxPosition.x, mm->ptMaxPosition.y,
                  mm->ptMinTrackSize.x, mm->ptMinTrackSize.y, mm->ptMaxTrackSize.x, mm->ptMaxTrackSize.y);
        for (i = 0; i < 5; i++) {
            dst[i].x = (int16_t)src[i].x;
            dst[i].y = (int16_t)src[i].y;
        }
        l16 = (long)(intptr_t)&mm16;
    } else if (msg == WM_NCCALCSIZE || msg == WM_WINDOWPOSCHANGING || msg == WM_WINDOWPOSCHANGED) {
        return DefWindowProcA(h, msg, wp, lp);   /* the game leaves these to Windows */
    }
    if (depth >= 12) {
        int i;
        w16_trace("message nesting %d: %#x to %p (from %#x %#x %#x)", depth, msg, (void *)h,
                  stack[depth - 1].msg, stack[depth - 2].msg, stack[depth - 3].msg);
        if (depth >= 40) {
            for (i = 0; i < depth; i++)
                w16_trace("  [%d] %#x to %p", i, stack[i].msg, (void *)stack[i].hwnd);
            ExitProcess(6);
        }
    }
    stack[depth].hwnd = h; stack[depth].msg = msg; stack[depth].wp = wp; stack[depth].lp = lp;
    stack[depth].m16 = m16; stack[depth].w16 = w16; stack[depth].l16 = l16;
    depth++;
    r = proc(TOK(h), m16, w16, l16);
    depth--;
    if (msg == WM_GETMINMAXINFO) {
        MINMAXINFO *mm = (MINMAXINFO *)lp;
        POINT *dst = &mm->ptReserved;
        POINT16 *src = &mm16.ptReserved;
        int i;
        for (i = 0; i < 5; i++) {
            dst[i].x = src[i].x;
            dst[i].y = src[i].y;
        }
    }
    /* Win16 WM_CTLCOLOR returns a brush token */
    if (m16 == 0x0019)
        return (LRESULT)w16_handle((unsigned)r);
    return r;
}

static LRESULT call_win32(HWND h, unsigned m16, unsigned w16, long l16, WNDPROC wp32, int def)
{
    UINT msg;
    WPARAM wp;
    LPARAM lp;
    int i;
    /* the message the thunk is handling, passed on unchanged: the Win32 original */
    for (i = depth - 1; i >= 0; i--)
        if (stack[i].hwnd == h && stack[i].m16 == m16 && stack[i].w16 == w16 && stack[i].l16 == l16) {
            msg = stack[i].msg; wp = stack[i].wp; lp = stack[i].lp;
            goto send;
        }
    to32(m16, w16, l16, &msg, &wp, &lp);
send:
    if (def)
        return DefWindowProcA(h, msg, wp, lp);
    return CallWindowProcA(wp32, h, msg, wp, lp);
}

long w16_DefWindowProc(unsigned hwnd, unsigned msg, unsigned wp, long lp) W16(DefWindowProc);
long w16_DefWindowProc(unsigned hwnd, unsigned msg, unsigned wp, long lp)
{
    LRESULT r = call_win32(W16_HWND(hwnd), msg, wp, lp, 0, 1);
    if (msg == 0x0019)
        return (long)TOK(r);
    return (long)r;
}

long w16_CallWindowProc(GAMEPROC proc, unsigned hwnd, unsigned msg, unsigned wp, long lp) W16(CallWindowProc);
long w16_CallWindowProc(GAMEPROC proc, unsigned hwnd, unsigned msg, unsigned wp, long lp)
{
    WNDPROC p32 = win32_of(proc);
    if (!p32)
        return proc(hwnd, msg, wp, lp);
    return (long)call_win32(W16_HWND(hwnd), msg, wp, lp, p32, 0);
}

long w16_SendMessage(unsigned hwnd, unsigned m16, unsigned w16, long l16) W16(SendMessage);
long w16_SendMessage(unsigned hwnd, unsigned m16, unsigned w16, long l16)
{
    UINT msg;
    WPARAM wp;
    LPARAM lp;
    LRESULT r;
    to32(m16, w16, l16, &msg, &wp, &lp);
    r = SendMessageA(W16_HWND(hwnd), msg, wp, lp);
    if (m16 == 0x0019)
        return (long)TOK(r);
    return (long)r;
}

/* ------------------------------------------------------------------ classes and windows */

unsigned w16_RegisterClass(WNDCLASS_G *g) W16(RegisterClass);
unsigned w16_RegisterClass(WNDCLASS_G *g)
{
    WNDCLASSA wc;
    WNDPROC p32 = win32_of(g->lpfnWndProc);
    ATOM a;
    memset(&wc, 0, sizeof wc);
    wc.style = g->style;
    wc.lpfnWndProc = p32 ? p32 : w16_wndproc;
    wc.cbClsExtra = g->cbClsExtra;
    wc.cbWndExtra = g->cbWndExtra;
    wc.hInstance = GetModuleHandleA(0);
    wc.hIcon = (HICON)w16_handle(g->hIcon);
    wc.hCursor = (HCURSOR)w16_handle(g->hCursor);
    wc.hbrBackground = g->hbrBackground <= COLOR_BTNHIGHLIGHT + 1 ? (HBRUSH)(uintptr_t)g->hbrBackground
                                                                  : (HBRUSH)w16_handle(g->hbrBackground);
    wc.lpszMenuName = 0;
    wc.lpszClassName = g->lpszClassName;
    a = RegisterClassA(&wc);
    w16_trace("RegisterClass(%s) = %#x", g->lpszClassName, a);
    if (a && !p32 && n_classes < 64) {
        classes[n_classes].atom = a;
        classes[n_classes++].proc = g->lpfnWndProc;
    }
    return a;
}

int w16_UnregisterClass(const char *cls, unsigned hinst) W16(UnregisterClass);
int w16_UnregisterClass(const char *cls, unsigned hinst)
{
    (void)hinst;
    return UnregisterClassA(cls, GetModuleHandleA(0));
}

unsigned w16_CreateWindow(const char *cls, const char *title, unsigned long style, int x, int y, int w, int h,
                          unsigned parent, unsigned menu, unsigned hinst, void *param) W16(CreateWindow);
unsigned w16_CreateWindow(const char *cls, const char *title, unsigned long style, int x, int y, int w, int h,
                          unsigned parent, unsigned menu, unsigned hinst, void *param)
{
    HWND hw;
    HMENU hm;
    (void)hinst;
#define USEDEFAULT16(v) ((int16_t)(v) == (int16_t)0x8000 ? CW_USEDEFAULT : (int)(int16_t)(v))
    /* a child's "menu" is its id; a top-level window's is a menu token */
    hm = (style & WS_CHILD) ? (HMENU)(uintptr_t)menu : (HMENU)w16_handle(menu);
    hw = CreateWindowExA(0, cls, title, style, USEDEFAULT16(x), USEDEFAULT16(y), USEDEFAULT16(w), USEDEFAULT16(h),
                         W16_HWND(parent), hm, GetModuleHandleA(0), param);
    w16_trace("CreateWindow(%s, %s, %#lx, %d,%d %dx%d) = %p", cls, title ? title : "", style, x, y, w, h, (void *)hw);
    return TOK(hw);
}

int w16_DestroyWindow(unsigned h) W16(DestroyWindow);
int w16_DestroyWindow(unsigned h)
{
    int r = DestroyWindow(W16_HWND(h));
    if (r)
        w16_forget(h);
    return r;
}

long w16_GetWindowLong(unsigned h, int i) W16(GetWindowLong);
long w16_GetWindowLong(unsigned h, int i)
{
    HWND hw = W16_HWND(h);
    if (i == GWLP_WNDPROC) {
        GAMEPROC g = (GAMEPROC)GetPropA(hw, PROP_PROC);
        WNDPROC p;
        if (g)
            return (long)(intptr_t)(void *)g;
        p = (WNDPROC)GetWindowLongPtrA(hw, GWLP_WNDPROC);
        if (p == w16_wndproc)
            return (long)(intptr_t)(void *)game_proc(hw);
        return (long)(intptr_t)(void *)cookie_for(p);
    }
    if (i == GWLP_HWNDPARENT)
        return (long)TOK(GetWindowLongPtrA(hw, i));
    return GetWindowLongA(hw, i);
}

long w16_SetWindowLong(unsigned h, int i, long v) W16(SetWindowLong);
long w16_SetWindowLong(unsigned h, int i, long v)
{
    HWND hw = W16_HWND(h);
    if (i == GWLP_WNDPROC) {
        long old = w16_GetWindowLong(h, i);
        GAMEPROC g = (GAMEPROC)(intptr_t)v;
        WNDPROC p32 = win32_of(g);
        if (p32) {
            RemovePropA(hw, PROP_PROC);
            SetWindowLongPtrA(hw, GWLP_WNDPROC, (LONG_PTR)p32);
        } else {
            SetPropA(hw, PROP_PROC, (HANDLE)(void *)g);
            SetWindowLongPtrA(hw, GWLP_WNDPROC, (LONG_PTR)w16_wndproc);
        }
        return old;
    }
    return SetWindowLongA(hw, i, v);
}

unsigned w16_GetClassWord(unsigned h, int i) W16(GetClassWord);
unsigned w16_GetClassWord(unsigned h, int i)
{
    HWND hw = W16_HWND(h);
    switch (i) {
    case -12: case -14: case -10:         /* GCW_HCURSOR, GCW_HICON, GCW_HBRBACKGROUND */
        return TOK(GetClassLongPtrA(hw, i));
    case -18:                             /* GCW_CBWNDEXTRA */
        return (unsigned)GetClassLongA(hw, GCL_CBWNDEXTRA);
    case -20:                             /* GCW_CBCLSEXTRA */
        return (unsigned)GetClassLongA(hw, GCL_CBCLSEXTRA);
    case -26:                             /* GCW_STYLE */
        return (unsigned)GetClassLongA(hw, GCL_STYLE);
    default:
        return GetClassWord(hw, i);
    }
}

unsigned w16_SetClassWord(unsigned h, int i, unsigned v) W16(SetClassWord);
unsigned w16_SetClassWord(unsigned h, int i, unsigned v)
{
    HWND hw = W16_HWND(h);
    if (i == -12 || i == -14 || i == -10)
        return TOK(SetClassLongPtrA(hw, i, (LONG_PTR)w16_handle(v)));
    return SetClassWord(hw, i, (WORD)v);
}

void w16_GetClassName(unsigned h, char *buf, int n) W16(GetClassName);
void w16_GetClassName(unsigned h, char *buf, int n)
{
    GetClassNameA(W16_HWND(h), buf, n);
}

/* ------------------------------------------------------------------ the message loop */

static MSG last;                         /* the Win32 message PeekMessage last returned */

static void msg_out(MSG_G *g, const MSG *m)
{
    unsigned m16, w16;
    long l16;
    to16(m->message, m->wParam, m->lParam, &m16, &w16, &l16);
    g->hwnd = TOK(m->hwnd);
    g->message = m16;
    g->wParam = w16;
    g->lParam = l16;
    g->time = m->time;
    g->pt = m->pt.x;
    g->y = m->pt.y;
}

int w16_PeekMessage(MSG_G *g, unsigned hwnd, unsigned lo, unsigned hi, unsigned remove) W16(PeekMessage);
int w16_PeekMessage(MSG_G *g, unsigned hwnd, unsigned lo, unsigned hi, unsigned remove)
{
    MSG m;
    for (;;) {
        if (!PeekMessageA(&m, W16_HWND(hwnd), lo, hi, remove)) {
            if (w16_lockstep())
                w16_lockstep_idle();      /* time passes as winsim's does (W6) */
            return 0;
        }
        /* a lockstep run hears no one but its script: the host's keys and mouse are dropped (W6) */
        if (!w16_lockstep() || w16_lockstep_input(&m))
            break;
        if (!(remove & PM_REMOVE))
            PeekMessageA(&m, m.hwnd, m.message, m.message, PM_REMOVE);
    }
    if (w16_lockstep())
        w16_lockstep_message();
    last = m;
    msg_out(g, &m);
    return 1;
}

/* the game hands back the MSG PeekMessage filled: the Win32 one it came from goes on */
static const MSG *msg_in(MSG_G *g, MSG *tmp)
{
    unsigned m16, w16;
    long l16;
    to16(last.message, last.wParam, last.lParam, &m16, &w16, &l16);
    if (g->hwnd == TOK(last.hwnd) && g->message == m16 && g->wParam == w16 && g->lParam == l16)
        return &last;
    memset(tmp, 0, sizeof *tmp);
    tmp->hwnd = W16_HWND(g->hwnd);
    to32(g->message, g->wParam, g->lParam, &tmp->message, &tmp->wParam, &tmp->lParam);
    tmp->time = g->time;
    tmp->pt.x = g->pt;
    tmp->pt.y = g->y;
    return tmp;
}

int w16_TranslateMessage(MSG_G *g) W16(TranslateMessage);
int w16_TranslateMessage(MSG_G *g)
{
    MSG tmp;
    return TranslateMessage(msg_in(g, &tmp));
}

long w16_DispatchMessage(MSG_G *g) W16(DispatchMessage);
long w16_DispatchMessage(MSG_G *g)
{
    MSG tmp;
    return (long)DispatchMessageA(msg_in(g, &tmp));
}
