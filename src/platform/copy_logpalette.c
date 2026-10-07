/* copy_logpalette: candidate for 1070:04c5 (37 bytes).  Verdict: RELOCATABLE.
 *
 * hmemcpy of 0x404 bytes from the first pointer to the second.
 */

#include "colonization.h"

void copy_logpalette(void *src, void *dst)
{
    hmemcpy(dst, src, 0x404L);
}
