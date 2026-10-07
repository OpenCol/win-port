/* set_on_lbutton_down_1058_c2ce: candidate for 1058:c2ce (57 bytes).  Verdict: EXACT.
 *
 * Module 1058's copy of the handler-slot setter for offset 4: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lbutton_down_1058_c2ce(EventTable *table, void *h)
{
    void *old = table->on_lbutton_down;
    table->on_lbutton_down = h;
    return old;
}
