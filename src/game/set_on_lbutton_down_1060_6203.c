/* set_on_lbutton_down_1060_6203: candidate for 1060:6203 (57 bytes).  Verdict: EXACT.
 *
 * Module 1060's copy of the handler-slot setter for offset 4: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lbutton_down_1060_6203(EventTable *table, void *fn)
{
    void *old = table->on_lbutton_down;
    table->on_lbutton_down = fn;
    return old;
}
