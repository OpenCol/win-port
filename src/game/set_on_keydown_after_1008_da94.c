/* set_on_keydown_after_1008_da94: candidate for 1008:da94 (57 bytes).  Verdict: EXACT.
 *
 * Module 1008's copy of the handler-slot setter for offset 40: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_keydown_after_1008_da94(EventTable *table, void *h)
{
    void *old = table->on_keydown_after;
    table->on_keydown_after = h;
    return old;
}
