/* text_dialog_1048_ecef -- shipped at 1048:ecef.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1048_ecef.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 run_text_dialog(void *file, void *name, col_i16 a, void *d1, void *d2,
                               void *d3, col_i16 b);

col_i16 text_dialog_1048_ecef(void *file, void *name, col_i16 a, col_i16 b)
{
    return run_text_dialog(file, name, a, 0, 0, 0, b);
}
