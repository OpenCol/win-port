/* MSSeekFile_1040_a877 -- shipped at 1040:a877.
 *
 * Ported by tools/port.py from matched/game/MSSeekFile_1040_a877.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSSeekFile(void *f, col_i32 pos);

col_i32 MSSeekFile_1040_a877(void *f, col_i32 pos)
{
    return MSSeekFile(f, pos);
}
