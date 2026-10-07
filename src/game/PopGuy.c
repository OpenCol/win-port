/* PopGuy: candidate for 1058:1d2d (128 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Advances a 16-pixel grid cursor, wrapping to a staggered new row at x >= 0x248 and
 * stopping below y 0x120, then draws sprite `kind` of the third table at the base
 * rectangle's origin plus the cursor.
 *
 * `g_report_rect` IS A RECT, and this file used to say `Pt {short x, y;}`. `ReportWin_Init`
 * assigns it `&o->rect`, so the object is settled from the writing side; `short` and `int` are
 * one type to this compiler, so `Pt.x` and `Pt.y` sat exactly on `RECT.left` and `RECT.top` and
 * the bytes could not tell the two spellings apart. The model reported the pair as its own kind
 * of disagreement -- two NAMED types for one cell, not two views of one -- which is what found
 * it. Reading the members by their real names costs nothing and removes a typedef that existed
 * only here.
 *
 * RENAMED 2026-09-26 from draw_next_icon_in_grid: the Mac build's PopGuy(GRPort&, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between ReportWin_ReportsReligious and ReportWin_Score, which are already bound; the arity agrees and the Mac body is 1.23x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void PopGuy(GRPort *dst, int kind)
{
    char buf[8];

    g_layout_x = g_layout_x + 16;
    if (g_layout_x >= 0x248) {
        g_layout_y += 16;
        g_layout_x = ((g_layout_x >> 1) & 4) + 0x20;
    }
    if (g_layout_y >= 0x120)
        return;
    Sprite_Draw((RECT *)buf, &g_icon_sprites[kind], dst, g_report_rect->left + g_layout_x, g_report_rect->top + g_layout_y);
}
