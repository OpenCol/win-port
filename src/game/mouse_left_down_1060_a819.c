/* mouse_left_down_1060_a819 -- shipped at 1060:a819.
 *
 * Ported by tools/port.py from matched/game/mouse_left_down_1060_a819.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 mouse_left_down_1078_2cd2(void);

col_i16 mouse_left_down_1060_a819(col_i16 unused1, col_i16 unused2)
{
    return mouse_left_down_1078_2cd2();
}
