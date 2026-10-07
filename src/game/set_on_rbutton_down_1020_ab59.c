/* set_on_rbutton_down_1020_ab59: candidate for 1020:ab59 (57 bytes).  Verdict: EXACT.
 *
 * Module 1020's copy of the handler-slot setter for offset 16: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_rbutton_down_1020_ab59(EventTable *table, void *h)
{
    void *old = table->on_rbutton_down;
    table->on_rbutton_down = h;
    return old;
}
