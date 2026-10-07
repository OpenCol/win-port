/* set_on_keydown_1040_abfe: candidate for 1040:abfe (57 bytes).  Verdict: EXACT.
 *
 * Module 1040's copy of the handler-slot setter for offset 32: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_keydown_1040_abfe(EventTable *table, void *h)
{
    void *old = table->on_keydown;
    table->on_keydown = h;
    return old;
}
