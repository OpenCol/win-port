/* win_get_text: candidate for 1070:658b (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `GetWindowText(hwnd, buf, 0xff)`: copies up to 255 characters of a window's text into the
 * caller's buffer. The getter paired with `win_set_text` (1070:656f) immediately before it,
 * which calls SetWindowText.
 */

#include "colonization.h"

void win_get_text(unsigned hwnd, char *buf)
{
    GetWindowText(hwnd, buf, 0xff);
}
