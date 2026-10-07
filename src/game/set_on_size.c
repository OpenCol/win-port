/* set_on_size: candidate for 1048:d78c (57 bytes).  Verdict: EXACT.
 *
 * Module 1048's copy of the handler-slot setter for offset 64: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_size(EventTable *table, void *h)
{
    void *old = table->on_size;
    table->on_size = h;
    return old;
}
