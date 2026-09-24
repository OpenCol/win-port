/* read_text_line_1040_a89b -- shipped at 1040:a89b.
 *
 * Ported by tools/port.py from matched/game/read_text_line_1040_a89b.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 read_text_line(void *f, char *out, col_i16 max);

col_i16 read_text_line_1040_a89b(void *f, char *out, col_i16 max)
{
    return read_text_line(f, out, max);
}
