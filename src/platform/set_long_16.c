/* set_long_16: candidate for 1078:1f5b (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a 32-bit value at +0x16 of the record, one of THREE identical setters back to back
 * at 1078:1f27, 1f41 and 1f5b writing +0xe, +0x12 and +0x16 -- three consecutive `long` slots
 * four bytes apart, which is what says they are one array or one trio of pointers rather than
 * three unrelated fields.
 *
 * One 32-bit `MOV` under `-3`.
 */

#include "colonization.h"

#include "records.h"

void set_long_16(MSWin *win, long v)
{
    win->icon = (void *)v;
}
