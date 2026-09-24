/* text_dialog_plain_1010_ef3c -- shipped at 1010:ef3c.
 *
 * Ported by tools/port.py from matched/game/text_dialog_plain_1010_ef3c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 text_directive_parser(void *p, void *a, void *b, col_i16 c,
                                     void *z, void *d, void *e, void *f, col_i16 g);

col_i16 text_dialog_plain_1010_ef3c(void *p, void *a, void *b, col_i16 c, col_i16 g)
{
    return text_directive_parser(p, a, b, c, 0L, 0L, 0L, 0L, g);
}
