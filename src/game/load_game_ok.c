/* load_game_ok -- shipped at 1008:b24d.
 *
 * Ported by tools/port.py from matched/game/load_game_ok.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 load_game(void);

/* same object: 4-byte `push cs; call ` */


col_i16 load_game_ok(void)
{
    register col_i16 ok = 1;

    if (load_game() == 0)
        ok = 0;
    return ok;
}
