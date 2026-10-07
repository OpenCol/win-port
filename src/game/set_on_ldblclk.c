/* set_on_ldblclk: candidate for 1010:e84a (57 bytes).  Verdict: EXACT.
 *
 * Module 1010's copy of the handler-slot setter for offset 28: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_ldblclk(EventTable *table, void *h)
{
    void *old = table->on_ldblclk;
    table->on_ldblclk = h;
    return old;
}
