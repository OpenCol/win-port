/* min_char_ptr_1048_efbd -- shipped at 1048:efbd.
 *
 * Ported by tools/port.py from matched/game/min_char_ptr_1048_efbd.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * min_char_ptr_1048_efbd(char *a, char *b)
{
    if (*a < *b)
        return a;
    else
        return b;
}
