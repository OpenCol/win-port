/* PORT: this file's own declarations of PutLabel removed: the header declares it from the definition (W1 step 3). */
/* DrawIconRow: candidate for 1038:4517 (492 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws a row of `m` copies of one unit sprite, the last `marked` of them overlaid with sprite
 * 0x38, and labels the two groups with their counts. `ComputeIconRow` does the spacing:
 * it returns the step between sprites (0 means nothing fits, and nothing is drawn), may move
 * the start x through the pointer, and hands back a compression SHIFT for rows too long for
 * the span. Every count here is shifted by it, so a compressed row draws one sprite per 2^shift
 * units; when both halves were odd before the shift, the plain count is bumped by one.
 *
 * Style bit 2 draws each sprite twice -- a shadow a pixel up through `Sprite_DrawMono`, then the
 * sprite -- and bit 1 spreads the free width through the row: the remainder `ComputeIconRow` left
 * in `out` accumulates per sprite and pays out one pixel each time it passes the count.
 *
 * The labels are drawn only when DGROUP:0x1940 is set, or when the row is packed one pixel
 * apart with more than one unit in it -- the case where the sprites alone cannot be counted.
 * The plain count goes at the row's start in colour 0xff, the marked count at the first
 * marked sprite in colour 0xc, both in the heading font.
 *
 * `y` is a `register` parameter and `acc` a `register` local: the local takes SI and the
 * parameter DI (rule 53). `px`, `out` and `shift` have their addresses taken and so are never
 * register candidates. `ComputeIconRow` and `PutLabel` come through
 * the 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from draw_unit_row: the Mac build's DrawIconRow(GRPort&, short, short, short, short, short, short, short, short, short), bound by call-sequence alignment (2.5 aligned gaps in ColonyWin_DrawABuilding, ColonyWin_DrawColonyMap leave it as the lone unbound callee, none against); the arity agrees.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern RECT *Sprite_DrawMono(RECT *out, Sprite *rec, GRPort *port, int x, int y, unsigned char mode);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

/* same object: 4-byte `push cs; call ` */
extern int ComputeIconRow(int pic, int n, int m, int *px, int y, int span, int span2, int *out, int *shift, int style);

void DrawIconRow(GRPort *port, int pic, int n, int count, int px, register int y,
                                 int span, int span2, int marked, int style)
{
    int i;
    int step;
    int first_marked;
    int out;
    int x0;
    int x_marked;
    int labels;
    int shift;
    int plain;
    int marked0;
    int n_sh;
    int m_sh;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    register int acc;

    x_marked = 0;
    step = ComputeIconRow(pic, n, count, &px, y, span, span2, &out, &shift, style);
    if (step == 0)
        goto done;
    first_marked = count - marked;
    acc = 0;
    marked0 = marked;
    plain = first_marked;
    marked >>= shift;
    first_marked >>= shift;
    n_sh = n >> shift;
    m_sh = count >> shift;
    if (shift != 0 && (marked0 & plain & 1))
        plain++;
    x0 = px;
    for (i = 0; i < m_sh; i++) {
        if (style & 2) {
            Sprite_DrawMono(&r1, &g_icon_sprites[pic], port, px, y, 0);
            Sprite_Draw(&r2, &g_icon_sprites[pic], port, px, y + 1);
        } else {
            Sprite_Draw(&r3, &g_icon_sprites[pic], port, px, y + 1);
        }
        if (i == first_marked)
            x_marked = px;
        if (i >= first_marked)
            Sprite_Draw(&r4, &g_icon_sprites[0x38], port, px, y + 1);
        px += step;
        if (style & 1) {
            acc += out;
            while (acc >= n_sh) {
                acc -= n_sh;
                px++;
            }
        }
    }
    labels = g_draw_labels;
    if (step == 1 && count > 1)
        labels = 1;
    if (labels != 0) {
        PutLabel(port, &g_heading_font, plain, x0 + 2, y, 0xff, 1);
        PutLabel(port, &g_heading_font, marked0, x_marked + 2, y, 0xc, 1);
    }
done:
    ;
}
