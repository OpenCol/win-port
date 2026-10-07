/* get_max_track_size_1060_af79: candidate for 1060:af79 (40 bytes).  Verdict: EXACT.
 *
 * Returns the two words at +0x72/+0x74 of the record through the pointers.
 *
 * RENAMED 2026-09-26 from get_pos_words_1060_af79: reads the window's maximum track size, the int pair at +0x72;
 * object_message_handler adds the non-client size and returns it as ptMaxTrackSize on WM_GETMINMAXINFO,
 * and win_clamp_client_size reads it as maxw/maxh. It was named size/pos before the reader was seen.
 */

#include "colonization.h"

#include "classes.h"

void get_max_track_size_1060_af79(EventWin *r, int *pa, int *pb)
{
    *pa = r->max_track_w;
    *pb = r->max_track_h;
}
