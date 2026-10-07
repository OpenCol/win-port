/* CDControlWin_Init: candidate for 1040:961c (1923 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CDControlWin_Init
 * name-source: sibling-build
 *
 * THE CD PLAYER, built and run in one function. A 420 by 140 panel titled "CD Player", five transport
 * buttons across the middle, two sunken read-outs labelled Track and Time, and a modal loop that
 * redraws the time until the close button is pressed.
 *
 * THE FIVE BUTTONS are 80 by 50 at y 80, side by side from x 10, and they all go to the same handler
 * `CDControlWin_ButtonProc` with ids 0x64..0x68 -- so the handler tells them apart by id alone. Their art
 * ids are 0x30c, 0x30e, 0x30d, 0x30f and 0x310, which is NOT the order they are drawn in: the second
 * button gets 0x30e and the third 0x30d. The close button is the sixth, art 0xce, placed at the
 * window's own origin rather than at a laid-out rectangle.
 *
 * THE BACKGROUND is one sprite, SEG20:0x7506, tiled across the panel's interior on a 62 by 47 grid
 * inside a clip rectangle three pixels in from every edge -- the only place the panel's width and
 * height locals are read after being set.
 *
 * THE TWO READ-OUTS are filled with the colour at DGROUP:0x337e and given a three-pixel bevel drawn as
 * SIX SEPARATE LINES: three horizontal along the top and three vertical down the right, which is a
 * shadow on the wrong two sides for a sunken box and makes them look raised from one corner.
 *
 * TWO THINGS DONE AND NOT USED. A rectangle is set up at (0x10e, 0xa, 0x15e, 0x3c) and offset into
 * place before the first button, and nothing ever reads it -- a layout step for a control that is no
 * longer there. And an `SButton` is constructed at the top of the frame and destroyed at the bottom
 * without a single member ever being touched.
 *
 * WHAT THE BYTES SETTLED: `CDControlWin_DrawTrack` is reached by a REAL call even though it lives
 * in this very segment, so it has to be `extern` and not an in-file stub -- rule 47 read the other way
 * round, and the one thing that stopped this matching on the first build.
 *
 * RENAMED 2026-09-26 from cd_player_panel: the Mac build's CDControlWin::Init(void): this only, and it builds the CD panel, its buttons and their actions.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
typedef struct { short hfont, height; } Font;
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class CDControlWin : public ColChild {
public:
    char pad_77a[0x9e0 - 0x77a];
    int done;
    char pad_9e2[0xa56 - 0x9e2];
    int paused, track, stopped, tray_open;
};

typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void ColChild_InitCWin_nc(void *o, char *title, int which, int x, int y,
                              int w, int h, int save, int noclose);
extern void ColChild_DrawNonClient(ColChild *w);
extern void ColChild_Center(ColChild *w);

extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_FillColor(GRPort *p, RECT *r, int colour);

extern void GRPort_DrawLine(GRPort *w, short x1, short y1, short x2, short y2);
extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int size);
extern void disable_parent(void *win);
extern void enable_parent(void *win);
extern void CDControlWin_ButtonProc(void);
extern void CDControlWin_OnOkClick(void);
extern void CDControlWin_DrawTime(CDControlWin *w);
extern void CDControlWin_DrawTrack(CDControlWin *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void image_button_init_1040_ada4(void *btn, void *parent, int id, int x, int y, int pic1, int pic2, int cx, int cy);
void set_control_action_1040_aecc(void *p, void (*fn)(void)) { }
extern "C" void *get_win_1040_aee6(void *p);
extern "C" void show_window_1040_a79d(CDControlWin *w);

extern "C" void CDControlWin_Init(CDControlWin *w)
{
    register int x;
    register int y;
    RECT rc;
    int wd;
    int ht;
    RECT rc2;
    SButton b;

    wd = 0x1a4;
    ht = 0x8c;
    ColChild_InitCWin_nc(w, "CD Player", -1, 0, 0, 0x1a4, 0x8c, 0, 1);
    ColChild_DrawNonClient(w);
    ColChild_Center(w);
    SetRect(&rc, 0x10e, 0xa, 0x15e, 0x3c);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    SetRect(&rc, 0xa, 0x50, 0x5a, 0x82);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    image_button_init_1040_ada4((SImageButton *)((char *)w + 0x7ee), (DialogWin *)((char *)w + 0x26), 0x64,
                                rc.left, rc.top, 0x30c, 0x30c, 0x28, 0x60);
    set_control_action_1040_aecc((SImageButton *)((char *)w + 0x7ee), CDControlWin_ButtonProc);
    SetRect(&rc, 0x5a, 0x50, 0xaa, 0x82);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    image_button_init_1040_ada4((SImageButton *)((char *)w + 0x94a), (DialogWin *)((char *)w + 0x26), 0x65,
                                rc.left, rc.top, 0x30e, 0x30e, 0x28, 0x60);
    set_control_action_1040_aecc((SImageButton *)((char *)w + 0x94a), CDControlWin_ButtonProc);
    SetRect(&rc, 0xaa, 0x50, 0xfa, 0x82);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    image_button_init_1040_ada4((SImageButton *)((char *)w + 0x77a), (DialogWin *)((char *)w + 0x26), 0x66,
                                rc.left, rc.top, 0x30d, 0x30d, 0x28, 0x60);
    set_control_action_1040_aecc((SImageButton *)((char *)w + 0x77a), CDControlWin_ButtonProc);
    SetRect(&rc, 0xfa, 0x50, 0x14a, 0x82);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    image_button_init_1040_ada4((SImageButton *)((char *)w + 0x862), (DialogWin *)((char *)w + 0x26), 0x67,
                                rc.left, rc.top, 0x30f, 0x30f, 0x28, 0x60);
    set_control_action_1040_aecc((SImageButton *)((char *)w + 0x862), CDControlWin_ButtonProc);
    SetRect(&rc, 0x14a, 0x50, 0x19a, 0x82);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    image_button_init_1040_ada4((SImageButton *)((char *)w + 0x8d6), (DialogWin *)((char *)w + 0x26), 0x68,
                                rc.left, rc.top, 0x310, 0x310, 0x28, 0x60);
    set_control_action_1040_aecc((SImageButton *)((char *)w + 0x8d6), CDControlWin_ButtonProc);
    w->paused = 0;
    if (MSGetCDInfo(0, 0, (int *)((char *)w + 0xa58)) == 0)
        w->track = 0;
    SetRect(&rc, 3, 3, wd - 3, ht - 3);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    GRPort_SetClip(w, &rc);
    for (y = w->rect.top + 3; w->rect.top + ht - 3 > y; y += 0x2f) {
        for (x = w->rect.left + 3; w->rect.left + wd - 3 > x; x += 0x3e)
            Sprite_Draw(&rc2, &g_background_tile, w, x, y);
    }
    GRPort_ResetClip(w);
    SetRect(&rc, 0xa, 0xa, 0x3c, 0x3c);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    GRPort_FillColor(w, &rc, g_draw_color);
    TextColor(0x19);
    GRPort_DrawLine(w, rc.left, rc.top, rc.left + 0x32, rc.top);
    GRPort_DrawLine(w, rc.left, rc.top + 1, rc.left + 0x32, rc.top + 1);
    GRPort_DrawLine(w, rc.left, rc.top + 2, rc.left + 0x32, rc.top + 2);
    GRPort_DrawLine(w, rc.right - 2, rc.top + 2, rc.right - 2, rc.bottom);
    GRPort_DrawLine(w, rc.right - 1, rc.top + 2, rc.right - 1, rc.bottom);
    GRPort_DrawLine(w, rc.right, rc.top + 2, rc.right, rc.bottom);
    TextColor(0xc);
    CDControlWin_DrawTrack(w);
    GRPort_String(w, (unsigned *)&g_font_small, "Track", w->rect.left + 0x23, w->rect.top + 0x3c, 4);
    SetRect(&rc, 0x64, 0xa, 0xc8, 0x3c);
    OffsetRect(&rc, w->rect.left, w->rect.top);
    GRPort_FillColor(w, &rc, g_draw_color);
    TextColor(0x19);
    GRPort_DrawLine(w, rc.left, rc.top, rc.left + 0x64, rc.top);
    GRPort_DrawLine(w, rc.left, rc.top + 1, rc.left + 0x64, rc.top + 1);
    GRPort_DrawLine(w, rc.left, rc.top + 2, rc.left + 0x64, rc.top + 2);
    GRPort_DrawLine(w, rc.right - 2, rc.top + 2, rc.right - 2, rc.bottom);
    GRPort_DrawLine(w, rc.right - 1, rc.top + 2, rc.right - 1, rc.bottom);
    GRPort_DrawLine(w, rc.right, rc.top + 2, rc.right, rc.bottom);
    TextColor(0xc);
    CDControlWin_DrawTime(w);
    GRPort_String(w, (unsigned *)&g_font_small, "Time", w->rect.left + 0x96, w->rect.top + 0x3c, 4);
    image_button_init_1040_ada4((SImageButton *)((char *)w + 0x9e2), (DialogWin *)((char *)w + 0x26), 0xc8,
                                0, 0, 0xce, 0xce, 0x28, 0x60);
    set_control_action_1040_aecc((SImageButton *)((char *)w + 0x9e2), CDControlWin_OnOkClick);
    w->done = 0;
    w->tray_open = 1;
    w->stopped = 0;
    show_window_1040_a79d(w);
    MSDoAllTasks_1040_ac99();
    disable_parent((MSWin *)(get_win_1040_aee6((Screen *)((char *)w + 0x26))));
    while (w->done == 0) {
        CDControlWin_DrawTime(w);
        MSDoTask_1040_a7f5();
    }
    enable_parent((MSWin *)(get_win_1040_aee6((Screen *)((char *)w + 0x26))));
}
