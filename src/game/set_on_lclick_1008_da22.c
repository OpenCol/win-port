/* set_on_lclick_1008_da22: candidate for 1008:da22 (57 bytes).  Verdict: EXACT.
 *
 * Module 1008's copy of the handler-slot setter for offset 12: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lclick_1008_da22(EventTable *table, void *h)
{
    void *old = table->on_lclick;
    table->on_lclick = h;
    return old;
}
