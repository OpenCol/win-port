/* ColChild_DrawTitleBar: candidate for 1048:0ecf (394 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DrawTitleBar
 * name-source: sibling-build
 *
 * Draws a window's title bar, when the window has both a height and a width: the port is
 * clipped to the strip from (2, 2) to 0x1e down and two short of the right edge, the tile sprite
 * at SEG20:0x7506 is laid across it in two rows (width / tile width + 1 columns, each tile
 * w->tile_w by w->tile_h), and the title at +0xc4 is drawn centred at y 0x12 in colour 0xc with
 * the font at DGROUP:0x4b06. Then the port's rectangle and clip are put back, the strip copied to
 * the screen, and the map scale that `GetGlobalSpriteScale` saved restored after the tiles
 * were drawn at 1:1.
 *
 * THE TILE POSITIONS ARE `long` PRODUCTS truncated to the `int` arguments (rule 65): `MOV AX,DI /
 * LES BX / IMUL WORD [ES:BX+0x146]`, the counter loaded first. Every `int` spelling of the product
 * -- either operand order, casts on either side, `register` or promoted counters, `(*w).` or
 * `w[0].` -- loads the member into AX and multiplies by the register instead.
 *
 * RENAMED 2026-09-26 from draw_window_title: the Mac build's ColChild::DrawTitleBar(void): this only, painting the title bar strip. The width matches.
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
typedef struct { short hfont, height; } Font;
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void GRPort_GetClip(ColChild *o, RECT *r);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GetGlobalSpriteScale(int *num, int *den);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int align);
extern void GRPort_ResetClip(GRPort *port);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void copy_rect_to_screen(ColChild *win, RECT *r);

void ColChild_DrawTitleBar(ColChild *w)
{
    RECT r;
    RECT clip;
    int sx;
    int sy;
    int n;
    RECT out;
    int j;
    int i;

    if (port_height_1048_d7c5(w) != 0 && port_width_1048_d4f6(w) != 0) {
        GRPort_GetClip(w, &clip);
        SetRect(&r, 2, 2, port_width_1048_d4f6(w) - 2, 0x1e);
        GRPort_SetClip((GRPort *)w, &r);
        GetGlobalSpriteScale(&sx, &sy);
        SetGlobalSpriteScale(1, 1);
        n = port_width_1048_d4f6(w) / w->tile_w + 1;
        for (j = 0; j < 2; j++)
            for (i = 0; i < n; i++)
                Sprite_Draw(&out, &g_background_tile, (GRPort *)w, (long)i * w->tile_w, (long)j * w->tile_h);
        TextColor(0xc);
        GRPort_String((GRPort *)w, (unsigned *)&g_title_font, w->title, port_width_1048_d4f6(w) / 2, 0x12, 0x10);
        TextColor(0);
        GRPort_ResetClip((GRPort *)w);
        GRPort_SetClip((GRPort *)w, &clip);
        copy_rect_to_screen(w, &r);
        SetGlobalSpriteScale(sx, sy);
    }
}
