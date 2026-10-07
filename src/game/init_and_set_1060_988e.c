/* init_and_set_1060_988e: candidate for 1060:988e (73 bytes).  Verdict: RELOCATABLE.
 * symbol: init_and_set_1060_988e
 *
 * build_window_1060_983a with eleven words of arguments, then eventwin_set_menu with the
 * trailing pointer.  Both are -called and defined in-file as stubs.
 *
 * The words are not ints: the pair after the object is the window's NAME and the four
 * before the trailing pointer its PALETTE (and, in the eleven-word variants, the four
 * after that its PARENT). See `init_and_set_1060_a1f6` for what settles them (§119).
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;

extern void eventwin_set_menu(void *o, void *p);

void init_and_set_1060_988e(void *o, char *name, int style, int x, int y, int w, int h, void *pal, void *p)
{
    build_window_1060_983a(o, name, style, x, y, w, h, pal);
    eventwin_set_menu((EventWin *)o, p);
}

