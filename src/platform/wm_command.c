/* wm_command: candidate for 1070:692d (141 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * A menu command arriving at a window: the object behind the HWND comes out of
 * `GetWindowLong(hwnd, 4)`, slot 128 of its handler table gets it first, and only if no handler is
 * installed there does the command go on to be looked up in the window's menu and handed to slot
 * 124 as a PAIR of words.
 *
 * The pair is one `long` taken apart. `menu_find_item` returns 32 bits; the shipped code
 * stores them, reads the low half back as a word and the high half with `SHR EAX,16`, and
 * pushes the two separately -- so slot 124's handler takes two `int`s where the finder
 * returned one `long`, and the split is in this function rather than in either of them.
 *
 * The command id is widened to 32 bits for slot 128 (`MOVZX EAX,DI` and one `PUSH EAX`),
 * which is what says that handler's second parameter is a `long` and the id is unsigned.
 *
 * The early-out is on the BYTE the handler returns (`OR AL,AL`), not on a word.
 *
 * `GetWindowLong` and `GetMenu` are ``, so their arguments go left to right.
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` */
extern char call_command_handler(void *table, long command);
extern char call_menu_handler(void *win, int item_lo, int item_hi);

void wm_command(register unsigned hwnd, register unsigned id)
{
    void *obj;
    unsigned menu;
    int lo;
    int hi;
    long item;

    obj = (void *)GetWindowLong(hwnd, 4);
    if (call_command_handler((EventWin *)obj, (unsigned long)id) == 0) {
        g_menu_depth = 0;
        menu = GetMenu(hwnd);
        if (menu != 0) {
            item = menu_find_item(menu, id);
            lo = (int)item;
            hi = (int)((unsigned long)item >> 16);
            call_menu_handler((EventWin *)obj, lo, hi);
        }
    }
}
