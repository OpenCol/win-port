/* ClosingWin_ctor: candidate for 1010:00a8 (503 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ClosingWin::ClosingWin()
 *
 * The constructor of ClosingWin (0x9c8 bytes, over MagicWin, vtable at DGROUP:0x998) with a
 * Flic at +0x100 and an EventWin at +0x678 (module 1010's copy): a desktop-sized window is
 * created for the EventWin (the DGROUP object at 0x9a9, style 0x801) with the game palette (SEG20:0x1ac)
 * packed into the buffer at +0x704, its handlers for slots 4 and 32 installed, the closing
 * animation loaded (resource 0x2580), the MagicWin's window created at the animation's size
 * (the DGROUP object at 0x9aa) with the same two handlers, and the flag at DGROUP:0x4b98 cleared.
 * EventWin::EventWin and the 1010 helpers it calls (win_set_back_color_1010_2978 through port_height_1010_2a80) are defined in-file first, for the 4-byte calls.
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

/* Both titles are the empty literal "": DGROUP:0x9a9 and 0x9aa are two one-byte strings in
   segment 3's pool, after "coldata7.dll" (tools/check_module_data.py, 2026-10-02). */

struct logpal;
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);

extern void EventWin_Init(char *w, char *res, int style, int x, int y, int width, int height, CTable *pal);
extern "C" void win_set_back_color_1010_2978(EventWin *w, int index);
extern "C" void win_center_1010_299c(void *w);
extern "C" void MSShowWindow_of_1010_29c1(EventWin *w);
extern "C" void *set_on_lbutton_down_1010_29e2(void *table, void *h);
extern "C" void *set_on_keydown_1010_2a1b(void *table, void *h);

extern void flic_load_resource2(Flic *flic, int res, int start, int count);
extern "C" int port_width_1010_2a67(Flic *port);
extern "C" int port_height_1010_2a80(Flic *port);
extern void MagicWin_Init(void *w, char *res, int style, int a, int b, int width, int height, void *pal, EventWin *parent);
extern void GRPort_FillColor_all(GRPort *p, int color);
extern void ClosingWin_Mouse(void);
extern void ClosingWin_Keys(void);

ClosingWin::ClosingWin()
{
    RECT desk;

    MSGetDesktopSize(&desk);
    CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), palette, 10, 0xec);
    set_wing_orientation(1);
    EventWin_Init((char *)&win, "", 0x801, 0, 0, desk.right, desk.bottom, &g_game_palette);
    win_set_back_color_1010_2978(&win, 0);
    win_center_1010_299c(&win);
    MSShowWindow_of_1010_29c1(&win);
    set_on_lbutton_down_1010_29e2(&(EventTable &)win, (void *)ClosingWin_Mouse);
    set_on_keydown_1010_2a1b(&(EventTable &)win, (void *)ClosingWin_Keys);
    MSDoAllTasks_1010_2a54();
    flic_load_resource2(&flic, 0x2580, 10, 0xec);
    MagicWin_Init(this, "", 0x801, 0, 0, port_width_1010_2a67(&flic), port_height_1010_2a80(&flic), &g_game_palette, &win);
    win_center_1010_299c(&(DialogWin &)*this);
    GRPort_FillColor_all(this, 0);
    set_on_lbutton_down_1010_29e2(&(EventTable &)*this, (void *)ClosingWin_Mouse);
    set_on_keydown_1010_2a1b(&(EventTable &)*this, (void *)ClosingWin_Keys);
    g_closing_flag = 0;
}
