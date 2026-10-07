/* set_command_handler_1038_e1ba: candidate for 1038:e1ba (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x80 of a record -- one 32-bit `MOV` through
 * ES and nothing else. One of TWO copies.
 */

#include "colonization.h"

#include "classes.h"

void set_command_handler_1038_e1ba(EventWin *r, void *h)
{
    r->command_handler = (long)h;
}
