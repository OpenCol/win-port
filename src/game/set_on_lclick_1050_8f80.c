/* set_on_lclick_1050_8f80: candidate for 1050:8f80 (57 bytes).  Verdict: EXACT.
 *
 * Module 1050's copy of the handler-slot setter for offset 12: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lclick_1050_8f80(EventTable *table, void *h)
{
    void *old = table->on_lclick;
    table->on_lclick = h;
    return old;
}
