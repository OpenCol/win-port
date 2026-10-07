/* set_on_mousemove_1058_bf13: candidate for 1058:bf13 (55 bytes).  Verdict: EXACT.
 *
 * Module 1058's copy of the handler-slot setter for offset 0: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_mousemove_1058_bf13(EventTable *table, void *h)
{
    void *old = table->on_mousemove;
    table->on_mousemove = h;
    return old;
}
