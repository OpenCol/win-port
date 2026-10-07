/* text_dialog_1008_ebd0: candidate for 1008:ebd0 (53 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The last step of segment 1008's TEXT dialog chain and the one that reaches
 * `run_text_dialog`, with the three long arguments zero. The same 53 bytes with the same
 * three `66 6A 00` pushes are `text_dialog_1038_8c3c` in the map module: one small function
 * compiled into every module that shows a TEXT dialog.
 *
 * 53 bytes, not the census 49: the trailing `return` is written out (rule 44).
 *
 * `run_text_dialog` is in another segment and is a call, so it is `extern`.
 */

#include "colonization.h"

int text_dialog_1008_ebd0(void *file, void *name, int sel, int kind)
{
    return run_text_dialog(file, name, sel, 0, 0, 0, kind);
}
