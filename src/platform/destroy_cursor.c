/* destroy_cursor: candidate for 1078:28b1 (29 bytes).  Verdict: RELOCATABLE.
 *
 * Destroys a cursor handle, ignoring a null one.
 */

#include "colonization.h"

void destroy_cursor(unsigned hcur) { if (hcur != 0) DestroyCursor(hcur); }
