/* text_plain_step_1010_ed15: candidate for 1010:ed15 (48 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The step below `text_plain_<seg>_<off>`: the three pointers and `kind` through with a zero injected
 * before `kind` (the seventh of eight argument words), then `text_dialog_plain`. One of TWO copies.
 *
 * 48 bytes, not the census 44: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int text_plain_step_1010_ed15(void *p, void *ctx, char *name, int kind)
{
    return text_dialog_plain_1010_ef3c(p, ctx, name, 0, kind);
}
