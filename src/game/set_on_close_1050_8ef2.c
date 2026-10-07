/* set_on_close_1050_8ef2: candidate for 1050:8ef2 (57 bytes).  Verdict: EXACT.
 *
 * Module 1050's copy of the handler-slot setter for offset 48: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_close_1050_8ef2(EventTable *table, void *h)
{
    void *old = table->on_close;
    table->on_close = h;
    return old;
}
