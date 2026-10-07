/* ClosingWin_DoClosing: candidate for 1010:032c (195 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ClosingWin_DoClosing
 * name-source: sibling-build
 *
 * Plays the closing sequence on a ClosingWin: set-up (show_window_1010_2af0), sound
 * 0x3d, the animation at +0x100 with DoFireworks as its callback, the
 * message pump until DGROUP:0x4b98 is set, then sound off, animation stop,
 * the two windows (the dialog part at +0x26 and `win` at +0x678) hidden and the palette range 10..245
 * restored from +0x704.  The five module helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from closing_play: the Mac build's ClosingWin::DoClosing(void): this only, playing the closing sequence and pumping until it ends. The width matches.
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
class MagicWin : public GameWin { public: GRPort port; void *pal; RECT rect; int first, count; char bound; char fb; void *anim; MagicWin(); ~MagicWin(); };
#include "records.h"
class DiskFile { public: MSFile msfile; DiskFile(); DiskFile(char *name, int mode); ~DiskFile(); };
class Flic : public GRPort {
public:
    char pad_26[0xa6 - 0x26];
    DiskFile file;
    char pad_132[0x574 - 0x132];
    void *loop_hook;              /* +0x574, set_handler_574 */
    Flic();
    ~Flic();
};
class ClosingWin : public MagicWin {
public:
    Flic flic;
    EventWin win;
    char palette[0x9c8 - 0x704];
    ClosingWin();
    ~ClosingWin();
};
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */

extern int g_stub_a;
extern void sound_play(int n);
extern void DoFireworks(void);
extern void Flic_Loop(Flic *anim, void *port);

extern void Flic_Reset(Flic *anim);
struct logpal { char pad[1]; };
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern "C" void show_window_1010_2af0(void *w);

extern "C" void set_handler_574(void *anim, void *fn);

extern "C" void MSHideWindow_of_1010_2b41(void *port);

void ClosingWin_DoClosing(ClosingWin *w)
{
    show_window_1010_2af0(w);
    MSDoAllTasks_1010_2a54();
    sound_play(0x3d);
    set_handler_574(&w->flic, (void *)DoFireworks);
    Flic_Loop(&w->flic, w);
    while (g_closing_flag == 0)
        MSDoTask_1010_2b2a();
    MSCloseSound();
    Flic_Reset(&w->flic);
    MSHideWindow_of_1010_2b41(&(DialogWin &)*w);
    CTable_Fill(&g_game_palette, 10, 0xec, (unsigned char *)w->palette);
    MSDoAllTasks_1010_2a54();
    MSHideWindow_of_1010_2b41(&w->win);
    MSDoAllTasks_1010_2a54();
}
