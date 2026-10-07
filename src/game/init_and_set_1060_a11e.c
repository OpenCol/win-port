/* init_and_set_1060_a11e: candidate for 1060:a11e (77 bytes).  Verdict: RELOCATABLE.
 * symbol: init_and_set_1060_a11e
 *
 * build_window_with_port with nine words of arguments, then eventwin_set_menu on the DialogWin part (+0x26)
 * with the trailing pointer.  Both are -called and defined in-file as
 * stubs.
 *
 * The words are not ints: the pair after the object is the window's NAME and the four
 * before the trailing pointer its PALETTE (and, in the eleven-word variants, the four
 * after that its PARENT). See `init_and_set_1060_a1f6` for what settles them (§119).
 *
 * THE TRAILING POINTER IS CONTESTED. `ClosingWin_ctor` declares it `EventWin *parent`
 * and passes its own window, which is why it is spelled `parent` here. But all three
 * `init_and_set` copies hand it to the same `eventwin_set_menu` (1060:ab8b), whose body is
 * `win_set_menu(get_win_1060_ab6e(o), get_w_0(p))`, and `win_set_menu` (1078:27d5) ends in
 * `SetMenu(hwnd, hmenu)`. `MainWin_Init` passes `&o->menu` there, its MenuClass member whose first word is the hmenu,
 * which fits exactly.
 * Both are the same four bytes and neither reading is byte-visible. Unresolved.
 */

#include "colonization.h"

#include "classes.h"
extern int g_stub_a;

extern void build_window_with_port(void *o, char *name, int style, int x, int y, int w, int h, void *pal);
extern void eventwin_set_menu(void *o, void *parent);

void init_and_set_1060_a11e(char *o, char *name, int style, int x, int y, int w, int h, void *pal, void *parent)
{
    build_window_with_port((GameWin *)o, name, style, x, y, w, h, pal);
    eventwin_set_menu((EventWin *)(o + 0x26), parent);
}

