/* return_one_1070_84b3: candidate for 1070:84b3 (17 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Returns 1 and does nothing else: `MOV AX,1` between the prologue and the epilogue.
 *
 * NAMED BY ITS BODY, because nothing else names it. No relocation points at it and no call in
 * the image reaches it. It sits immediately after `MSDebugOut`, which sends its string to
 * OutputDebugString twice and then also returns 1, so it reads as that function's silent
 * twin -- a debug entry compiled to do nothing -- but position is not evidence, and the
 * `return_one_<addr>` convention says only what the bytes show.
 */

#include "colonization.h"

int return_one_1070_84b3(void)
{
    return 1;
}
