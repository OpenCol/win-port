/* set_on_close_1040_a7bc: candidate for 1040:a7bc (57 bytes).  Verdict: EXACT.
 *
 * Module 1040's copy of the handler-slot setter for offset 48: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_close_1040_a7bc(EventTable *table, void *h)
{
    void *old = table->on_close;
    table->on_close = h;
    return old;
}
