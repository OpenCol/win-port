/* set_on_keydown_after_1058_bc70: candidate for 1058:bc70 (57 bytes).  Verdict: EXACT.
 *
 * Module 1058's copy of the handler-slot setter for offset 40: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_keydown_after_1058_bc70(EventTable *table, void *h)
{
    void *old = table->on_keydown_after;
    table->on_keydown_after = h;
    return old;
}
