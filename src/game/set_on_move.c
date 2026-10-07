/* set_on_move: candidate for 1048:d753 (57 bytes).  Verdict: EXACT.
 *
 * Module 1048's copy of the handler-slot setter for offset 68: stores the pointer
 * into the object's slot and returns the previous one.
 */

#include "colonization.h"

#include "classes.h"

void *set_on_move(EventTable *table, void *h)
{
    void *old = table->on_move;
    table->on_move = h;
    return old;
}
