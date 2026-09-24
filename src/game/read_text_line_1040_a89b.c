/* read_text_line_1040_a89b -- shipped at 1040:a89b.
 *
 * Ported by tools/port.py from matched/game/read_text_line_1040_a89b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 read_text_line(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e);

col_i16 read_text_line_1040_a89b(col_i16 a, col_i16 b, col_i16 c, col_i16 d, col_i16 e)
{
    return read_text_line(a, b, c, d, e);
}
