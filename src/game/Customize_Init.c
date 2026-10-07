/* PORT: this file's own declarations of ctl_init_button_1030_e55b removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 95, 99, 103 of the imported file. */
/* PORT: this file's own declarations of Customize_LeftMouseClick, Customize_OkButtonClick, GameWin_Init, IsBackdrop, MSHideWindow_of_1030_e647, show_window_1030_e611, win_center_1030_e5d9 removed: the header declares them from the definition (W1 step 3). */
/* Customize_Init: candidate for 1030:1397 (1158 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Customize_Init
 * name-source: sibling-build
 * size-source: 1158, not the census 1147 -- the `else` arm and a dead 9-byte epilogue follow the reachable code.
 *
 * BUILDS AND RUNS THE WORLD OPTIONS DIALOG (the "customize the New World" screen) and returns 1 for
 * Ok and 0 for Cancel. It is the third member of the family of `SetNations_Init` and
 * `SetDifficulty_Init`.
 *
 * TWELVE BOXES, a 4x3 grid: four options (columns at x 0x14, 0xac, 0x144, 0x1db) with three values
 * each (rows at y 0x27, 0xb6, 0x144). The four option words at SEG22:0x0 (0..2 each) become box
 * indices `option * 3 + value` in the dialog's +0x12c..+0x132 for the click handler
 * (`Customize_LeftMouseClick`) to move around. Ok writes them back as `index % 3`.
 *
 * Unlike its two siblings, the dialog places its Ok and Cancel buttons from the window's own size
 * (`get_w_2`/`get_w_4`): each a third of the width wide, Ok running from a sixth of the width to the middle and Cancel starting
 * at the middle, 30 pixels above the bottom. The picture is resource 0x44e.
 *
 * It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-26 from run_world_dialog: the Mac build's Customize::Init(void), the third of the module's three Init twins (SetNations_Init, SetDifficulty_Init).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, Customize, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-







   /* 0x41e bytes: the palette, its handle and the cycling state */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;              /* SEG20:0x5ca */

struct logpal { char pad[1]; };
extern int GRPort_LoadPicture(GRPort *p, int res, int w, int h, struct logpal *pal);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void Customize_DrawIt(Customize *s);
void set_on_lbutton_down_1030_e522(void *o, void (*fn)()) { }

void set_button_action_1030_e5bf(void *c, void (*fn)()) { }



int Customize_Init(Customize *s)
{
    RECT r;
    Screen *parent;
    register int i;

    s->hit[0] = 0;
    s->hit[1] = 0;
    s->hit[2] = 0;
    s->hit[3] = 0;
    s->choice[0] = g_world_opt[0];
    s->choice[1] = g_world_opt[1] + 3;
    s->choice[2] = g_world_opt[2] + 6;
    s->choice[3] = g_world_opt[3] + 9;
    s->family = 0;
    s->done = 0;
    s->cancelled = 0;
    SetRect(&s->boxes[0], 0x14, 0x27, 0xa3, 0x9c);
    SetRect(&s->boxes[1], 0x14, 0xb6, 0xa3, 0x12b);
    SetRect(&s->boxes[2], 0x14, 0x144, 0xa3, 0x1b8);
    SetRect(&s->boxes[3], 0xac, 0x27, 0x13b, 0x9c);
    SetRect(&s->boxes[4], 0xac, 0xb6, 0x13b, 0x12b);
    SetRect(&s->boxes[5], 0xac, 0x144, 0x13b, 0x1b8);
    SetRect(&s->boxes[6], 0x144, 0x27, 0x1d3, 0x9c);
    SetRect(&s->boxes[7], 0x144, 0xb6, 0x1d3, 0x12b);
    SetRect(&s->boxes[8], 0x144, 0x144, 0x1d3, 0x1b8);
    SetRect(&s->boxes[9], 0x1db, 0x27, 0x26a, 0x9c);
    SetRect(&s->boxes[10], 0x1db, 0xb6, 0x26a, 0x12b);
    SetRect(&s->boxes[11], 0x1db, 0x144, 0x26a, 0x1b8);
    parent = (Screen *)IsBackdrop();
    if (parent != 0)
        GameWin_Init((GameWin *)s, "", 0x801, 0, 0, 0x280, 0x1e0, &g_game_palette, parent);
    else
        GameWin_Init((GameWin *)s, "", 0x801, 0, 0, 0x280, 0x1e0, &g_game_palette, (Screen *)&g_main_win);
    SetRect(&r, 0, 0, 0x280, 0x1e0);
    GRPort_LoadPicture(&s->port, 0x44e, 0x28, 0xc0, (struct logpal *)&g_game_palette);
    Customize_DrawIt(s);
    set_on_lbutton_down_1030_e522(&(EventTable &)*s, (void*)(Customize_LeftMouseClick));
    SetRect(&r, 0, 0, port_width_1030_e93a(s) / 3, 0x19);
    OffsetRect(&r, port_width_1030_e93a(s) / 3 / 2, port_height_1030_e959(s) - 0x1e);
    ctl_init_button_1030_e55b(&s->ok, &(DialogWin &)*s, 0x64, &r, "~Ok");
    set_button_action_1030_e5bf(&s->ok, (void*)(Customize_OkButtonClick));
    SetRect(&r, port_width_1030_e93a(s) / 2, 0, port_width_1030_e93a(s) / 2 + port_width_1030_e93a(s) / 3, 0x19);
    OffsetRect(&r, 0, port_height_1030_e959(s) - 0x1e);
    ctl_init_button_1030_e55b(&s->cancel, &(DialogWin &)*s, 0x65, &r, "~Cancel");
    set_button_action_1030_e5bf(&s->cancel, (void*)(Customize_OkButtonClick));
    win_center_1030_e5d9(&(DialogWin &)*s);
    MSDoAllTasks_1030_e5fe();
    show_window_1030_e611(s);
    while (s->done == 0)
        MSDoTask_1030_e630();
    MSHideWindow_of_1030_e647(&(DialogWin &)*s);
    MSDoAllTasks_1030_e5fe();
    if (s->cancelled == 0) {
        for (i = 0; i < 4; i++)
            g_world_opt[i] = s->choice[i] % 3;
        return 1;
    } else
        return 0;
}
