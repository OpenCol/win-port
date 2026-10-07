/* say_number_1060_8947: candidate for 1060:8947 (53 bytes).  Verdict: RELOCATABLE.
 *
 * Appends n in decimal to the message buffer (DGROUP:0x4e6c) through the
 * number buffer at 0x566c.
 */

#include "colonization.h"

void say_number_1060_8947(int n)
{
    LTOA1(n, g_number_buf, 10);
    STRCAT(g_message_buf, g_number_buf);
}
