/* flag_4bee_set: candidate for 1020:9a3d (20 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Stores 1 into the word at DGROUP:0x4bee, and does nothing else.
 *
 * NOTHING READS IT. This is the only instruction in the image that touches 0x4bee by
 * absolute address, and tools/check_dead_dgroup.py classifies the cell as written and never
 * read. That scan cannot see an array element or a block copy that happens to cover the
 * address, so "dead" is its verdict rather than a proof -- but no reader names the flag,
 * and the name says only which word it sets.
 *
 * The word immediately before it, DGROUP:0x4bec, is the same kind of cell: an `int` written
 * by its own word stores in this segment (refresh_port_30_* and PopupBoxDraw) and
 * read by nothing. They are two adjacent flags, not one array -- no instruction indexes
 * either -- and this global is named the way that neighbour is.
 */

#include "colonization.h"

extern int g_4bee;   /* DGROUP:0x4bee */

void flag_4bee_set(void)
{
    g_4bee = 1;
}
