/* show_plain_step_1048_e8e4: candidate for 1048:e8e4 (45 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The PLAIN dialog chain's own step, the one `show_text_dialog_1048_e35c` reaches:
 * three pointers (six words) through, a zero appended. One of TWO copies.
 *
 * 45 bytes, not the census 41: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int show_plain_step_1048_e8e4(void *p, void *dialog_ctx, char *name)
{
    return text_plain_step_1048_de5f(p, dialog_ctx, name, 0);
}
