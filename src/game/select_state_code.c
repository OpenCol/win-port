/* select_state_code -- shipped at 1038:47ab.
 *
 * Ported by tools/port.py from matched/game/select_state_code.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern char far g_game_flags;                        SEG20:0x7784
 *
 * The file's own NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "control.h"
#include "gameflag.h"
col_i16 select_state_code(col_i16 x, col_i16 y)
{
    col_i16 r;

    switch (x) {
    case 2:
        r = 0x12;
        break;
    case 3:
        r = 0xb;
        break;
    case 1:
        r = (y < 4 && g_nation52[y].control == CONTROL_HUMAN) ? ((g_game_flags & GAMEFLAG_INDEPENDENCE) ? 7 : 4) : 8;
        break;
    case 0:
    default:
        r = (y < 4 && g_nation52[y].control == CONTROL_HUMAN) ? ((g_game_flags & GAMEFLAG_INDEPENDENCE) ? 9 : 4) : 6;
        break;
    }
    return r;
}
