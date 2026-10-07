/* get_max_track_size_1070_9c38: candidate for 1070:9c38 (40 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: get_max_track_size_1070_9c38
 *
 * Returns the two words at +0x72/+0x74 of the record through the pointers
 * (segment 1070's copy).
 *
 * RENAMED 2026-09-26 from get_pos_words_1070_9c38: reads the window's maximum track size, the int pair at +0x72;
 * object_message_handler adds the non-client size and returns it as ptMaxTrackSize on WM_GETMINMAXINFO,
 * and win_clamp_client_size reads it as maxw/maxh. It was named size/pos before the reader was seen.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

void get_max_track_size_1070_9c38(EventWin *obj, int *pa, int *pb)
{
    *pa = obj->max_track_w;
    *pb = obj->max_track_h;
}

