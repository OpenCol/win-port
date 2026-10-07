/* MagicWin_load_picture: candidate for 1068:ac02 (202 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MagicWin_load_picture
 * name-source: descriptive
 *
 * Loads a picture into the sprite's port and remaps it if the palette has moved: the palette range
 * (`first`, `count`) goes into +0xf6/+0xf8 and the palette pointer into +0xea, `GRPort_LoadPicture` fills the
 * GRPort at +0xc4, the port is selected against the object at +0x26's bits, and the sprite is
 * remapped only when those bits differ from the palette just stored. The port's rectangle is
 * copied out to +0xee at the end.
 *
 * ONE OF FOUR IDENTICAL 202-BYTE FUNCTIONS back to back at 1068:ac02, accc, ad96 and ae60,
 * differing ONLY in which loader they call first -- `GRPort_LoadPicture`, `load_gif_resource`,
 * `load_lbm_resource` and `load_pcx_resource`. Every other instruction is the same, including the
 * two separate `get_ctable_1068_d77f` calls: the result is not kept across `GRPort_ColorSync`,
 * because `-Od` keeps nothing across a call.
 *
 * The comparison against +0xea is done as two 16-bit halves, high first, which is what a 
 * pointer compared for inequality gives when the branch is written the short way round.
 *
 * `GRPort_LoadPicture` and `GRPort_ColorSync` are reached by the five-byte NOP form and are
 * `extern`; `get_ctable_1068_d77f`, `MagicWin_remap_to_palette` and `port_bounds` are reached by
 * the four-byte form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-27 from sprite_load_picture: the receiver is a MagicWin, not a Sprite: port at +0xc4, palette at +0xea, and every caller sits in MagicWin's module 1068:a698..b188 (521cb981e). The Mac build has no such method; the name stays descriptive.
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

struct logpal { char pad[1]; };
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *port, CTable *table);

/* same object: 4-byte `push cs; call ` */
extern "C" void *get_ctable_1068_d77f(void *p);
extern "C" void MagicWin_remap_to_palette(MagicWin *sprite, short first, short count);
extern "C" RECT *port_bounds(void *p);

void MagicWin_load_picture(MagicWin *o, int id, register int first, register int count, void *pal)
{
    o->first = first;
    o->count = count;
    o->pal = pal;
    GRPort_LoadPicture(&o->port, id, first, count, (struct logpal *)pal);
    GRPort_ColorSync(&o->port, (CTable *)get_ctable_1068_d77f(&(DialogWin &)*o));
    if (get_ctable_1068_d77f(&(DialogWin &)*o) != o->pal)
        MagicWin_remap_to_palette(o, first, count);
    CopyRect(&o->rect, port_bounds(&o->port));
}
