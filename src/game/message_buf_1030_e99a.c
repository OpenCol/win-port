/* message_buf_1030_e99a: candidate for 1030:e99a (24 bytes).  Verdict: RELOCATABLE.
 *
 * Returns the address of the message buffer at DGROUP:0x4e6c -- segment
 * 1030's copy of the accessor.
 */

#include "colonization.h"

char *message_buf_1030_e99a(void)
{
    return g_message_buf;
}
