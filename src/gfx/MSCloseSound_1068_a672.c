/* MSCloseSound_1068_a672: candidate for 1068:a672 (19 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSCloseSound`, named for the routine it reaches. One of
 * FORTY such forwarders left in the image, one or two per module, each reaching a
 * routine of the widget layer through a call; fifteen of them reach MSDoAllTasks.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSCloseSound_1068_a672(void)
{
    MSCloseSound();
}
