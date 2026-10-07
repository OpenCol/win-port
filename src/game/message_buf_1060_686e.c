/* message_buf_1060_686e: candidate for 1060:686e (24 bytes).  Verdict: RELOCATABLE.
 *
 * Returns the address of the message buffer at DGROUP:0x4e6c -- segment
 * 1060's copy of the accessor.
 */

#include "colonization.h"

char *message_buf_1060_686e(void)
{
    return g_message_buf;
}
