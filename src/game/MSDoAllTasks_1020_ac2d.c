/* MSDoAllTasks_1020_ac2d: candidate for 1020:ac2d (19 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSDoAllTasks`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSDoAllTasks_1020_ac2d(void)
{
    MSDoAllTasks();
}
