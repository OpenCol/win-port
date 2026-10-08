/* USER: window geometry and state, input, painting, scroll bars, timers, menus, message
   boxes (roadmap W4). Handles cross as 16-bit tokens, rectangles and points as 16-bit
   structures; Win16 and Win32 agree on the rest. */
#include <string.h>
#include "w16.h"

#define TOK(h) w16_token((HANDLE)(h))
#define HW(t) W16_HWND(t)
#define HM(t) ((HMENU)w16_handle(t))

/* Win16 PAINTSTRUCT, as include/gen/w16types.h has it */
typedef struct {
    uint16_t hdc; int16_t fErase; RECT16 rcPaint; int16_t fRestore, fIncUpdate; unsigned char rgbReserved[16];
} __attribute__((packed)) PAINTSTRUCT16;

/* ------------------------------------------------------------------ geometry */

void w16_GetClientRect(unsigned h, RECT16 *r) W16(GetClientRect);
void w16_GetClientRect(unsigned h, RECT16 *r)
{
    RECT rc;
    GetClientRect(HW(h), &rc);
    w16_rect_out(r, &rc);
    w16_trace("GetClientRect(%#x) = %ld,%ld,%ld,%ld", h, rc.left, rc.top, rc.right, rc.bottom);
}

void w16_GetWindowRect(unsigned h, RECT16 *r) W16(GetWindowRect);
void w16_GetWindowRect(unsigned h, RECT16 *r)
{
    RECT rc;
    GetWindowRect(HW(h), &rc);
    w16_rect_out(r, &rc);
    w16_trace("GetWindowRect(%#x) = %ld,%ld,%ld,%ld", h, rc.left, rc.top, rc.right, rc.bottom);
}

void w16_ClientToScreen(unsigned h, POINT16 *p) W16(ClientToScreen);
void w16_ClientToScreen(unsigned h, POINT16 *p)
{
    POINT q = { p->x, p->y };
    ClientToScreen(HW(h), &q);
    p->x = (int16_t)q.x; p->y = (int16_t)q.y;
}

void w16_ScreenToClient(unsigned h, POINT16 *p) W16(ScreenToClient);
void w16_ScreenToClient(unsigned h, POINT16 *p)
{
    POINT q = { p->x, p->y };
    ScreenToClient(HW(h), &q);
    p->x = (int16_t)q.x; p->y = (int16_t)q.y;
}

void w16_MapWindowPoints(unsigned from, unsigned to, POINT16 *p, unsigned n) W16(MapWindowPoints);
void w16_MapWindowPoints(unsigned from, unsigned to, POINT16 *p, unsigned n)
{
    unsigned i;
    for (i = 0; i < n; i++) {
        POINT q = { p[i].x, p[i].y };
        MapWindowPoints(HW(from), HW(to), &q, 1);
        p[i].x = (int16_t)q.x; p[i].y = (int16_t)q.y;
    }
}

int w16_MoveWindow(unsigned h, int x, int y, int w, int hh, int repaint) W16(MoveWindow);
int w16_MoveWindow(unsigned h, int x, int y, int w, int hh, int repaint)
{
    w16_trace("MoveWindow(%#x, %d,%d %dx%d, %d)", h, (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)hh, repaint);
    return MoveWindow(HW(h), (int16_t)x, (int16_t)y, (int16_t)w, (int16_t)hh, repaint);
}

int w16_GetSystemMetrics(int i) W16(GetSystemMetrics);
int w16_GetSystemMetrics(int i)
{
    /* a lockstep run has the oracle's screen, 640 x 480 (W6): the game lays its windows out by it */
    int v = w16_lockstep() && (i == SM_CXSCREEN || i == SM_CYSCREEN) ? (i == SM_CXSCREEN ? 640 : 480)
                                                                    : GetSystemMetrics(i);
    w16_trace("GetSystemMetrics(%d) = %d", i, v);
    return v;
}

/* ------------------------------------------------------------------ window state */

int w16_ShowWindow(unsigned h, int cmd) W16(ShowWindow);
int w16_ShowWindow(unsigned h, int cmd)
{
    w16_trace("ShowWindow(%#x, %d)", h, cmd);
    return ShowWindow(HW(h), cmd);
}

void w16_UpdateWindow(unsigned h) W16(UpdateWindow);
void w16_UpdateWindow(unsigned h) { UpdateWindow(HW(h)); }

int w16_BringWindowToTop(unsigned h) W16(BringWindowToTop);
int w16_BringWindowToTop(unsigned h) { return BringWindowToTop(HW(h)); }

int w16_EnableWindow(unsigned h, int on) W16(EnableWindow);
int w16_EnableWindow(unsigned h, int on) { return EnableWindow(HW(h), on); }

