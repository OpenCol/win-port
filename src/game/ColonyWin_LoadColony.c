/* ColonyWin_LoadColony: candidate for 1010:bb30 (527 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_LoadColony
 * name-source: sibling-build
 *
 * Paints the colony window's backdrop into the saved GRPort at +0x7a8, which everything else
 * in the window then blits out of (`ColonyWin_UpdateColony`, the next function, is the
 * blitter).
 *
 * TWO TILINGS SIDE BY SIDE. The left 0x190 columns get sprite 384 on a flat 0x20 grid down to
 * y = 0x138; the right-hand strip from 0x190 to 0x280 gets sprite 383 on a 0x3e by 0x2f grid
 * down to y = 0x149 -- the same sprite and the same spacing `PopupBoxDraw`
 * uses for its own background, so the two screens are papered alike.
 *
 * Four rectangles in colour 0 frame the two halves, each drawn twice one pixel in, which is
 * what makes the border two pixels thick. Then picture resource 0xfa, 0x28 by 0x60, is loaded
 * into a local port and blitted in at y = 0x109 -- directly under the two framed halves, which
 * are 0x109 tall.
 *
 * The local port has a destructor, so the function carries Borland's exception block; the two
 * size accessors come through the four-byte `PUSH CS` form and are stubs (rule 47).
 *
 * RENAMED 2026-09-26 from colony_paint_back: the Mac build's ColonyWin::LoadColony(void): this only; ColonyWin_Show calls it once to paint the window's backdrop port (+0x7a8) that every panel then blits from -- EuropeWin_LoadEurope's counterpart. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class ColonyWin : public ColChild { public: SRegion region; GRPort back; GRPort terrain; SImageButton ib[3]; SButton b[2]; SImageButton ib2; char buy_shown; char change_shown; short panel; short cx; short cy; int timer; int colony; ColonyWin(); ~ColonyWin(); };
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_DrawRect(GRPort *p, RECT *r);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int port_width_1010_ebd4(GRPort *port);
extern "C" int port_height_1010_ebed(GRPort *port);

void ColonyWin_LoadColony(ColonyWin *w)
{
    GRPort pic;
    RECT r;
    RECT r2;
    RECT t1;
    RECT t2;
    register int x;
    register int y;

    GRPort_Init_wh(&w->back, 0x280, 0x1b8);
    for (y = 0; y < 0x138; y += 0x20)
        for (x = 0; x < 0x190; x += 0x20)
            Sprite_Draw(&t1, &g_route_panel_sprite, &w->back, x, y);
    for (y = 0; y < 0x149; y += 0x2f)
        for (x = 0x190; x < 0x280; x += 0x3e)
            Sprite_Draw(&t2, &g_background_tile, &w->back, x, y);
    TextColor(0);
    SetRect(&r, 0, 0, 0x190, 0x109);
    GRPort_DrawRect(&w->back, &r);
    SetRect(&r, 1, 1, 0x18f, 0x108);
    GRPort_DrawRect(&w->back, &r);
    SetRect(&r, 0x190, 0, 0x280, 0x109);
    GRPort_DrawRect(&w->back, &r);
    SetRect(&r, 0x191, 1, 0x27f, 0x108);
    GRPort_DrawRect(&w->back, &r);
    GRPort_LoadPicture(&pic, 0xfa, 0x28, 0x60, (struct logpal *)&g_game_palette);
    SetRect(&r, 0, 0, port_width_1010_ebd4(&pic), port_height_1010_ebed(&pic));
    CopyRect(&r2, &r);
    OffsetRect(&r2, 0, 0x109);
    GRPort_CopyToPort(&pic, &w->back, &r, &r2);
}

