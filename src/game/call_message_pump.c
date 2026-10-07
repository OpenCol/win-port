/* call_message_pump: candidate for 1008:d157 (23 bytes).  Verdict: RELOCATABLE.
 *
 * Runs one turn of the widget layer's message pump (MSDoTask).
 */

#include "colonization.h"

void call_message_pump(void)
{
    MSDoTask();
    return;
}
