/* set_on_lbutton_up_1038_e099: candidate for 1038:e099 (57 bytes).  Verdict: EXACT.
 *
 * Module 1038's copy of the handler-slot setter for offset 8: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lbutton_up_1038_e099(EventTable *table, void *h)
{
    void *old = table->on_lbutton_up;
    table->on_lbutton_up = h;
    return old;
}
