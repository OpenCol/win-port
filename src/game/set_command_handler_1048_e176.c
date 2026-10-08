/* PORT: a callback is stored as intptr_t, not long: a pointer is 8 bytes on 64-bit Windows (W9). */
/* set_command_handler_1048_e176: candidate for 1048:e176 (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x80 of a record -- one 32-bit `MOV` through
 * ES and nothing else. One of TWO copies.
 */

#include "colonization.h"

#include "classes.h"

void set_command_handler_1048_e176(EventWin *r, void *h)
{
    r->command_handler = (intptr_t)h;
}
