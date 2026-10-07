/* win_build_and_attach_1068_d49c: candidate for 1068:d49c (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: win_build_and_attach_1068_d49c
 *
 * Builds the window by forwarding all seven of its arguments to this module's builder,
 * attaches the result to the object, then initialises the part at +0xd. One of a pair
 * differing only in how many arguments they forward.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"
extern void build_movie_window_1060_a56b(EventWin *o, char *title, int c, int d, int e, int f, int g);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void * get_win_1068_d72a(EventWin *o);
extern void win_set_object(EventWin *o, void *win);
extern "C" void ClearEventTable_1068_e157(void *table);
void win_build_and_attach_1068_d49c(EventWin *o, char *title, int style, int x, int y, int w, int h)
{
    build_movie_window_1060_a56b(o, title, style, x, y, w, h);
    win_set_object(o, (MSWin *)(get_win_1068_d72a(o)));
    ClearEventTable_1068_e157(&(EventTable &)*o);
}
