/* set_on_rclick_1040_ab3c: candidate for 1040:ab3c (57 bytes).  Verdict: EXACT.
 *
 * Module 1040's copy of the handler-slot setter for offset 24: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_rclick_1040_ab3c(EventTable *table, void *h)
{
    void *old = table->on_rclick;
    table->on_rclick = h;
    return old;
}
