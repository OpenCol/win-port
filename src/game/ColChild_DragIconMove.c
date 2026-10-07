/* ColChild_DragIconMove: candidate for 1048:179d (388 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_DragIconMove
 *
 * Moves the dragged icon to (x, y) inside the owner's rectangle (+0x76a, InRect):
 * the saved background (port at DGROUP:0x4cbc) is put back where the icon was (the two
 * rectangles in data segment 30), the icon rectangle recentred on the pointer, the new
 * background saved, the icon sprite (DGROUP:0x4cb8) drawn through Sprite_Draw, and both
 * the new and the old areas flushed to the screen.
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
extern void *GetEventWindow(void);
extern GRPort *g_drag_port;        /* DGROUP:0x4cbc */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int InRect(RECT *r, int x, int y);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

/* same object: 4-byte `push cs; call ` (rule 47) */

typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern "C" void copy_rect_to_screen(void *win, RECT *r);
void ColChild_DragIconMove(int x, int y)
{
    ColChild *o;
    RECT r3;
    RECT r2;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ColChild *)w;
    if (InRect(&o->rect, x, y)) {
        GRPort_CopyToPort(g_drag_port, (GRPort *)o, &g_drag_rect_src, g_drag_rect_dst);
        CopyRect(&r2, g_drag_rect_dst);
        SetRect(g_drag_rect_dst, x - (rect_width_1048_d69b(&g_drag_rect_src) >> 1), y - (rect_height_1048_d6ba(&g_drag_rect_src) >> 1),
                x - (rect_width_1048_d69b(&g_drag_rect_src) >> 1) + rect_width_1048_d69b(&g_drag_rect_src),
                y - (rect_height_1048_d6ba(&g_drag_rect_src) >> 1) + rect_height_1048_d6ba(&g_drag_rect_src));
        GRPort_CopyToPort((GRPort *)o, g_drag_port, g_drag_rect_dst, &g_drag_rect_src);
        Sprite_Draw(&r3, g_drag_sprite, (GRPort *)o, g_drag_rect_dst[0].left, g_drag_rect_dst[0].top);
        copy_rect_to_screen(o, &r3);
        copy_rect_to_screen(o, &r2);
    }
}
