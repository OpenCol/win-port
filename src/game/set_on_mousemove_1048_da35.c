/* set_on_mousemove_1048_da35: candidate for 1048:da35 (55 bytes).  Verdict: EXACT.
 *
 * Module 1048's copy of the handler-slot setter for offset 0: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_mousemove_1048_da35(EventTable *table, void *h)
{
    void *old = table->on_mousemove;
    table->on_mousemove = h;
    return old;
}
