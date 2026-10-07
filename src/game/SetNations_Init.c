/* SetNations_Init: candidate for 1030:030a (701 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SetNations_Init
 * name-source: sibling-build
 * size-source: 701, not the census 694 -- the last seven bytes are the dead second epilogue.
 *
 * Builds and runs the "Select European Power" dialog and returns the nation, 1 to 4, or 0 for
 * cancel. `SetNations_DrawIt` draws it; this puts it together and turns the crank.
 *
 * THE FOUR BOXES ARE SET IN THE WRONG ORDER: box 1 first (the right-hand top), then box 0, then
 * 2 and 3. The screen reads left-to-right and the code does not, which is how the second
 * rectangle in the struct came to be written first.
 *
 * The parent window is whatever `IsBackdrop` hands back, and the game record at SEG20:0x5ca
 * when it hands back nothing. The two arms are otherwise identical -- same style, same size,
 * same palette -- and each carries its OWN empty string literal, at DGROUP:0x139a and 0x139b,
 * which Borland did not merge.
 *
 * The modal loop is `while (!done) MSDoTask()` with the flag at +0x152, set by the two buttons'
 * shared handler. Cancel sets +0x154 as well, and that is what turns the answer into 0 on the
 * way out.
 *
 * The return is a WORD (`MOV AX,[ES:BX+0xe6]`). `set_nation` keeps only its low byte and
 * widens it with `MOV AH,0`: `answer = (unsigned char)SetNations_Init(&sn)`. One prototype,
 * `int`, serves both; the merged module (segment 7) needs that, since a caller declaring
 * `unsigned char` would contradict the definition. (Corrected 2026-09-29.)
 *
 * RENAMED 2026-09-26 from run_nation_dialog: the Mac build's SetNations::Init(void): this only, and it is the method that builds the window, its two buttons and handlers (as MapWin_Init and MiniWin_Init do for theirs); DrawIt, the only other this-only method, is already bound.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SetNations, CTable removed: include/gen/classes.h define them once (W1 step 2). */
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
extern void SetNations_LeftMouseClick(void);
extern void SetNations_OkButtonClick(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void SetNations_DrawIt(SetNations *s);
void set_on_lbutton_down_1030_e522(void *o, void (*fn)()) { }
extern "C" void ctl_init_button_1030_e55b(void *button, void *win, int id, RECT *r, char *title);
void set_button_action_1030_e5bf(void *c, void (*fn)()) { }
extern "C" void win_center_1030_e5d9(void *w);

extern "C" void show_window_1030_e611(SetNations *s);

extern "C" void MSHideWindow_of_1030_e647(void *w);

int SetNations_Init(SetNations *s)
{
    RECT r;
    Screen *parent;

    s->hit = 0;
    s->choice = 1;
    s->done = 0;
    s->cancelled = 0;
    SetRect(&s->boxes[1], 0x1a6, 0x1e, 0x256, 0xe6);
    SetRect(&s->boxes[0], 0xdf, 0x1e, 0x18f, 0xe6);
    SetRect(&s->boxes[2], 0xdf, 0xfa, 0x18f, 0x1c2);
    SetRect(&s->boxes[3], 0x1a6, 0xfa, 0x256, 0x1c2);
    parent = (Screen *)IsBackdrop();
    if (parent != 0)
        GameWin_Init((GameWin *)s, "", 0x801, 0, 0, 0x280, 0x1e0,
                                         &g_game_palette, parent);
    else
        GameWin_Init((GameWin *)s, "", 0x801, 0, 0, 0x280, 0x1e0,
                                         &g_game_palette, (Screen *)&g_main_win);
    SetRect(&r, 0, 0, 0x280, 0x1e0);
    GRPort_LoadPicture(&s->port, 0x44c, 0x28, 0x9e, (struct logpal *)&g_game_palette);
    SetNations_DrawIt(s);
    set_on_lbutton_down_1030_e522(&(EventTable &)*s, SetNations_LeftMouseClick);
    SetRect(&r, 0, 0, 0xa0, 0x78);
    OffsetRect(&r, 0x20, 0x32);
    FrameInfoRect((GRPort *)s, &r, -2, 0xff, 0);
    SetRect(&r, r.left + 2, r.bottom - 0x3e, r.right - 2, r.bottom - 0x20);
    ctl_init_button_1030_e55b(&s->ok, &(DialogWin &)*s, 0x64, &r, "~Ok");
    set_button_action_1030_e5bf(&s->ok, SetNations_OkButtonClick);
    OffsetRect(&r, 0, 0x1e);
    ctl_init_button_1030_e55b(&s->cancel, &(DialogWin &)*s, 0x65, &r, "~Cancel");
    set_button_action_1030_e5bf(&s->cancel, SetNations_OkButtonClick);
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
