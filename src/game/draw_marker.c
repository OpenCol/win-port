/* draw_marker: candidate for 1038:a849 (342 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: draw_marker
 * name-source: descriptive
 *
 * Draws a sprite on the map with an optional two-pixel SHADOW on either side. Bit 3 puts a
 * copy two pixels right, bit 0 a copy two pixels left, and bit 1 the sprite itself on top --
 * so the three bits are independent and a caller can ask for shadow without substance.
 *
 * Bit 2 chooses the shadow's colour: 0x5f when set, otherwise the byte at DGROUP:0x337e that
 * `CDControlWin_DrawTrack` also fills with.
 *
 * The map scale is set to `(w->zoom + 8) / 8` for the drawing and put back to 1/1 afterwards,
 * which is what makes the same sprite come out at the window's current zoom.
 *
 * +0x76a IS THE CLIP RECTANGLE, and its first two words are what other reconstructions in this
 * corpus call the window's `ox`/`oy`: the clip is passed WHOLE to `GRPort_SetClip` and
 * its corner is what every coordinate here is measured from. One field, two readings, and the
 * displacement byte tells them apart.
 *
 * `Sprite_DrawMono`'s last parameter is an `unsigned char`: the shipped sets AL in both arms of
 * the ternary where an `int` parameter gives `MOV AX,0x5F` and a widening of the global. That
 * corrects `ReportWin_ReportsLabor`, which passes a literal 0 there and so could not tell.
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
class MapWin : public ColChild { public: short cx; short cy; char pad_77e[4]; short zoom; short drag; short lbutton_held; MapWin(); ~MapWin(); };
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void SetGlobalSpriteScale(int num, int den);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *port, int x, int y, unsigned char mode);

void draw_marker(MapWin *w, register int spr, int x, int y, register int flags)
{
    int scale;
    int base;
    RECT r1;
    RECT r2;
    RECT r3;

    scale = w->zoom + 8;
    base = 8;
    SetGlobalSpriteScale(scale, base);
    GRPort_SetClip((GRPort *)w, &w->rect);
    if (flags & 8)
        Sprite_DrawMono(&r1, &g_icon_sprites[spr], (GRPort *)w, w->rect.left + x + 2, w->rect.top + y,
                     (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 1)
        Sprite_DrawMono(&r2, &g_icon_sprites[spr], (GRPort *)w, w->rect.left + x - 2, w->rect.top + y,
                     (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        Sprite_Draw(&r3, &g_icon_sprites[spr], (GRPort *)w, w->rect.left + x, w->rect.top + y);
    GRPort_ResetClip((GRPort *)w);
    SetGlobalSpriteScale(1, 1);
}
