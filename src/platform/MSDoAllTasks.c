/* MSDoAllTasks: candidate for 1078:11ae (24 bytes).  Verdict: RELOCATABLE.
 * symbol: MSDoAllTasks
 *
 * Runs queued tasks until MSDoTask reports there are none left. The task runner is -called and defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_i;

void MSDoAllTasks(void)
{
    while (MSDoTask())
        ;
}
