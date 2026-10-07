/* win_set_text: candidate for 1070:656f (28 bytes).  Verdict: RELOCATABLE.
 *
 * `SetWindowText(hwnd, text)` and nothing else. Three pushes for two arguments, because the
 * text is a pointer: the window handle from [bp+6], then the pointer's high half from
 * [bp+0xa] and its low half from [bp+8], which is `` order for a 2-byte handle followed
 * by a 4-byte string.
 */

#include "colonization.h"

#include "w16types.h"

void win_set_text(unsigned hwnd, char *text)
{
    SetWindowText(hwnd, text);
}
