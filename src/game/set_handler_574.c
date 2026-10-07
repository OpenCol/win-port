/* set_handler_574: candidate for 1010:2b0f (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a function pointer in the object's slot at +0x574. Its caller passes
 * `DoFireworks` against `w + 0x100`, so the slot holds a callback.
 */

#include "colonization.h"

#include "classes.h"

void set_handler_574(Flic *flic, void *h)
{
    flic->loop_hook = h;
}
