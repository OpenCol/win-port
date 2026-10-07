/* port_draw_sprite: candidate for 1068:aafb (54 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: port_draw_sprite
 * name-source: descriptive
 *
 * `Sprite_Draw(&o->rect, rec, o->port, x, y)`: draws a sprite into the object's own port at
 * +0xc4 and records where it landed in the rectangle at +0xee. `port_blit_and_select`
 * (immediately before it) uses the same two fields the same way -- "records the destination
 * rectangle at +0xee, blits into the port at +0xc4" -- and this is its sprite counterpart.
 *
 * The call has a NOP before its `push cs`, the five-byte form TLINK leaves for an `extern`,
 * so the callee is declared here and not defined (rule 47).
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
class MagicWin : public GameWin { public: GRPort port; void *pal; RECT rect; int first, count; char bound; char fb; void *anim; MagicWin(); ~MagicWin(); };
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void port_draw_sprite(MagicWin *o, void *rec, int x, int y)
{
    Sprite_Draw(&o->rect, (Sprite *)rec, &o->port, x, y);
}
