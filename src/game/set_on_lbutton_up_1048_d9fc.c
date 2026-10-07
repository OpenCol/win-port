/* set_on_lbutton_up_1048_d9fc: candidate for 1048:d9fc (57 bytes).  Verdict: EXACT.
 *
 * Module 1048's copy of the handler-slot setter for offset 8: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_lbutton_up_1048_d9fc(EventTable *table, void *h)
{
    void *old = table->on_lbutton_up;
    table->on_lbutton_up = h;
    return old;
}
