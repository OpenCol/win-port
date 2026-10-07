/* GameWin_Resize: candidate for 1060:a2e4 (177 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GameWin_Resize
 * name-source: sibling-build
 *
 * Resizes a window, and does nothing at all when the size has not changed: the width and
 * height it is given are compared against `port_width_1060_aef6` and `port_height_1060_af15` on the
 * object, and only a difference in either goes on.
 *
 * What "going on" is, in order: build a rectangle of the new size at the origin, hand it to
 * the object's own VIRTUAL function, sync the port's colours to the colour table of the window at +0x26 (GRPort_ColorSync), point the
 * current-window reference at that port, and then run the resize handler and repaint.
 *
 * THE VIRTUAL CALL IS ON THE OBJECT ITSELF, not on a sub-object: `LES BX,[BP+6] /
 * MOV BX,[ES:BX] / CALL FAR [BX]` reads the vptr from offset 0, where
 * `PediaWin_Done` reads one from +0x818. So this window class has the vtable and that
 * one holds a second object that does.
 *
 * The two getters return `int`: their results are compared word-wide with `CMP AX,SI`, which
 * is what settled their return type when twenty forwarders of that family were corrected
 * from `void *`.
 *
 * RENAMED 2026-09-26 from win_resize: the Mac build's GameWin::Resize(short, short): this plus a width and a height, resizing the window when the size has changed. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };

typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *port, CTable *page);

/* same object: 4-byte `push cs; call ` */

extern "C" void *get_ctable_1060_af34(void *p);
extern "C" void call_relayout_handler(GameWin *table);

void GameWin_Resize(GameWin *w, register int width, register int height)
{
    RECT r;

    if (port_width_1060_aef6(w) != width || port_height_1060_af15(w) != height) {
        SetRect(&r, 0, 0, width, height);
        w->ResizePort(&r);
        GRPort_ColorSync((GRPort *)w, (CTable *)get_ctable_1060_af34(&(DialogWin &)*w));
        g_current_window = &(DialogWin &)*w;
        call_relayout_handler(w);
        copy_all_to_screen_1060_ae72(w);
    }
}

