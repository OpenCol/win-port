/* set_on_close_1058_b9cf: candidate for 1058:b9cf (57 bytes).  Verdict: EXACT.
 *
 * Module 1058's copy of the handler-slot setter for offset 48: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_close_1058_b9cf(EventTable *table, void *h)
{
    void *old = table->on_close;
    table->on_close = h;
    return old;
}
