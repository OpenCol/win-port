/* set_on_rbutton_down_1038_e0d2: candidate for 1038:e0d2 (57 bytes).  Verdict: EXACT.
 *
 * Module 1038's copy of the handler-slot setter for offset 16: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_rbutton_down_1038_e0d2(EventTable *table, void *h)
{
    void *old = table->on_rbutton_down;
    table->on_rbutton_down = h;
    return old;
}
