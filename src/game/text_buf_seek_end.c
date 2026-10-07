/* text_buf_seek_end: candidate for 1040:6f95 (40 bytes).  Verdict: RELOCATABLE.
 *
 * Points the text cursor (DGROUP:0x4ca8) at the terminator of the text
 * buffer at SEG29:0x3c.
 */

#include "colonization.h"

void text_buf_seek_end(void)
{
    g_text_cursor = g_text_buf + STRLEN(g_text_buf);
}
