/* get_current_port: candidate for 1068:614a (32 bytes).  Verdict: RELOCATABLE.
 *
 * Returns the pointer at DGROUP:0x2ac2, beside the error context at 0x2ac0.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern GRPort * g_current_port;          /* DGROUP:0x2ac2, declared */

void *get_current_port(void)
{
    return g_current_port;
}
