/* DrawMultiIconRow: candidate for 1038:4084 (1061 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * DRAWS ONE ROW OF THE COLONY WINDOW'S PRODUCTION PANEL: the list `ColonyWin_DrawMakeList`
 * builds in the three nine-word tables at SEG26 (kind, count, part) with `ResetMultiIcon` and
 * `AddMultiIcon`. That second name is a misreading: it appends a row entry and undoes nothing.
 * It also draws for 1010:962d and 1058:7056.
 *
 * Each entry is a goods icon (kind & 0xfff indexes the art at SEG20:0x6246) repeated `count` times,
 * followed by the next entry `gap` pixels on.
 *
 * FITTING THE ROW INTO `avail` PIXELS. Every stack of more than one icon needs room for its extra
 * icons. While the space left after the icons and gaps is less than one pixel per stack, the gaps
 * shrink first, then every icon is drawn that many pixels narrower (`shrink`), and at the last
 * resort the space is simply taken as enough. The extra icons are then spaced `space / extra`
 * apart, but no wider than an icon plus one. When even one pixel each will not fit, the stack is
 * thinned: only every 2^shift-th icon is drawn.
 *
 * THE FLAGS in the kind's high bits. Normally the last `part` icons of a stack carry the overlay
 * at SEG20:0x6946, and the two labels count the plain icons and the `part`. Then:
 *   - 0x8000 marks a shortfall: every icon carries the overlay, and the first label is colour 0xc;
 *   - 0x4000 draws the first `count - part` icons as the sprite at SEG20:0x6986 instead of the
 *     goods. No icon carries the overlay, the whole count goes in the first label, and the
 *     second label reads 0 in colour 0xff.
 * The counts are labelled when DGROUP:0x1940 is set or the stack holds 12 or more. Label one is
 * at the stack's start; label two is where the `part` begins.
 *
 * The second label's colour is written `!(kind & 0x4000) ? 0xc : 0xff`. The plain spelling of that
 * ternary compiles with the branch the other way round, and that was the only DIFFERS.
 *
 * RENAMED 2026-09-26 from draw_goods_row: the Mac build's DrawMultiIconRow(GRPort&, short, short, short, short), bound by layout (between ComputeIconRow and DrawIconRow on both sides); the arity agrees and the Mac body is 0.97x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

#include "icons.h"
/* g_sprite_6986 (SEG20:0x6986) is g_icon_sprites[0x3a] */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *rec, GRPort *port, int x, int y, unsigned char flag);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int sprite_width_1038_86f0(void *spr);

extern void PutLabel(void *w, void *font, int n, int x, int y, int colour, int fill);

void DrawMultiIconRow(GRPort *port, int x, int y, int avail, int gap)
{
    int space;
    int total;
    int widths;
    int slack;
    int shrink;
    int groups;
    int extra;
    int step;
    int shift;
    int icon;
    int dx;
    int first;
    int n1;
    int n2;
    int stack_x;
    int xsplit;
    int show;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    RECT r5;
    RECT r6;
    register int i;

    step = 0;
    shift = 0;
    total = 0;
    groups = 0;
    extra = 0;
    for (i = 0; i < g_row_count; i++) {
        total += g_row.count[i];
        if (g_row.count[i] > 1) {
            groups++;
            extra += g_row.count[i] - 1;
        }
    }
    widths = 0;
    for (i = 0; i < g_row_count; i++)
        widths += sprite_width_1038_86f0(&g_icon_sprites[g_row.kind[i] & 0xfff]);
    shrink = 0;
    do {
        slack = widths - shrink * g_row_count;
        slack = max_int_1038_86a6(0, slack);
        space = avail;
        space -= slack;
        space -= (g_row_count - 1) * gap;
        if (space < groups) {
            if (gap > 0)
                gap--;
            else if (slack > 0)
                shrink++;
            else
                space = groups;
        }
    } while (space < groups);
    if (extra != 0) {
        step = space / extra;
        if (step == 0) {
            do
                shift++;
            while (extra >> shift > space);
        }
    }
    for (i = 0; i < g_row_count; i++) {
        if (step != 0) {
            dx = sprite_width_1038_86f0(&g_icon_sprites[g_row.kind[i] & 0xfff]) + 1;
            dx = min_int_1038_8680(dx, step);
        } else
            dx = 1;
        n1 = g_row.count[i] - g_row.part[i];
        first = n1 >> shift;
        stack_x = x;
        xsplit = -1;
        for (icon = 0; g_row.count[i] >> shift > icon; icon++) {
            if (icon == first)
                xsplit = x;
            if ((g_row.kind[i] & 0x4000) && icon < first) {
                Sprite_DrawMono(&r1, &g_icon_sprites[0x3a], port, x - 1, y, 0);
                Sprite_Draw(&r2, &g_icon_sprites[0x3a], port, x, y);
            } else {
                Sprite_DrawMono(&r3, &g_icon_sprites[g_row.kind[i] & 0xfff], port, x - 1, y, 0);
                Sprite_Draw(&r4, &g_icon_sprites[g_row.kind[i] & 0xfff], port, x, y);
                if ((g_row.kind[i] & 0x8000) || (icon >= first && !(g_row.kind[i] & 0x4000))) {
                    Sprite_DrawMono(&r5, &g_icon_sprites[ICON_BOYCOTT], port, x - 1, y, 0);
                    Sprite_Draw(&r6, &g_icon_sprites[ICON_BOYCOTT], port, x, y);
                }
            }
            if ((g_row.count[i] >> shift) - 1 > icon)
                x += dx;
        }
        show = g_draw_labels;
        if (g_row.count[i] >= 12)
            show = 1;
        if (show != 0) {
            n2 = g_row.part[i];
            if (g_row.kind[i] & 0x4000) {
                n1 += n2;
                n2 = 0;
            }
            PutLabel(port, &g_heading_font, n1, stack_x + 2, y,
                                        (g_row.kind[i] & 0x8000) ? 0xc : 0xff, 1);
            PutLabel(port, &g_heading_font, n2, xsplit + 2, y,
                                        !(g_row.kind[i] & 0x4000) ? 0xc : 0xff, 1);
        }
        x += sprite_width_1038_86f0(&g_icon_sprites[g_row.kind[i] & 0xfff]) - shrink + gap;
    }
}
