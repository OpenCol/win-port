/* swap_current_port: candidate for 1068:610c (62 bytes).  Verdict: RELOCATABLE.
 *
 * Installs a new pointer at DGROUP:0x2ac2 and returns the old one -- the
 * setter half of `get_current_port`, next door to it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern GRPort * g_current_port;          /* DGROUP:0x2ac2, declared */

void *swap_current_port(void *p)
{
    void *old = g_current_port;

    g_current_port = (GRPort *)p;
    return old;
}
