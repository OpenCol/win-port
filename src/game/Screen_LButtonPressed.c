/* Screen_LButtonPressed: candidate for 1060:a722 (80 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -r-
 * symbol: Screen_LButtonPressed
 *
 * Waits on the LEFT mouse button -- wait_mouse_button with up = 1 -- on the object's own window when the caller asks for it and on a null window (the whole queue) otherwise.
 *
 * RENAMED 2026-09-26 from msg_box_1060_a722: the Mac build's Screen::LButtonPressed(char, char, char), bound by call-sequence alignment (the lone unbound callee of its gap, none against) and by what it does: it hands wait_mouse_button its (own, remove, wait) flags with up = 1, and wait_mouse_button's up = 1 watches for the LEFT button-up (0x202/0xa2). It is not a message box; the old name read the modal wait as one.
 */

#include "colonization.h"

#include "classes.h"
#include "records.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern MSWin * get_win_1060_ab6e(void *o);
extern int wait_mouse_button(MSWin *win, char left, char remove, char wait);
int Screen_LButtonPressed(Screen *o, char own, char remove, char wait)
{
    if (own) {
        return wait_mouse_button((MSWin *)(get_win_1060_ab6e(o)), 1, remove, wait);
    } else {
        return wait_mouse_button(0, 1, remove, wait);
    }
}
