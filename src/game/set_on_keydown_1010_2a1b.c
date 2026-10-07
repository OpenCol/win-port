/* set_on_keydown_1010_2a1b: candidate for 1010:2a1b (57 bytes).  Verdict: EXACT.
 *
 * Module 1010's copy of the handler-slot setter for offset 32: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_keydown_1010_2a1b(EventTable *table, void *h)
{
    void *old = table->on_keydown;
    table->on_keydown = h;
    return old;
}
