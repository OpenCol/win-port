/* rec_get_pair: candidate for 1068:7b8d (40 bytes).  Verdict: EXACT.
 *
 * Reads the two ints at +0x14 and +0x16 of a Sprite (just past its hotspot) into the caller's two out-parameters.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
void rec_get_pair(Sprite *r, int *a, int *b) { *a = r->scale_x; *b = r->scale_y; }
