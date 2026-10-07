/* set_on_mousemove_1038_e126: candidate for 1038:e126 (55 bytes).  Verdict: EXACT.
 *
 * Module 1038's copy of the handler-slot setter for offset 0: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_mousemove_1038_e126(EventTable *table, void *h)
{
    void *old = table->on_mousemove;
    table->on_mousemove = h;
    return old;
}
