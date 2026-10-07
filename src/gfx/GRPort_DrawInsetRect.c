/* GRPort_DrawInsetRect: candidate for 1068:5fd5 (311 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_DrawInsetRect
 *
 * An inset bevel: the outline in colour 0xef, the inner top and left edges in 0xf7 (dark),
 * the inner right and bottom edges in 0xff (light); the line colour is restored afterwards.
 * GRPort_DrawRect is defined here because the shipped call to it is the compiler's own
 * `push cs / call ` (it precedes this function in the translation unit); the call to
 * TextColor, defined later in the module, is a call TLINK converted to its
 * NOP-padded form.
 */

#include "colonization.h"

#include "w16types.h"

#include "classes.h"
#include "records.h"
extern void MSDrawLine(GrafRec *win, int x1, int y1, int x2, int y2);

extern "C" void GRPort_DrawRect(GRPort *p, RECT *r);

void GRPort_DrawInsetRect(GRPort *p, RECT *r)
{
    char saved = g_line_color_index;

    TextColor(0xef);
    GRPort_DrawRect(p, r);
    TextColor(0xf7);
    MSDrawLine((GrafRec *)p->win, r->left + 1, r->top + 1, r->right - 1, r->top + 1);
    MSDrawLine((GrafRec *)p->win, r->left + 1, r->bottom - 1, r->left + 1, r->top + 1);
    TextColor(0xff);
    MSDrawLine((GrafRec *)p->win, r->right - 1, r->top + 1, r->right - 1, r->bottom - 1);
    MSDrawLine((GrafRec *)p->win, r->right - 1, r->bottom - 1, r->left + 1, r->bottom - 1);
    TextColor(saved);
}
