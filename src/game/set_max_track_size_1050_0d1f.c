/* set_max_track_size_1050_0d1f: candidate for 1050:0d1f (34 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores two `int`s at +0x72 and +0x74 of the record -- two consecutive words set together. A one-line accessor: 20-odd bytes of prologue and epilogue around the move.
 *
 * RENAMED 2026-09-26 from set_pair_72_1050_0d1f: stores the window's maximum track size, the int pair at +0x72;
 * object_message_handler adds the non-client size and returns it as ptMaxTrackSize on WM_GETMINMAXINFO,
 * and win_clamp_client_size reads it as maxw/maxh.
 */

#include "colonization.h"

#include "classes.h"

void set_max_track_size_1050_0d1f(EventWin *r, int max_track_w, int max_track_h)
{
    r->max_track_w = max_track_w;
    r->max_track_h = max_track_h;
}
