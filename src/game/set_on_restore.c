/* set_on_restore: candidate for 1038:e15d (57 bytes).  Verdict: EXACT.
 *
 * Module 1038's copy of the handler-slot setter for offset 56: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_restore(EventTable *table, void *h)
{
    void *old = table->on_restore;
    table->on_restore = h;
    return old;
}
