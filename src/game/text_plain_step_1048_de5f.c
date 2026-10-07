/* text_plain_step_1048_de5f: candidate for 1048:de5f (48 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The step below `text_plain_<seg>_<off>`: four arguments through with a zero injected
 * SECOND from last (between the name and the kind), then `text_dialog_plain`. One of TWO copies.
 *
 * 48 bytes, not the census 44: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int text_plain_step_1048_de5f(void *p, void *dialog_ctx, char *name, int kind)
{
    return text_dialog_plain_1048_e7f3(p, dialog_ctx, name, 0, kind);
}
