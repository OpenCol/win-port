/* set_on_lclick_1010_2c6b: candidate for 1010:2c6b (57 bytes).  Verdict: EXACT.
 *
 * Module 1010's copy of the handler-slot setter for offset 12: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lclick_1010_2c6b(EventTable *table, void *h)
{
    void *old = table->on_lclick;
    table->on_lclick = h;
    return old;
}