int w16_IsWindow(unsigned h) W16(IsWindow);
int w16_IsWindow(unsigned h) { return h && IsWindow(HW(h)); }

int w16_IsWindowVisible(unsigned h) W16(IsWindowVisible);
int w16_IsWindowVisible(unsigned h) { return IsWindowVisible(HW(h)); }

int w16_IsIconic(unsigned h) W16(IsIconic);
int w16_IsIconic(unsigned h) { return IsIconic(HW(h)); }

int w16_IsZoomed(unsigned h) W16(IsZoomed);
int w16_IsZoomed(unsigned h) { return IsZoomed(HW(h)); }

int w16_IsChild(unsigned p, unsigned c) W16(IsChild);
int w16_IsChild(unsigned p, unsigned c) { return IsChild(HW(p), HW(c)); }

unsigned w16_GetParent(unsigned h) W16(GetParent);
unsigned w16_GetParent(unsigned h) { return TOK(GetParent(HW(h))); }

unsigned w16_GetWindow(unsigned h, unsigned cmd) W16(GetWindow);
unsigned w16_GetWindow(unsigned h, unsigned cmd) { return TOK(GetWindow(HW(h), cmd)); }

int w16_GetNextWindow(int h, int cmd) W16(GetNextWindow);
int w16_GetNextWindow(int h, int cmd) { return (int)TOK(GetWindow(HW((unsigned)h), cmd)); }

int w16_GetWindowText(unsigned h, char *s, int n) W16(GetWindowText);
int w16_GetWindowText(unsigned h, char *s, int n) { return GetWindowTextA(HW(h), s, n); }

void w16_SetWindowText(unsigned h, char *s) W16(SetWindowText);
void w16_SetWindowText(unsigned h, char *s) { SetWindowTextA(HW(h), s); }

unsigned w16_GetFocus(void) W16(GetFocus);
unsigned w16_GetFocus(void) { return TOK(GetFocus()); }

unsigned w16_SetFocus(unsigned h) W16(SetFocus);
unsigned w16_SetFocus(unsigned h) { return TOK(SetFocus(HW(h))); }

unsigned w16_SetCapture(unsigned h) W16(SetCapture);
unsigned w16_SetCapture(unsigned h) { return TOK(SetCapture(HW(h))); }

void w16_ReleaseCapture(void) W16(ReleaseCapture);
void w16_ReleaseCapture(void) { ReleaseCapture(); }

/* ------------------------------------------------------------------ input */

int w16_GetKeyState(int k) W16(GetKeyState);
int w16_GetKeyState(int k) { return (int16_t)GetKeyState(k); }

int w16_GetAsyncKeyState(int k) W16(GetAsyncKeyState);
int w16_GetAsyncKeyState(int k) { return (int16_t)GetAsyncKeyState(k); }

void w16_GetCursorPos(POINT16 *p) W16(GetCursorPos);
void w16_GetCursorPos(POINT16 *p)
{
    POINT q;
    GetCursorPos(&q);
    p->x = (int16_t)q.x; p->y = (int16_t)q.y;
}

unsigned w16_SetCursor(unsigned c) W16(SetCursor);
unsigned w16_SetCursor(unsigned c) { return TOK(SetCursor((HCURSOR)w16_handle(c))); }

int w16_ShowCursor(int on) W16(ShowCursor);
int w16_ShowCursor(int on) { return ShowCursor(on); }

int w16_DestroyCursor(unsigned c) W16(DestroyCursor);
int w16_DestroyCursor(unsigned c)
{
    int r = DestroyCursor((HCURSOR)w16_handle(c));
    w16_forget(c);
    return r;
}

unsigned long w16_GetTickCount(void) W16(GetTickCount);
unsigned long w16_GetTickCount(void) { return w16_lockstep() ? w16_lockstep_ticks() : GetTickCount(); }

void w16_MessageBeep(unsigned t) W16(MessageBeep);
void w16_MessageBeep(unsigned t) { MessageBeep(t); }

int w16_MessageBox(unsigned h, char *text, char *title, unsigned type) W16(MessageBox);
int w16_MessageBox(unsigned h, char *text, char *title, unsigned type)
{
    return MessageBoxA(HW(h), text, title, type);
}

/* ------------------------------------------------------------------ painting */

unsigned w16_BeginPaint(unsigned h, PAINTSTRUCT16 *p) W16(BeginPaint);
unsigned w16_BeginPaint(unsigned h, PAINTSTRUCT16 *p)
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(HW(h), &ps);
    memset(p, 0, sizeof *p);
    p->hdc = (uint16_t)TOK(dc);
    p->fErase = (int16_t)ps.fErase;
    w16_rect_out(&p->rcPaint, &ps.rcPaint);
    return p->hdc;
}

