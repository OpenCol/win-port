/* StartCheat: candidate for 1048:a717 (383 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE CHEAT MENU. Typing Ctrl+W, Ctrl+I, Ctrl+N calls this (docs/findings/keyboard-and-cheat-
 * code.md); bit 13 of SEG20:0x7784 says whether the menu is up. Down: a `Cheat` menu is added
 * to the menu bar at SEG20:0x656 and its eleven items -- ids 0x320..0x32a, `Create Unit` through
 * `Test Routine` -- are inserted as menu 7, positions 1..11. Up: menu 7 is deleted. Either way the
 * game window's menu bar is redrawn and the bit flips.
 *
 * The flip is written in both arms, so the shipped has two `XOR WORD [0x7784],0x2000`. The
 * `(flag & 0x2000) == 0` test folds onto the high byte, `TEST BYTE [0x7785],0x20` (rule 59).
 * The twelve labels are string literals the comparator cannot see; they were read from DGROUP
 * 0x22ac..0x234a in push order (§119). The four module helpers come through the 4-byte `PUSH
 * CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from cheat_menu_toggle: the Mac build's StartCheat(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between MainWin_RightMouseClick and MainWin_ResetWindows, which are already bound; the arity agrees and the Mac body is 1.14x this one.
 */

#include "colonization.h"

#include "gameflag.h"
#include "classes.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern MainWin g_main_win;             /* SEG20:0x5ca */

/* same object: 4-byte `push cs; call ` */
extern void menu_insert_auto(MenuClass *m, int pos, int item, const char *text);
extern void menu_insert_1048_e260(MenuClass *m, int pos, int item, const char *text, unsigned id);
extern void win_redraw_menu_bar(void *w);
extern void MSDeleteMenuItem_1048_e2bb(MenuClass *m, int menu, int item);

void StartCheat(void)
{
    if ((g_game.game_flags & GAMEFLAG_CHEAT_MENU) == 0) {
        menu_insert_auto(&g_menu_owner, -1, -1, "Cheat");
        menu_insert_1048_e260(&g_menu_owner, 7, 1, "Create Unit", 0x320);
        menu_insert_1048_e260(&g_menu_owner, 7, 2, "Debug Info Flags", 0x321);
        menu_insert_1048_e260(&g_menu_owner, 7, 3, "Reveal Map", 0x322);
        menu_insert_1048_e260(&g_menu_owner, 7, 4, "Set Human Player", 0x323);
        menu_insert_1048_e260(&g_menu_owner, 7, 5, "Kill Indians", 0x324);
        menu_insert_1048_e260(&g_menu_owner, 7, 6, "Advance Revolution Status", 0x325);
        menu_insert_1048_e260(&g_menu_owner, 7, 7, "Sound Test", 0x326);
        menu_insert_1048_e260(&g_menu_owner, 7, 8, "Memory Check", 0x327);
        menu_insert_1048_e260(&g_menu_owner, 7, 9, "Show Strategy", 0x328);
        menu_insert_1048_e260(&g_menu_owner, 7, 10, "Show Colony Sites", 0x329);
        menu_insert_1048_e260(&g_menu_owner, 7, 11, "Test Routine", 0x32a);
        win_redraw_menu_bar(&g_main_win);
        g_game.game_flags ^= GAMEFLAG_CHEAT_MENU;
    } else {
        MSDeleteMenuItem_1048_e2bb(&g_menu_owner, 7, -1);
        win_redraw_menu_bar(&g_main_win);
        g_game.game_flags ^= GAMEFLAG_CHEAT_MENU;
    }
}
