/* free_space_1068_9487: candidate for 1068:9487 (23 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `free_space`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * 23 bytes, not the census 19: the trailing `return` is written out, which costs a
 * second, unreachable epilogue (rule 44).
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void free_space_1068_9487(void)
{
    free_space();
    return;
}
