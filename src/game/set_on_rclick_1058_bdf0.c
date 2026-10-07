/* set_on_rclick_1058_bdf0: candidate for 1058:bdf0 (57 bytes).  Verdict: EXACT.
 *
 * Module 1058's copy of the handler-slot setter for offset 24: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_rclick_1058_bdf0(EventTable *table, void *h)
{
    void *old = table->on_rclick;
    table->on_rclick = h;
    return old;
}
