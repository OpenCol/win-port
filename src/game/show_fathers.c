/* show_fathers: candidate for 1020:9a90 (770 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: show_fathers
 *
 * THE CONTINENTAL CONGRESS SCREEN, called by `give_me_liberty` and by `1058:6ff7`. Picture 0x2ee is
 * the hall, and every father the nation has is drawn in his own seat: the seat order is the 25 bytes at
 * SEG22:0xce and the seat POSITIONS are the words at SEG22:0xe8 and 0x11a -- indexed by the SEAT, not by
 * the father, while the portrait sprite is `father + 0x2bc`.
 *
 * With a new father, the hall is drawn TWICE: once with his bit cleared, which is the picture the player
 * sees first, and then again with it set, remembering his seat's rectangle. That rectangle goes to
 * `DoFizzle`, the same highlighter the woodcut uses, so the new portrait is what flashes. Without a
 * father (-1) the screen is just shown.
 *
 * Spellings: the seat index rather than the father index on the position tables; `full` declared before
 * the highlight rectangle; the loop counter is the SI register variable and the father DI; and the
 * shared exit is an explicit `return;` (rule 41).
 *
 * RENAMED 2026-09-26 from show_congress: the Mac build's show_fathers(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller give_me_liberty; callee Draw; callee Execute; callee Init), the runner-up gets 3, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 *
 * THE DIALOG CLASS IS `SuperWin`, renamed from `EventDialog` 2026-09-28: its destructor call lands on
 * SuperWin::~SuperWin (1050:6b7d), and the image's RTTI names `SuperWin` (never `EventDialog`)
 * in this module's segment. Its constructor is the module's inline copy (EventDialog_ctor_*).
 * SuperWin::SuperWin is defined before the function, for the 4-byte call.
 */

#include "colonization.h"

#include "fathers.h"
#include "w16types.h"
typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SuperWin : public GameWin {
public:
    EventWin win;
    char palette[0x414 - 0x150];
    SuperWin();
    ~SuperWin();
};

extern void SuperWin_Init(SuperWin *w);
struct logpal;
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern int has_father(int nation, int bit);
extern void Sprite_Init(Sprite *s, int res);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void DoFizzle(GRPort *port, GameWin *o, RECT *r, int x, int y, int mode, int step);
extern void SuperWin_Execute(SuperWin *w);
extern void SuperWin_GoAway(SuperWin *w);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_father(int nation, int father, int on);

extern "C" void win_center_1020_abcb(void *p);
extern "C" void show_window_1020_ac0e(SuperWin *w);

void show_fathers(int nation, int father)
{
    SuperWin dlg;
    GRPort port;
    Sprite spr;
    RECT full;
    RECT hi;
    RECT r1;
    RECT r2;
    register int i;
    register int bit;

    SuperWin_Init(&dlg);
    GRPort_LoadPicture(&port, 0x2ee, 0x10, 0xcc, (struct logpal *)&g_game_palette);
    if (father >= 0)
        set_father(nation, father, 0);
    for (i = 0; i < FATHER_COUNT; i++) {
        bit = g_father_order[i];
        if (has_father(nation, bit) == 0)
            continue;
        Sprite_Init(&spr, bit + 0x2bc);
        Sprite_Draw(&r1, &spr, &port, g_father_x[i], g_father_y[i]);
    }
    SetRect(&full, 0, 0, 0x280, 0x1e0);
    GRPort_CopyToPort(&port, &dlg, &full, &full);
    copy_all_to_screen_1020_abf0(&dlg);
    win_center_1020_abcb((DialogWin *)((char *)&dlg + 0x26));
    show_window_1020_ac0e(&dlg);
    MSDoAllTasks_1020_ac2d();
    if (father >= 0) {
        set_father(nation, father, 1);
        GRPort_LoadPicture(&port, 0x2ee, 0x10, 0xcc, (struct logpal *)&g_game_palette);
        for (i = 0; i < FATHER_COUNT; i++) {
            bit = g_father_order[i];
            if (has_father(nation, bit) == 0)
                continue;
            Sprite_Init(&spr, bit + 0x2bc);
            Sprite_Draw(&r2, &spr, &port, g_father_x[i], g_father_y[i]);
            if (bit == father)
                SetRect(&hi, g_father_x[i], g_father_y[i],
                        rect_width_1020_ace9(&spr) + g_father_x[i],
                        rect_height_1020_ad08(&spr) + g_father_y[i]);
        }
        MSDoAllTasks_1020_ac2d();
        DoFizzle(&port, &dlg, &hi, hi.left, hi.top, -1, 8);
        SuperWin_Execute(&dlg);
    } else {
        SuperWin_Execute(&dlg);
    }
    SuperWin_GoAway(&dlg);
    return;
}

