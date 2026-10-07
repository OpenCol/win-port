/* set_on_mousemove_1010_e7a1: candidate for 1010:e7a1 (55 bytes).  Verdict: EXACT.
 *
 * Module 1010's copy of the handler-slot setter for offset 0: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_mousemove_1010_e7a1(EventTable *table, void *h)
{
    void *old = table->on_mousemove;
    table->on_mousemove = h;
    return old;
}
