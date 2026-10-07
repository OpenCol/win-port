/* MSDrawLine: candidate for 1070:aea4 (170 bytes).  Verdict: RELOCATABLE.
 *
 * Draws a one-pixel line on the WinG surface's DC (GrafRec +2) from (x1, y1) to (x2, y2) in the colour of
 * palette entry DGROUP:0x2ada, read back from the WinG DIB colour table.
 */

#include "colonization.h"

#include "records.h"
typedef struct { unsigned char rgbBlue, rgbGreen, rgbRed, rgbReserved; } RGBQUAD;

extern unsigned WinGGetDIBColorTable(unsigned hdc, unsigned start, unsigned n, RGBQUAD *q);

void MSDrawLine(GrafRec *win, int x1, int y1, int x2, int y2)
{
    unsigned pen;
    unsigned old;
    RGBQUAD q;

    WinGGetDIBColorTable(win->hdc, g_line_color_index, 1, &q);
    pen = CreatePen(0, 1, (unsigned)(q.rgbRed | (q.rgbGreen << 8)) | ((unsigned long)q.rgbBlue << 16));
    old = SelectObject(win->hdc, pen);
    MoveTo(win->hdc, x1, y1);
    LineTo(win->hdc, x2, y2);
    SelectObject(win->hdc, old);
    DeleteObject(pen);
}
