/* MSDoTask_1010_2b2a: candidate for 1010:2b2a (23 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSDoTask`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * 23 bytes, not the census 19: the trailing `return` is written out, which costs a
 * second, unreachable epilogue (rule 44).
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

int MSDoTask_1010_2b2a(void)
{
    return MSDoTask();
}
