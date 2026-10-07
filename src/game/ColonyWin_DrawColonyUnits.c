/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* ColonyWin_DrawColonyUnits: candidate for 1010:7b7e (861 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawColonyUnits
 * name-source: sibling-build
 *
 * Draws the colony window's unit strip and the line of text above it. The text is either the
 * name of what the colony is building -- when DGROUP:0xa08 is set -- or a fixed string from the
 * table; the strip is every unit standing on the colony's square, walked through the tile chain.
 *
 * THREE ROWS, AND THE FIRST IS NOT LIKE THE OTHER TWO. Row 0 holds five units at 0x22 apart,
 * drawn through `PieceDraw` at full size with a 0x20 box; rows 1 and 2 hold seventeen each
 * at 0xa apart, drawn as 3/8-scale PORTRAITS through `Sprite_Draw` with a 10 by 10 box (the 0xf and 0x11 put in `bw`/`bh` are read only by the row-0 box). Every
 * one of those seven numbers is re-assigned when the first row fills, which is why they are
 * variables and not constants.
 *
 * A fourth row would run off the panel, so the walk stops at row 3 -- units past that are not
 * drawn and not counted.
 *
 * `ten = 10` IS TESTED FOR ZERO immediately after being set, and the branch is dead. It is in
 * the shipped code, and it costs six bytes.
 *
 * The selection box is a plain `if/else` and not a ternary: the shipped stores the row's Y
 * straight into DI in both arms, where `y = row == 1 ? 0x13f : 0x12e;` computes into AX first
 * and moves. Same for the two `SetRect` shapes, which differ in both size and offset.
 *
 * `x` and `y` are the two `register` locals; the other twelve live in the frame, and `buf` is
 * 0x50 bytes of it.
 *
 * RENAMED 2026-09-26 from colony_draw_units: the Mac build's ColonyWin::DrawColonyUnits(void): this only, painting every unit standing on the colony's square.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Font, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

extern void GRPort_ResetClip(GRPort *port);
extern void set_colony(int colony);
extern char *get_production_name(int item);

extern char *strings(int n);
extern int top_unit_in_square(int x, int y);
extern void SetGlobalSpriteScale(int num, int den);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y, int span, int mode);
extern "C" int UnitIcon(int u);   /* PORT: C linkage, as defined (W2) */
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern int next_unit(int u);

void ColonyWin_DrawColonyUnits(ColonyWin *w)
{
    int u;
    int first;
    int x0;
    int col;
    int row;
    int n;
    int ten;
    int dy;
    int bw;
    int bh;
    int per_row;
    int dx;
    char *name;
    RECT box;
    RECT sr;
    char buf[0x50];
    register int x;
    register int y;

    GRPort_ResetClip((GRPort *)w);
    set_colony(w->colony);
    if (g_scout_visit != 0) {
        buf[0] = 0;
        name = get_production_name((int)g_colonies[w->colony].item);
        if (name != 0)
            STRCAT(buf, name);
        TextColor(0x39);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0x1fb, w->rect.top + 0x118, 0);
    } else {
        TextColor(0x39);
        GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, strings(g_cmisc_word[1]),
                             w->rect.left + 0x1fb, w->rect.top + 0x118, 0);
    }
    first = top_unit_in_square(g_colonies[w->colony].x, g_colonies[w->colony].y);
    x0 = 0x1aa;
    y = 0x15b;
    SetGlobalSpriteScale(1, 1);
    x = x0;
    col = 0;
    row = 0;
    n = 0;
    dx = 0x22;
    per_row = 5;
    bw = 0x20;
    bh = 0x20;
    dy = 0;
    u = first;
    while (u >= 0) {
        if (g_unit_types[g_units[u].type].holds > 0)
            goto next;
        if (row == 0) {
            PieceDraw((GRPort *)w, u, 0, w->rect.left + x + 0x12, w->rect.top + y, 0x20, 0x64);
        } else {
            SetGlobalSpriteScale(3, 8);
            Sprite_Draw(&sr, &g_icon_sprites[UnitIcon(u)], (GRPort *)w,
                        w->rect.left + x, w->rect.top + y);
            SetGlobalSpriteScale(1, 1);
        }
        if (g_tile_sel == n) {
            ten = 10;
            if (ten != 0 && g_scout_visit == 0) {
                TextColor(0xfa);
                if (row == 0)
                    SetRect(&box, x - 1, y - 1 - dy, x + bw, y + bh);
                else
                    SetRect(&box, x - 5, y, x + 5, y + 10);
                OffsetRect(&box, w->rect.left, w->rect.top);
                GRPort_DrawRect((GRPort *)w, &box);
            }
        }
        x += dx;
        col++;
        if (col >= per_row) {
            col = 0;
            row++;
            if (row >= 3)
                break;
            if (row == 1)
                y = 0x13f;
            else
                y = 0x12e;
            x = 0x1aa;
            per_row = 0x11;
            dx = 0xa;
            bw = 0xf;
            bh = 0x11;
            dy = 1;
        }
        n++;
next:
        u = next_unit(u);
    }
    SetGlobalSpriteScale(1, 1);
}
