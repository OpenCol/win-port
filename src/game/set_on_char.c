/* set_on_char: candidate for 1048:df77 (57 bytes).  Verdict: EXACT.
 *
 * Module 1048's copy of the handler-slot setter for offset 44: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_char(EventTable *table, void *h)
{
    void *old = table->on_char;
    table->on_char = h;
    return old;
}
