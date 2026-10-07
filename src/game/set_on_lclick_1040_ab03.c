/* set_on_lclick_1040_ab03: candidate for 1040:ab03 (57 bytes).  Verdict: EXACT.
 *
 * Module 1040's copy of the handler-slot setter for offset 12: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lclick_1040_ab03(EventTable *table, void *h)
{
    void *old = table->on_lclick;
    table->on_lclick = h;
    return old;
}
