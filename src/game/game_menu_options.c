/* game_menu_options: candidate for 1048:b6ae (638 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_menu_options
 *
 * The `GAMEOPTIONS` dialog: eight checkboxes over the top bits of SEG20:0x7784 -- 0x8000,
 * 0x4000, 0x1000, 0x800, 0x400, 0x200, 0x100 and 0x80. They go in, the dialog runs, and
 * the word is rebuilt: three masks clear exactly those eight bits (`~0xd000`, `~0xf00`,
 * `~0x80`, in that order, which is how the shipped code spells it) and each checkbox that
 * came back set puts its bit back.
 *
 * SLOT 6 IS INVERTED, in both directions. It is set from `!(flags & 0x100)` and its bit is
 * restored when the box comes back CLEAR. So the option the player sees is the negation of
 * the stored bit -- an enable/disable printed the other way round. The `NEG AX / SBB AX,AX
 * / INC AX` on the way in and the `JNZ` on the way out are the only trace of it, and one of
 * eight otherwise identical statements being different is easy to read straight past.
 *
 * The same 0x7784 that add_liberty (§116) and test_routine use for the independence and
 * intervention latches: the low bits are game state, the top eight are preferences.
 *
 * RENAMED 2026-09-27 from get_game_options: String witness: GAMEOPTIONS is in Mac game_menu_options(void), and the Mac get_game_options has none of this function's strings. Call graph: Mac game_menu_options is called by MainWin::MainMenuProc and constructs a Popup, then does set_check x8, exec and get_check x8. b6ae is called by MainWin_MainMenuProc and calls Popup_ctor, Popup_set_check x8, popup_run_thunk, Popup_get_check x8 and Popup_dtor. Arity: (void) on both sides. Size is 584 Mac against 638 Windows. Why it went wrong: the RT_MENU and dispatch evidence correctly shows this is the 'Game Options...' handler. tools/mac_rename.py then substituted the Mac identifier whose words matched the caption ('get_game_options is the function that reads it'), and its arity check was unmeasured. The developers' names for the menu handlers are the game_menu_* family.
 */

#include "colonization.h"

#include "gameflag.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
extern GameState g_game;   /* SEG20:0x7782 */

extern void Popup_set_check(Popup *p, int key, int on);
extern int Popup_get_check(Popup *p, int key);

void game_menu_options(void)
{
    Popup p(0x800);

    show_text_dialog_1048_e35c(&p, "GAMEOPTIONS");
    Popup_set_check(&p, 0, g_game.game_flags & GAMEFLAG_SHOW_INDIAN_MOVES);
    Popup_set_check(&p, 1, g_game.game_flags & GAMEFLAG_SHOW_FOREIGN_MOVES);
    Popup_set_check(&p, 2, g_game.game_flags & GAMEFLAG_FAST_PIECE_SLIDE);
    Popup_set_check(&p, 3, g_game.game_flags & GAMEFLAG_END_OF_TURN);
    Popup_set_check(&p, 4, g_game.game_flags & GAMEFLAG_AUTOSAVE);
    Popup_set_check(&p, 5, g_game.game_flags & GAMEFLAG_COMBAT_ANALYSIS);
    Popup_set_check(&p, 6, !(g_game.game_flags & GAMEFLAG_NO_WATER_CYCLING));
    Popup_set_check(&p, 7, g_game.game_flags & GAMEFLAG_TUTORIAL);
    popup_run_thunk_1048_de13(&p);
    g_game.game_flags &= ~0xd000;
    g_game.game_flags &= ~0xf00;
    g_game.game_flags &= ~0x80;
    if (Popup_get_check(&p, 0))
        g_game.game_flags |= 0x8000;
    if (Popup_get_check(&p, 1))
        g_game.game_flags |= 0x4000;
    if (Popup_get_check(&p, 2))
        g_game.game_flags |= 0x1000;
    if (Popup_get_check(&p, 3))
        g_game.game_flags |= 0x800;
    if (Popup_get_check(&p, 4))
        g_game.game_flags |= 0x400;
    if (Popup_get_check(&p, 5))
        g_game.game_flags |= 0x200;
    if (!Popup_get_check(&p, 6))
        g_game.game_flags |= 0x100;
    if (Popup_get_check(&p, 7))
        g_game.game_flags |= 0x80;
}

