/* get_min_track_size_1070_9c10: candidate for 1070:9c10 (40 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_min_track_size_1070_9c10
 *
 * Returns the two words at +0x6e/+0x70 of the record through the pointers
 * (segment 1070's copy).
 *
 * RENAMED 2026-09-26 from get_size_words_1070_9c10: reads the window's minimum track size, the int pair at +0x6e;
 * object_message_handler adds the non-client size and returns it as ptMinTrackSize on WM_GETMINMAXINFO,
 * and win_clamp_client_size reads it as minw/minh. It was named size/pos before the reader was seen.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void get_min_track_size_1070_9c10(EventWin *obj, int *pa, int *pb)
{
    *pa = obj->min_track_w;
    *pb = obj->min_track_h;
}

