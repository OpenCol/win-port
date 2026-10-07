/* stop_mci_device_1068_a65f: candidate for 1068:a65f (19 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `stop_mci_device_1078_0940`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void stop_mci_device_1068_a65f(void)
{
    stop_mci_device_1078_0940();
}
