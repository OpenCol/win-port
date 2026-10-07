/* set_on_keydown_1060_867d: candidate for 1060:867d (57 bytes).  Verdict: EXACT.
 *
 * Module 1060's copy of the handler-slot setter for offset 32: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_keydown_1060_867d(EventTable *table, void *h)
{
    void *old = table->on_keydown;
    table->on_keydown = h;
    return old;
}
