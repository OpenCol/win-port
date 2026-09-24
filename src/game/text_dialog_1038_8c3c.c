/* text_dialog_1038_8c3c -- shipped at 1038:8c3c.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1038_8c3c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 run_text_dialog(void *file, void *name, col_i16 a, void *d1, void *d2,
                               void *d3, col_i16 b);

col_i16 text_dialog_1038_8c3c(void *file, void *name, col_i16 a, col_i16 b)
{
    return run_text_dialog(file, name, a, 0, 0, 0, b);
}
