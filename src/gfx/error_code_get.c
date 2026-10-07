/* error_code_get: candidate for 1068:94b2 (17 bytes).  Verdict: RELOCATABLE.
 *
 * Returns the graphics layer's error code from DGROUP:0x2b1a, the word `error_code_set`
 * writes. Three bytes shorter than its partner because it has no argument to fetch: the
 * -ml -WS prologue, one `MOV AX,[0x2b1a]`, the epilogue, and the return value is simply
 * whatever the load left in AX.
 *
 * NO CALLER IN THE CENSUS. It is reconstructed because its partner is and because the two
 * together are the entire life of that variable; a getter nothing calls is still the other
 * half of the pair, and leaving it out would leave the corpus asserting a variable with a
 * writer and no reader.
 */

#include "colonization.h"

extern int g_gr_error;   /* DGROUP:0x2b1a */

int error_code_get(void)
{
    return g_gr_error;
}
