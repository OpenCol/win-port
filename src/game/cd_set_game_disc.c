/* cd_set_game_disc -- shipped at 1040:8513.
 *
 * Ported by tools/port.py from matched/game/cd_set_game_disc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_cd_is_game_disc;                        DGROUP:0x1d1c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void cd_set_game_disc(void)
{
    g_cd_is_game_disc = 1;
}
