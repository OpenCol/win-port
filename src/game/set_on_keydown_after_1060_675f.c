/* set_on_keydown_after_1060_675f: candidate for 1060:675f (57 bytes).  Verdict: EXACT.
 *
 * Module 1060's copy of the handler-slot setter for offset 40: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_keydown_after_1060_675f(EventTable *table, void *fn)
{
    void *old = table->on_keydown_after;
    table->on_keydown_after = fn;
    return old;
}
