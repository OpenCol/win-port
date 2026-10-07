/* write_in_color: candidate for 1040:a4f4 (21 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Returns the word at BP+0xa -- its second argument -- and does nothing else. SIX of
 * these sit in a run at 1040:a4df..a59c, and a seventh shape beside them returns zero.
 * Something was compiled out of this module and left its signatures behind.
 *
 * THE ARGUMENT TOTAL IS THE CALLER'S, not the body's. SEG12, SEG13 and SEG7 all clean
 * TEN bytes after calling one of these -- a pointer, a pointer and a widened
 * byte -- while the body reads one word. The split beyond the returned word is not
 * established (§119); ten bytes is.
 *
 * 21 bytes, not the census 17: the written-out `return` adds a dead second epilogue
 * (rule 44).
 *
 * RENAMED 2026-09-26 from return_arg2_1040_a4f4: the Mac build's write_in_color(char*, short, short, short), bound by call-sequence alignment (1.5 aligned gaps in PediaWin_CargoLine, ReportWin_Score leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.81x this one. The Windows body only returns its second argument: the port kept the call sites and stubbed the coloured-text writer.
 */

#include "colonization.h"

int write_in_color(void *unused1, int x, int unused2, int unused3)
{
    return x;
}
