/* set_on_rclick_1050_8fb9: candidate for 1050:8fb9 (57 bytes).  Verdict: EXACT.
 *
 * Module 1050's copy of the handler-slot setter for offset 24: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_rclick_1050_8fb9(EventTable *table, void *h)
{
    void *old = table->on_rclick;
    table->on_rclick = h;
    return old;
}
