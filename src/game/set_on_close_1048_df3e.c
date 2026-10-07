/* set_on_close_1048_df3e: candidate for 1048:df3e (57 bytes).  Verdict: EXACT.
 *
 * Module 1048's copy of the handler-slot setter for offset 48: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_close_1048_df3e(EventTable *table, void *h)
{
    void *old = table->on_close;
    table->on_close = h;
    return old;
}
