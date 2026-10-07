/* draw_ctl_border: candidate for 1070:623d (489 bytes).  Verdict: RELOCATABLE.
 * symbol: draw_ctl_border
 * name-source: descriptive
 *
 * Draws the border of the static-text control painted by `static_text_wndproc`, its only
 * caller, from three bits of the control word that control keeps in its window extra bytes.
 * Bit 0 is a plain black rectangle around the client area; bit 1 is that rectangle plus a
 * SUNKEN bevel one pixel inside it -- grey down the top and left, white up the bottom and
 * right -- and bit 2 is the same rectangle with the RAISED bevel, the two pens exchanged.
 * The tests are an if/else-if chain, so bit 0 wins over bit 1 and bit 1 over bit 2, and a
 * style with none of the three draws nothing at all.
 *
 * THE FLAGS ARE A LONG. The caller is in the same translation unit and pushes the whole
 * `GetWindowLong` result; the tests here only ever look at the low word, because Borland
 * folds `style & 1` on a long into a word test. The shipped bytes are identical with `int`,
 * so the argument size comes from the caller and not from here.
 *
 * All three pens are created before the flags are looked at and all three are deleted after,
 * so the two that are never selected are still made and destroyed on every paint. No
 * SelectObject result is kept, so the DC is left holding one of these pens when the last
 * DeleteObject runs -- deleting a pen that is still selected.
 *
 * The bevel is drawn as one open polyline per pen, and THE SECOND PEN HAS NO MoveTo: it
 * continues from where the first stopped, at the bottom-left corner, along the bottom and
 * back up the right side. That is why the bottom-left and top-right pixels belong to
 * whichever pen was selected second.
 */

#include "colonization.h"

#include "w16types.h"
#include "w16const.h"

void draw_ctl_border(unsigned hwnd, register unsigned hdc, unsigned long style)
{
    unsigned white;
    unsigned grey;
    RECT r;
    unsigned black;

    black = CreatePen(PS_SOLID, 1, 0x000000L);
    white = CreatePen(PS_SOLID, 1, 0xffffffL);
    grey = CreatePen(PS_SOLID, 1, 0x808080L);
    GetClientRect(hwnd, &r);
    if (style & 1) {
        SelectObject(hdc, black);
        MoveTo(hdc, 0, 0);
        LineTo(hdc, r.right, 0);
        LineTo(hdc, r.right, r.bottom);
        LineTo(hdc, 0, r.bottom);
        LineTo(hdc, 0, 0);
    } else if (style & 2) {
        SelectObject(hdc, black);
        MoveTo(hdc, 0, 0);
        LineTo(hdc, r.right, 0);
        LineTo(hdc, r.right, r.bottom);
        LineTo(hdc, 0, r.bottom);
        LineTo(hdc, 0, 0);
        SelectObject(hdc, grey);
        MoveTo(hdc, r.right - 1, 1);
        LineTo(hdc, 1, 1);
        LineTo(hdc, 1, r.bottom - 1);
        SelectObject(hdc, white);
        LineTo(hdc, r.right - 1, r.bottom - 1);
        LineTo(hdc, r.right - 1, 1);
    } else if (style & 4) {
        SelectObject(hdc, black);
        MoveTo(hdc, 0, 0);
        LineTo(hdc, r.right, 0);
        LineTo(hdc, r.right, r.bottom);
        LineTo(hdc, 0, r.bottom);
        LineTo(hdc, 0, 0);
        SelectObject(hdc, white);
        MoveTo(hdc, r.right - 1, 1);
        LineTo(hdc, 1, 1);
        LineTo(hdc, 1, r.bottom - 1);
        SelectObject(hdc, grey);
        LineTo(hdc, r.right - 1, r.bottom - 1);
        LineTo(hdc, r.right - 1, 1);
    }
    DeleteObject(black);
    DeleteObject(white);
    DeleteObject(grey);
}
