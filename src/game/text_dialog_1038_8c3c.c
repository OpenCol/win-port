/* text_dialog_1038_8c3c: candidate for 1038:8c3c (53 bytes).  Verdict: RELOCATABLE.
 *
 * run_text_dialog with three zero longs: the map module's TEXT dialog call.
 */

#include "colonization.h"

int text_dialog_1038_8c3c(void *file, void *name, int sel, int kind)
{
    return run_text_dialog(file, name, sel, 0, 0, 0, kind);
}
