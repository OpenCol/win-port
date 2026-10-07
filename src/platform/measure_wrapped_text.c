/* PORT: this file's own declarations of MSHLock_1068_93af removed: the header declares it from the definition (W1 step 3). */
/* measure_wrapped_text: candidate for 1078:173f (219 bytes).  Verdict: RELOCATABLE.
 *
 * Measures word-wrapped text: the font record is locked (MSHLock_1068_93af), the text laid out
 * with DrawText(DT_CALCRECT | DT_WORDBREAK) into a rectangle `width` wide on a memory DC,
 * the record unlocked (MSHUnlock_1068_93cc), and the resulting rectangle copied to `out`, which is
 * returned.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"

RECT *measure_wrapped_text(RECT *out, unsigned font, char *s, int width)
{
    unsigned dc0;
    unsigned old;
    RECT rc;
    FontRec *f;
    unsigned dc;

    f = (FontRec *)MSHLock_1068_93af(font);
    dc0 = GetDC(0);
    dc = CreateCompatibleDC(dc0);
    ReleaseDC(0, dc0);
    old = SelectObject(dc, f->hfont);
    SetRect(&rc, 0, 0, width, 0);
    DrawText(dc, s, STRLEN(s), &rc, 0x410);
    SelectObject(dc, old);
    DeleteDC(dc);
    MSHUnlock_1068_93cc(font);
    out->left = rc.left;
    out->top = rc.top;
    out->right = rc.right;
    out->bottom = rc.bottom;
    return out;
}
