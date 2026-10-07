/* get_min_track_size_1060_af51: candidate for 1060:af51 (40 bytes).  Verdict: EXACT.
 *
 * Returns the two words at +0x6e/+0x70 of the record through the pointers.
 *
 * RENAMED 2026-09-26 from get_size_words_1060_af51: reads the window's minimum track size, the int pair at +0x6e;
 * object_message_handler adds the non-client size and returns it as ptMinTrackSize on WM_GETMINMAXINFO,
 * and win_clamp_client_size reads it as minw/minh. It was named size/pos before the reader was seen.
 */

#include "colonization.h"

#include "classes.h"

void get_min_track_size_1060_af51(EventWin *r, int *pa, int *pb)
{
    *pa = r->min_track_w;
    *pb = r->min_track_h;
}
