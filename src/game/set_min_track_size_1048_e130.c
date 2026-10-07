/* set_min_track_size_1048_e130: candidate for 1048:e130 (34 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores two `int`s at +0x6e and +0x70 of the record -- two consecutive words set together. A one-line accessor: 20-odd bytes of prologue and epilogue around the move.
 *
 * RENAMED 2026-09-26 from set_pair_6e_1048_e130: stores the window's minimum track size, the int pair at +0x6e;
 * object_message_handler adds the non-client size and returns it as ptMinTrackSize on WM_GETMINMAXINFO,
 * and win_clamp_client_size reads it as minw/minh. MainWin_Init sets 500x400.
 */

#include "colonization.h"

#include "classes.h"

void set_min_track_size_1048_e130(EventWin *r, int min_track_w, int min_track_h)
{
    r->min_track_w = min_track_w;
    r->min_track_h = min_track_h;
}
