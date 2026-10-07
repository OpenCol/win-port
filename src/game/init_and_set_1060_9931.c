/* init_and_set_1060_9931: candidate for 1060:9931 (79 bytes).  Verdict: RELOCATABLE.
 * symbol: init_and_set_1060_9931
 *
 * build_window_1060_98d7 with eleven words of arguments, then eventwin_set_menu on the window
 * with the trailing pointer.  Both are -called and defined in-file as
 * stubs.
 *
 * The words are not ints: the pair after the object is the window's NAME and the four
 * after the five ints its PALETTE and the four after that its PARENT, which sit just
 * before the trailing pointer. See `init_and_set_1060_a1f6` for what settles them (§119).
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;

extern void eventwin_set_menu(void *o, void *p);

void init_and_set_1060_9931(void *o, char *name, int style, int x, int y, int w, int h, void *pal, void *parent, void *p)
{
    build_window_1060_98d7(o, name, style, x, y, w, h, pal, parent);
    eventwin_set_menu((EventWin *)o, p);
}

