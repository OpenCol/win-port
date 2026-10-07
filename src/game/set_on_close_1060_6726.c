/* set_on_close_1060_6726: candidate for 1060:6726 (57 bytes).  Verdict: EXACT.
 *
 * Module 1060's copy of the handler-slot setter for offset 48: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_close_1060_6726(EventTable *table, void *fn)
{
    void *old = table->on_close;
    table->on_close = fn;
    return old;
}