void w16_EndPaint(unsigned h, PAINTSTRUCT16 *p) W16(EndPaint);
void w16_EndPaint(unsigned h, PAINTSTRUCT16 *p)
{
    PAINTSTRUCT ps;
    memset(&ps, 0, sizeof ps);
    ps.hdc = W16_HDC(p->hdc);
    ps.fErase = p->fErase;
    w16_rect_in(&ps.rcPaint, &p->rcPaint);
    EndPaint(HW(h), &ps);
    w16_forget(p->hdc);
}

void w16_InvalidateRect(unsigned h, RECT16 *r, int erase) W16(InvalidateRect);
void w16_InvalidateRect(unsigned h, RECT16 *r, int erase)
{
    RECT rc;
    if (r)
        w16_rect_in(&rc, r);
    InvalidateRect(HW(h), r ? &rc : 0, erase);
}

void w16_ValidateRect(unsigned h, RECT16 *r) W16(ValidateRect);
void w16_ValidateRect(unsigned h, RECT16 *r)
{
    RECT rc;
    if (r)
        w16_rect_in(&rc, r);
    ValidateRect(HW(h), r ? &rc : 0);
}

unsigned w16_GetDC(unsigned h) W16(GetDC);
unsigned w16_GetDC(unsigned h) { return TOK(GetDC(HW(h))); }

int w16_ReleaseDC(unsigned h, unsigned dc) W16(ReleaseDC);
int w16_ReleaseDC(unsigned h, unsigned dc)
{
    int r = ReleaseDC(HW(h), W16_HDC(dc));
    w16_forget(dc);
    return r;
}

int w16_FillRect(unsigned dc, RECT16 *r, unsigned br) W16(FillRect);
int w16_FillRect(unsigned dc, RECT16 *r, unsigned br)
{
    RECT rc;
    w16_rect_in(&rc, r);
    return FillRect(W16_HDC(dc), &rc, br <= COLOR_BTNHIGHLIGHT + 1 ? (HBRUSH)(uintptr_t)br : (HBRUSH)w16_handle(br));
}

int w16_FrameRect(unsigned dc, RECT16 *r, unsigned br) W16(FrameRect);
int w16_FrameRect(unsigned dc, RECT16 *r, unsigned br)
{
    RECT rc;
    w16_rect_in(&rc, r);
    return FrameRect(W16_HDC(dc), &rc, (HBRUSH)w16_handle(br));
}

int w16_DrawText(unsigned dc, const char *s, int n, RECT16 *r, unsigned fmt) W16(DrawText);
int w16_DrawText(unsigned dc, const char *s, int n, RECT16 *r, unsigned fmt)
{
    RECT rc;
    int h;
    w16_rect_in(&rc, r);
    h = DrawTextA(W16_HDC(dc), s, (int16_t)n, &rc, fmt);
    if (fmt & DT_CALCRECT)
        w16_rect_out(r, &rc);
    return h;
}

/* ------------------------------------------------------------------ scroll bars and timers */

int w16_GetScrollPos(unsigned h, int bar) W16(GetScrollPos);
int w16_GetScrollPos(unsigned h, int bar) { return GetScrollPos(HW(h), bar); }

int w16_SetScrollPos(unsigned h, int bar, int pos, int redraw) W16(SetScrollPos);
int w16_SetScrollPos(unsigned h, int bar, int pos, int redraw) { return SetScrollPos(HW(h), bar, pos, redraw); }

void w16_GetScrollRange(int h, int bar, int *lo, int *hi) W16(GetScrollRange);
void w16_GetScrollRange(int h, int bar, int *lo, int *hi) { GetScrollRange(HW((unsigned)h), bar, lo, hi); }

void w16_SetScrollRange(int h, int bar, int lo, int hi, int redraw) W16(SetScrollRange);
void w16_SetScrollRange(int h, int bar, int lo, int hi, int redraw)
{
    SetScrollRange(HW((unsigned)h), bar, lo, hi, redraw);
}

int w16_ShowScrollBar(unsigned h, int bar, int show) W16(ShowScrollBar);
int w16_ShowScrollBar(unsigned h, int bar, int show) { return ShowScrollBar(HW(h), bar, show); }

unsigned w16_SetTimer(unsigned h, unsigned id, unsigned ms, void *fn) W16(SetTimer);
unsigned w16_SetTimer(unsigned h, unsigned id, unsigned ms, void *fn)
{
    /* the game sets no timer procedure: its timers arrive as WM_TIMER */
    (void)fn;
    if (w16_lockstep())                 /* fired from the virtual clock (W6) */
        return w16_lockstep_set_timer(HW(h), id, ms);
    return (unsigned)SetTimer(HW(h), id, ms, 0);
}

