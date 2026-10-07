/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of MSGetTextLength_1038_8713, font_height_1038_8739 removed: the header declares them from the definition (W1 step 3). */
/* PutLabel: candidate for 1038:3e9d (255 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: PutLabel
 *
 * Draws a positive number at (x, y + 2) in the given font: optionally a
 * black box behind it, then the digits twice (x + 1 and x + 2) in `color`,
 * vertically centred on the font height.  The two font measures are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from draw_number_label: the Mac build's PutLabel(GRPort&, Font&, short, short, short, short, short), bound by call-sequence alignment (1.0 aligned gaps in DrawIconRow leave it as the lone unbound callee, none against) and by layout (between ComputeIconRow and DrawIconRow on both sides); the arity agrees and the Mac body is 1.07x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;

extern void GRPort_FillColor(GRPort *p, RECT *r, int color);



void PutLabel(GRPort *port, Font *font, int n, int x, int y, int color, int fill)
{
    register int px;
    register int py;
    int tw;
    int th;
    RECT r;
    char buf[20];

    px = x;
    py = y;
    if (n <= 0)
        goto done;
    py += 2;
    buf[0] = 0;
    LTOA1(n, buf, 10);
    tw = MSGetTextLength_1038_8713(font, buf);
    th = font_height_1038_8739(font);
    if (fill != 0) {
        SetRect(&r, px + 1, py + 1, px + tw + 1, py + th - 1);
        GRPort_FillColor(port, &r, 0);
    }
    TextColor(color);
    GRPort_String(port, (unsigned *)font, buf, px + 1, th / 2 + py, 1);
    GRPort_String(port, (unsigned *)font, buf, px + 2, th / 2 + py, 1);
done:
    ;
}
