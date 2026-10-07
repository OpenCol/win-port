/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 119 of the imported file. */
/* PORT: this file's own declarations of SetPopupParent, delete_colony, show_window_1010_e404, win_release_1010_eb42 removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_Show: candidate for 1010:abc8 (562 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_Show
 * name-source: sibling-build
 *
 * Opens the colony screen on colony `colony` and RUNS IT: everything up to
 * `EventWin_Modal` is setup, that call is the whole time the player spends in the colony,
 * and everything after it is the tear-down. It is reached from a click on a colony
 * on the map (`MapWin_LeftMouseClick`, `MapWin_LeftMouseUp`), the colony key in `GameBrains_KeyboardEvent`,
 * `perform_orders_build`, `new_turn_colony` and `foreign_scout`.
 *
 * The window's own colony word at +0xa12 is written TWICE: once before the first redraw,
 * once after the show, and it is what `set_colony` was already told. The old value is
 * kept in `prev` and then never read: `if (prev == -1) ;` is an empty `if` in the shipped bytes
 * -- a `JNZ` over nothing -- and it is the only use the variable has.
 *
 * A NEW BUILDING IS ANNOUNCED HERE, not by whatever finished it. DGROUP:0xc20 carries the
 * building index across. A non-negative value, with DGROUP:0xa08 clear, flashes the building
 * through `ColonyWin_DoBuildingFizzle` -- building 0x10 alone is not flashed -- and then a
 * 0x2bc-millisecond timer is started on `ColonyWin_TimerProc`, which flips DGROUP:0xa06
 * between 0 and 1 and is what makes the new building blink. The handle lives at +0xa10 and is
 * reset on the way out.
 *
 * ABANDONING A COLONY IS DECIDED INSIDE AND ACTED ON OUTSIDE: the modal loop sets DGROUP:0xc1e
 * and this function calls `delete_colony` after the loop returns, so the colony
 * outlives the window it was abandoned in.
 *
 * `g_colony_window_open` (DGROUP:0xb2) is set here just before `EventWin_Modal` and cleared the
 * instant it returns, so it is exactly "the colony window is running". Every reader
 * branches on that: `attempt_cargo_to_ship` and `attempt_cargo_to_colony` refresh the window only
 * when it is up, and `possible_popup` falls back to the colony's MAP position for
 * its message when it is not.
 *
 * `g_reselect_unit`, set on the line after, keeps its address. Its three writers now read as one
 * thing -- `CheatMenuHuman` sets it, this sets it when the colony window closes, and
 * `GameBrains_DoHuman` clears it in two phases and tests it in a third to go back to phase 0
 * instead of idling -- but what phase 0 then does is the part that would name it.
 *
 * RENAMED 2026-09-26 from colony_window_open: the Mac build's ColonyWin::Show(short): this plus a colony, opening the colony screen on it and running it modally -- the role EuropeWin_Show and PediaWin_Show have for theirs.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, StatWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;             /* SEG20:0x5ca */

extern StatWin g_stat_win;           /* SEG20:0x1566 */

extern void ColChild_DrawNonClient(ColChild *w);
extern void set_colony(int colony);
extern void colony_indian_table(void);
extern void compute_colony_screen(void);
extern void ColonyWin_LoadColony(ColonyWin *w);
extern void ColonyWin_DrawColony(ColonyWin *w);
extern void ColChild_Center(ColChild *w);

extern void EventWin_Modal(EventWin *w);

extern void GameWin_Redraw(GameWin *rec);
extern void ColonyWin_TimerProc(void);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void win_disable_1010_e459(void *w);
extern "C" void ColonyWin_Init(ColonyWin *w);


extern "C" void ColonyWin_DoBuildingFizzle(ColonyWin *w);
extern "C" void label_remove_label(void);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void ColonyWin_Show(ColonyWin *w, register int colony)
{
    int prev;

    g_open_flag = 0;
    g_abandon_flag = 0;
    win_disable_1010_e459((Screen *)&g_main_win);
    ColonyWin_Init(w);
    SetPopupParent(&(DialogWin &)*w);
    ColChild_DrawNonClient((ColChild *)w);
    set_colony(colony);
    g_selected_colonist = 0;
    prev = w->colony;
    w->colony = colony;
    colony_indian_table();
    compute_colony_screen();
    ColonyWin_LoadColony(w);
    if (prev == -1)
        ;
    ColonyWin_DrawColony(w);
    copy_all_to_screen_1010_e3e6(w);
    g_colony_window_open = 1;
    ColChild_Center((ColChild *)w);
    show_window_1010_e404(w);
    win_show_and_focus_1010_e4ff(&(DialogWin &)*w);
    w->colony = colony;
    MSDoAllTasks_1010_e5bd();
    w->timer = -1;
    if (g_new_building >= 0 && g_scout_visit == 0) {
        if (g_new_building != 0x10)
            ColonyWin_DoBuildingFizzle(w);
        g_new_building = -1;
        w->timer = start_timer((void*)(ColonyWin_TimerProc), 0x2bc, -1);
        ColonyWin_DrawColony(w);
        copy_all_to_screen_1010_e3e6(w);
    }
    g_open_flag = 1;
    EventWin_Modal((EventWin *)(&(DialogWin &)*w));
    if (w->timer != -1) {
        ResetTimer(w->timer);
        w->timer = -1;
    }
    label_remove_label();
    g_colony_window_open = 0;
    g_reselect_unit = 1;
    if (g_abandon_flag != 0)
        delete_colony(colony);
    win_release_1010_eb42(w);
    SetPopupParent((EventWin *)&g_main_win);
    GameWin_Redraw((GameWin *)&g_stat_win);
    MSDoAllTasks_1010_e5bd();
}
