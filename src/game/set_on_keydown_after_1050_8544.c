/* set_on_keydown_after_1050_8544: candidate for 1050:8544 (57 bytes).  Verdict: EXACT.
 *
 * Module 1050's copy of the handler-slot setter for offset 40: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_keydown_after_1050_8544(EventTable *table, void *h)
{
    void *old = table->on_keydown_after;
    table->on_keydown_after = h;
    return old;
}
