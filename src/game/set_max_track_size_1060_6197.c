/* set_max_track_size_1060_6197: candidate for 1060:6197 (34 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores two `int`s at +0x72 and +0x74 of the record. Its twin `set_min_track_size_1060_6175` writes the PREVIOUS
 * pair, +0x6e and +0x70, and is the preceding function: four consecutive words, set two at
 * a time, which is what says they are two pairs and not four fields.
 *
 * RENAMED 2026-09-26 from set_pair_72_1060_6197: stores the window's maximum track size, the int pair at +0x72;
 * object_message_handler adds the non-client size and returns it as ptMaxTrackSize on WM_GETMINMAXINFO,
 * and win_clamp_client_size reads it as maxw/maxh.
 */

#include "colonization.h"

#include "classes.h"

void set_max_track_size_1060_6197(EventWin *r, int max_track_w, int max_track_h)
{
    r->max_track_w = max_track_w;
    r->max_track_h = max_track_h;
}
