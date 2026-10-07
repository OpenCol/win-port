/* set_on_close_1010_e883: candidate for 1010:e883 (57 bytes).  Verdict: EXACT.
 *
 * Module 1010's copy of the handler-slot setter for offset 48: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_close_1010_e883(EventTable *table, void *h)
{
    void *old = table->on_close;
    table->on_close = h;
    return old;
}
