/* error_code_set: candidate for 1068:949e (20 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the graphics layer's error code in the word at DGROUP:0x2b1a. The whole of that
 * variable is these two functions: the image holds exactly two references to 0x2b1a, this
 * store and the load in `error_code_get` at 1068:94b2 -- so the pair IS the variable, and
 * nothing else in the game reads or writes it directly.
 *
 * The values are `include/grerror.h`'s `GRERR_*`: `MSAllocateHandle` stores
 * GRERR_ALLOC_FAILED, `load_pcx_resource` stores GRERR_NOT_FOUND and GRERR_BAD_FORMAT.
 */

#include "colonization.h"

extern int g_gr_error;   /* DGROUP:0x2b1a */

void error_code_set(int code)
{
    g_gr_error = code;
}
