/* set_on_rbutton_down_1048_df05: candidate for 1048:df05 (57 bytes).  Verdict: EXACT.
 *
 * Module 1048's copy of the handler-slot setter for offset 16: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_rbutton_down_1048_df05(EventTable *table, void *h)
{
    void *old = table->on_rbutton_down;
    table->on_rbutton_down = h;
    return old;
}
