/* SetDifficulty_Init: candidate for 1030:0be3 (740 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetDifficulty_Init
 * name-source: sibling-build
 * size-source: 740, not the census 733 -- the last seven bytes are the dead second epilogue.
 *
 * Builds and runs the difficulty dialog and returns the level picked, 1 to 5, or 0 for cancel.
 * `set_difficulty` stores it less one, so box k is DIFFICULTY_* k-1 (difficul.h).
 *
 * IT IS `SetNations_Init` WITH FIVE BOXES, 0x8d9 bytes later in the same module:
 *   - the same parent fallback, picture load, bevel, "~Ok"/"~Cancel" pair, and modal loop;
 *   - its fields moved up by the fifth RECT: choice at +0xee, done at +0x15a, cancelled at +0x15c;
 *   - picture 0x44d instead of 0x44c;
 *   - its own click handlers, `SetDifficulty_LeftMouseClick` and `SetDifficulty_OkButtonClick`.
 * The boxes are two rows, two over three: 0 and 1 on top at x 0xff and 0x1d1, then 2, 3 and 4
 * below at x 0x2d, 0xff and 0x1d1. Unlike the nation dialog they are written in order.
 *
 * Two things the twin did not have, and the bytes fixed both. The two flags are cleared in the
 * other order, cancelled before done. After the click handler is installed, the window is copied
 * to the screen once (`copy_all_to_screen_1030_e504`) before the buttons are made.
 *
 * RENAMED 2026-09-26 from run_difficulty_dialog: the Mac build's SetDifficulty::Init(void), by the same reading as SetNations_Init, its twin two blocks up in module 1030. This module's copies of those two handlers (SetDifficulty_LeftMouseClick, SetDifficulty_OkButtonClick) were left unnamed on the belief that the Mac SetDifficulty has no LeftMouseClick or OkButtonClick of its own. It has both: SetDifficulty::LeftMouseClick(short, short) and SetDifficulty::OkButtonClick(short) are long-form MacsBug names (over 31 characters mangled), invisible until tools/mac_symbols.py read that form on 2026-09-28. They match SetNations_LeftMouseClick and SetNations_OkButtonClick, and the two unnamed functions are the same sizes as those twins (96 and 87 bytes).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SetDifficulty, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-






   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;             /* SEG20:0x5ca */

extern void *IsBackdrop(void);
extern void GameWin_Init(GameWin *o, char *name, int style,
                                                 int x, int y, int w, int h,
                                                 CTable *pal, Screen *parent);
struct logpal { char pad[1]; };
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
extern void FrameInfoRect(GRPort *s, RECT *r, int depth, int light, int dark);
extern void SetDifficulty_LeftMouseClick(void);
extern void SetDifficulty_OkButtonClick(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void SetDifficulty_DrawIt(SetDifficulty *s);

void set_on_lbutton_down_1030_e522(void *o, void (*fn)()) { }
extern "C" void ctl_init_button_1030_e55b(void *button, void *win, int id, RECT *r, char *title);
void set_button_action_1030_e5bf(void *c, void (*fn)()) { }
extern "C" void win_center_1030_e5d9(void *w);

extern "C" void show_window_1030_e611(SetDifficulty *s);

extern "C" void MSHideWindow_of_1030_e647(void *w);

int SetDifficulty_Init(SetDifficulty *s)
{
    RECT r;
    Screen *parent;

    s->hit = 0;
    s->choice = 1;
    s->cancelled = 0;
    s->done = 0;
    SetRect(&s->boxes[0], 0xff, 0x11, 0x188, 0xe7);
    SetRect(&s->boxes[1], 0x1d1, 0x11, 0x25a, 0xe7);
    SetRect(&s->boxes[2], 0x2d, 0xf8, 0xb6, 0x1cf);
    SetRect(&s->boxes[3], 0xff, 0xf8, 0x188, 0x1cf);
    SetRect(&s->boxes[4], 0x1d1, 0xf8, 0x25a, 0x1cf);
    parent = (Screen *)IsBackdrop();
    if (parent != 0)
        GameWin_Init((GameWin *)s, "", 0x801, 0, 0, 0x280, 0x1e0,
                                         &g_game_palette, parent);
    else
        GameWin_Init((GameWin *)s, "", 0x801, 0, 0, 0x280, 0x1e0,
                                         &g_game_palette, (Screen *)&g_main_win);
    SetRect(&r, 0, 0, 0x280, 0x1e0);
    GRPort_LoadPicture(&s->port, 0x44d, 0x28, 0xa8, (struct logpal *)&g_game_palette);
    SetDifficulty_DrawIt(s);
    set_on_lbutton_down_1030_e522(&(EventTable &)*s, SetDifficulty_LeftMouseClick);
    copy_all_to_screen_1030_e504(s);
    SetRect(&r, 0, 0, 0xa0, 0x78);
    OffsetRect(&r, 0x20, 0x32);
    FrameInfoRect((GRPort *)s, &r, -2, 0xff, 0);
    SetRect(&r, r.left + 2, r.bottom - 0x3e, r.right - 2, r.bottom - 0x20);
    ctl_init_button_1030_e55b(&s->ok, &(DialogWin &)*s, 0x64, &r, "~Ok");
    set_button_action_1030_e5bf(&s->ok, SetDifficulty_OkButtonClick);
    OffsetRect(&r, 0, 0x1e);
    ctl_init_button_1030_e55b(&s->cancel, &(DialogWin &)*s, 0x65, &r, "~Cancel");
    set_button_action_1030_e5bf(&s->cancel, SetDifficulty_OkButtonClick);
    win_center_1030_e5d9(&(DialogWin &)*s);
    MSDoAllTasks_1030_e5fe();
    show_window_1030_e611(s);
    while (s->done == 0)
        MSDoTask_1030_e630();
    MSHideWindow_of_1030_e647(&(DialogWin &)*s);
    MSDoAllTasks_1030_e5fe();
    if (s->cancelled != 0)
        s->choice = 0;
    return s->choice;
}
