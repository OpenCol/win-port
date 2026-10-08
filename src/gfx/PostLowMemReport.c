/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of get_ctable_1068_d77f removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of GameWin_Init, show_window_1068_db76, win_center_1068_dad3, win_disable_1068_db95 removed: the header declares them from the definition (W1 step 3). */
/* PostLowMemReport: candidate for 1068:b96a (745 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: PostLowMemReport
 *
 * The low-memory warning itself: builds a small modeless window over the caller's, sized to
 * the wrapped text of resource `res`, bevels it twice, draws the text, puts an "Okay" button
 * under it and hands that button `EndLowMemReport`. `LowMemTimerProc` is
 * the only caller and passes (666, the main window, 1) out of the globals
 * `EnableLowMemoryCheck` set.
 *
 * TWO STRINGS AND A RESOURCE TYPE, none of which a green verdict can check (§119):
 * DGROUP:0x3278 is the empty window name, DGROUP:0x3279 is "Okay", and the long at
 * DGROUP:0x325c is the four characters "TEXT" -- a Macintosh resource type, copied into a
 * local so its ADDRESS can be passed. That is the shape `MSGetResource` wants and the
 * reason the local exists at all.
 *
 * The geometry: 0x1a4 is the wrap width, the box is the wrapped text plus 0x20 across and
 * 0x48 down, the text is inset 0x10, and the 0x48x0x20 button is centred on the box
 * (`(w - 0x40) >> 1`) 0x30 up from the bottom. The `>> 1` is a SHIFT and not a divide:
 * `SAR AX,1` with no rounding correction in front of it.
 *
 * `new GameWin` is 0xc4 bytes and `new SButton` 0x22; the `SUB DWORD SS:[0x10],6` and
 * `SUB ...,2` after each constructor are rule 49's headcount, read at the allocating end.
 *
 * The button's parent is `(DialogWin *)g_lowmem_box`, an implicit upcast, and the shipped
 * code carries the null test Borland emits for one. The `+ 0x26` fed to `win_center_1068_dad3`
 * a line earlier has no test, so that one is pointer arithmetic -- the same two spellings
 * side by side.
 *
 * `GRPort_FillColor_all`, `TextColor` and `GRPort_Text` are -called THROUGH A NOP
 * and so are external; every other callee has the four-byte form and is a stub (rule 47).
 *
 * RENAMED 2026-09-27 from low_memory_warn_show: CONST PROPOSAL. Mac PostLowMemReport(unsigned short, EventWin&, short), CODE 1 0x1d04, 1212 bytes, called only by LowMemTimerProc. b96a (int res_id, void *win, int flag) has the same three arguments and is called only by low_memory_timer (= LowMemTimerProc). Callees: MSGetResource, lock, MSCalcTextRect (measure_wrapped_text), GameWin init, MSCenterWindow, a button, MSShowWindow, MSShowCursor, MSDoAllTasks, enable/disable window on both. Layout: Mac EndLowMemReport, PostLowMemReport, LowMemTimerProc, EnableLowMemoryCheck, DisableLowMemoryCheck; Windows b85e, b96a, bc53, bcc3, DisableLowMemoryCheck (bd25, already so named). Size 1212 against 745. C++ file (extern "C").
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SButton, Font, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-


class DlgControl;




extern GameWin *g_lowmem_box;          /* DGROUP:0x56ba */
extern SButton *g_lowmem_ok;           /* DGROUP:0x56be */


extern RECT *measure_wrapped_text(RECT *out, unsigned font,
                                                    char *s, int width);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_FillColor_all(GRPort *p, int color);

extern void GRPort_Text(GRPort *p, unsigned *hf, const char *s, RECT *r, int flags);
extern void FrameInfoRect(GRPort *port, RECT *r, int depth, int light, int dark);

extern void EndLowMemReport(void);

/* same object: 4-byte `push cs; call ` */
extern "C" void *MSGetResource_1068_da25(void *type, int id);
extern "C" void *MSLockResource_1068_da48(void *p);
extern "C" int font_handle_1068_da68(Font *font);

extern "C" void set_button_action_1068_db5c(SButton *button, void *h);


/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void ctl_init_button_1068_daf8(SButton *button, DialogWin *win, int y, RECT *r, char *title);
extern "C" void PostLowMemReport(int res_id, EventWin *win, int flag)
{
    register int w, h;
    RECT r;
    RECT btn;
    RECT frame;
    void *res_h;
    char *text;
    long type;

    type = g_restype_text;
    res_h = MSGetResource_1068_da25(&type, res_id);
    if (res_h == 0)
        return;
    text = (char *)MSLockResource_1068_da48(res_h);
    measure_wrapped_text(&r, font_handle_1068_da68(&g_log_font), text, 0x1a4);
    w = rect_width_1068_d79c(&r) + 0x20;
    h = rect_height_1068_d7bb(&r) + 0x48;
    OffsetRect(&r, 0x10, 0x10);
    SetRect(&btn, 0, 0, 0x48, 0x20);
    OffsetRect(&btn, (w - 0x40) >> 1, h - 0x30);
    g_lowmem_box = new GameWin;
    g_lowmem_ok = new SButton;
    GameWin_Init(g_lowmem_box, "", 0xc01, 0, 0, w, h, (CTable *)get_ctable_1068_d77f(win), win);
    GRPort_FillColor_all(g_lowmem_box, 7);
    win_center_1068_dad3((DialogWin *)((char *)g_lowmem_box + PORT_DIALOG_BASE));
    ctl_init_button_1068_daf8(g_lowmem_ok, g_lowmem_box, 0x65, &btn, "Okay");
    set_button_action_1068_db5c(g_lowmem_ok, (void *)EndLowMemReport);
    SetRect(&frame, 0, 0, w, h);
    FrameInfoRect(g_lowmem_box, &frame, 3, 0xff, 0xf8);
    InflateRect(&frame, -6, -6);
    FrameInfoRect(g_lowmem_box, &frame, -1, 0xff, 0xf8);
    FrameInfoRect(g_lowmem_box, &frame, 1, 0xff, 0xf8);
    TextColor(0);
    GRPort_Text(g_lowmem_box, (unsigned *)&g_log_font, text, &r, 0);
    show_window_1068_db76(g_lowmem_box);
    MSShowCursor();
    MSDoAllTasks_1068_d9b6();
    if (flag != 0)
        win_disable_1068_db95((DialogWin *)win);
}
