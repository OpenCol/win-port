/* text_dialog_1008_ebd0 -- shipped at 1008:ebd0.
 *
 * Ported by tools/port.py from matched/game/text_dialog_1008_ebd0.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 run_text_dialog(void *file, void *name, col_i16 a, col_i32 l1, col_i32 l2,
                               col_i32 l3, col_i16 b);

col_i16 text_dialog_1008_ebd0(void *file, void *name, col_i16 a, col_i16 b)
{
    return run_text_dialog(file, name, a, 0L, 0L, 0L, b);
}