int w16_KillTimer(unsigned h, unsigned id) W16(KillTimer);
int w16_KillTimer(unsigned h, unsigned id) { return w16_lockstep() ? w16_lockstep_kill_timer(HW(h), id) : KillTimer(HW(h), id); }

/* ------------------------------------------------------------------ menus */

unsigned w16_CreateMenu(void) W16(CreateMenu);
unsigned w16_CreateMenu(void) { return TOK(CreateMenu()); }

unsigned w16_CreatePopupMenu(void) W16(CreatePopupMenu);
unsigned w16_CreatePopupMenu(void) { return TOK(CreatePopupMenu()); }

int w16_DestroyMenu(unsigned m) W16(DestroyMenu);
int w16_DestroyMenu(unsigned m)
{
    int r = DestroyMenu(HM(m));
    w16_forget(m);
    return r;
}

/* MF_POPUP: the id is a submenu token */
static UINT_PTR item_id(unsigned flags, unsigned id)
{
    return (flags & MF_POPUP) ? (UINT_PTR)w16_handle(id) : (UINT_PTR)(uint16_t)id;
}

int w16_AppendMenu(unsigned m, unsigned flags, unsigned id, const char *s) W16(AppendMenu);
int w16_AppendMenu(unsigned m, unsigned flags, unsigned id, const char *s)
{
    return AppendMenuA(HM(m), flags, item_id(flags, id), s);
}

int w16_InsertMenu(unsigned m, unsigned pos, unsigned flags, unsigned id, const char *s) W16(InsertMenu);
int w16_InsertMenu(unsigned m, unsigned pos, unsigned flags, unsigned id, const char *s)
{
    return InsertMenuA(HM(m), pos, flags, item_id(flags, id), s);
}

int w16_ModifyMenu(unsigned m, unsigned item, unsigned flags, unsigned id, const char *s) W16(ModifyMenu);
int w16_ModifyMenu(unsigned m, unsigned item, unsigned flags, unsigned id, const char *s)
{
    return ModifyMenuA(HM(m), item, flags, item_id(flags, id), s);
}

int w16_DeleteMenu(unsigned m, unsigned item, unsigned flags) W16(DeleteMenu);
int w16_DeleteMenu(unsigned m, unsigned item, unsigned flags) { return DeleteMenu(HM(m), item, flags); }

int w16_RemoveMenu(unsigned m, unsigned item, unsigned flags) W16(RemoveMenu);
int w16_RemoveMenu(unsigned m, unsigned item, unsigned flags) { return RemoveMenu(HM(m), item, flags); }

int w16_CheckMenuItem(unsigned m, unsigned item, unsigned flags) W16(CheckMenuItem);
int w16_CheckMenuItem(unsigned m, unsigned item, unsigned flags) { return (int)CheckMenuItem(HM(m), item, flags); }

int w16_EnableMenuItem(unsigned m, unsigned item, unsigned flags) W16(EnableMenuItem);
int w16_EnableMenuItem(unsigned m, unsigned item, unsigned flags) { return EnableMenuItem(HM(m), item, flags); }

unsigned w16_GetMenu(unsigned h) W16(GetMenu);
unsigned w16_GetMenu(unsigned h) { return TOK(GetMenu(HW(h))); }

int w16_SetMenu(unsigned h, unsigned m) W16(SetMenu);
int w16_SetMenu(unsigned h, unsigned m) { return SetMenu(HW(h), HM(m)); }

void w16_DrawMenuBar(unsigned h) W16(DrawMenuBar);
void w16_DrawMenuBar(unsigned h) { DrawMenuBar(HW(h)); }

unsigned w16_GetSubMenu(unsigned m, int pos) W16(GetSubMenu);
unsigned w16_GetSubMenu(unsigned m, int pos) { return TOK(GetSubMenu(HM(m), pos)); }

int w16_GetMenuItemCount(unsigned m) W16(GetMenuItemCount);
int w16_GetMenuItemCount(unsigned m) { return GetMenuItemCount(HM(m)); }

unsigned w16_GetMenuItemID(unsigned m, int pos) W16(GetMenuItemID);
unsigned w16_GetMenuItemID(unsigned m, int pos) { return (uint16_t)GetMenuItemID(HM(m), pos); }

int w16_TrackPopupMenu(unsigned m, unsigned flags, int x, int y, int res, unsigned h, const void *r) W16(TrackPopupMenu);
int w16_TrackPopupMenu(unsigned m, unsigned flags, int x, int y, int res, unsigned h, const void *r)
{
    RECT rc;
    if (r)
        w16_rect_in(&rc, (const RECT16 *)r);
    return TrackPopupMenu(HM(m), flags, (int16_t)x, (int16_t)y, res, HW(h), r ? &rc : 0);
}
