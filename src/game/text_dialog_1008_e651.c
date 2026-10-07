/* text_dialog_1008_e651: candidate for 1008:e651 (42 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The third step of segment 1008's TEXT dialog chain: its own third argument moves to the
 * FOURTH place and a zero takes the third. That reordering is what distinguishes this from
 * a plain default-argument wrapper -- the parameter it is given is not the parameter it
 * passes on in the same position.
 *
 * 42 bytes, not the census 38: the trailing `return` is written out (rule 44).
 *
 * The callee is -called and defined in-file as a stub (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int text_dialog_1008_e651(void *file, void *name, int kind)
{
    return text_dialog_1008_ebd0(file, name, 0, kind);
}
