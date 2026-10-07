/* return_arg2_1040_a4df: candidate for 1040:a4df (21 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
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
 */

#include "colonization.h"

int return_arg2_1040_a4df(void *unused1, int b, int unused2, int unused3)
{
    return b;
}
