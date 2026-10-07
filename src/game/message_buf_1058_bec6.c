/* message_buf_1058_bec6: candidate for 1058:bec6 (24 bytes).  Verdict: RELOCATABLE.
 *
 * Returns the address of the message buffer at DGROUP:0x4e6c -- segment
 * 1058's copy of the accessor.
 */

#include "colonization.h"

char *message_buf_1058_bec6(void)
{
    return g_message_buf;
}
