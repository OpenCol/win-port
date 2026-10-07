/* PORT: this file's own declarations of ClearEventTable_1068_e157, build_movie_window_1060_a5ef, get_win_1068_d72a, win_set_object removed: the header declares them from the definition (W1 step 3). */
/* win_build_and_attach_1068_d4fb: candidate for 1068:d4fb (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_build_and_attach_1068_d4fb
 *
 * Builds the window by forwarding all eight of its arguments to `build_movie_window_1060_a5ef`
 * (a call into segment 1060),
 * attaches the result to the object, then initialises the part at +0xd. One of a pair
 * differing only in how many arguments they forward.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
void win_build_and_attach_1068_d4fb(EventWin *o, char *title, int style, int x,
                                        int y, int w, int h, void *owner)
{
    build_movie_window_1060_a5ef(o, title, style, x, y, w, h, owner);
    win_set_object(o, (MSWin *)(get_win_1068_d72a(o)));
    ClearEventTable_1068_e157(&(EventTable &)*o);
}
