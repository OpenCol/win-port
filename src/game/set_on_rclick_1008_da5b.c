/* set_on_rclick_1008_da5b: candidate for 1008:da5b (57 bytes).  Verdict: EXACT.
 *
 * Module 1008's copy of the handler-slot setter for offset 24: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_rclick_1008_da5b(EventTable *table, void *h)
{
    void *old = table->on_rclick;
    table->on_rclick = h;
    return old;
}
