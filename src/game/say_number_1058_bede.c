/* say_number_1058_bede: candidate for 1058:bede (53 bytes).  Verdict: RELOCATABLE.
 *
 * Appends n in decimal to the message buffer (DGROUP:0x4e6c) through the
 * number buffer at 0x566c.
 *
 * TRANSFERRED: byte-identical to `say_number_1060_8947` (1060:8947), one of 2 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

void say_number_1058_bede(int n)
{
    LTOA1(n, g_number_buf, 10);
    STRCAT(g_message_buf, g_number_buf);
}
