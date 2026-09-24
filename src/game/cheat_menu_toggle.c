/* cheat_menu_toggle -- shipped at 1048:a717.
 *
 * Ported by tools/port.py from matched/game/cheat_menu_toggle.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned far g_game_flags;                    SEG20:0x7784
 *   extern MenuRec far g_menu_owner;                     SEG20:0x0656
 *   extern char far g_game_record;                       SEG20:0x05ca
 *
 * The file's own MenuRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDeleteMenuItem_1048_e2bb(MenuRec *m, col_i16 menu, col_i16 item);
void menu_insert_1048_e260(MenuRec *m, col_i16 pos, col_i16 item, const char *text, col_u16 id);
void menu_insert_auto(MenuRec *m, col_i16 pos, col_i16 item, const char *text);
void win_redraw_menu_bar(void *w);

#include "gameflag.h"
/* same object: 4-byte `push cs; call ` */





void cheat_menu_toggle(void)
{
    if ((g_game_flags & GAMEFLAG_CHEAT_MENU) == 0) {
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
        win_redraw_menu_bar(&g_game_record);
        g_game_flags ^= GAMEFLAG_CHEAT_MENU;
    } else {
        MSDeleteMenuItem_1048_e2bb(&g_menu_owner, 7, -1);
        win_redraw_menu_bar(&g_game_record);
        g_game_flags ^= GAMEFLAG_CHEAT_MENU;
    }
}
