/* text_dialog_1008_db8c: candidate for 1008:db8c (39 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The second step of segment 1008's TEXT dialog chain: adds the zero third argument.
 * `show_text_dialog_1008_cdf3` is what calls it, and it calls `text_dialog_1008_e651`.
 *
 * 39 bytes, not the census 35: the trailing `return` is written out (rule 44).
 *
 * The callee is -called and defined in-file as a stub (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int text_dialog_1008_db8c(void *file, void *name)
{
    return text_dialog_1008_e651(file, name, 0);
}
