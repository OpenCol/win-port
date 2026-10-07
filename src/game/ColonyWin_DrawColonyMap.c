/* PORT: this file's own declarations of copy_rect_to_screen_1010_e6a4 removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of DrawIconRow, PieceIconDraw removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_DrawColonyMap: candidate for 1010:82cc (1331 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawColonyMap
 * name-source: sibling-build
 *
 * Draws panel 1 of the colony window, the 3x3 of work squares that `ColonyWin_ColonyMouseMap`
 * handles clicks on: the 192x192 terrain picture from the window's own buffer at +0x7ce, a
 * three-pixel black frame, and then, square by square in 64-pixel cells, what stands on it. Each
 * square's flags come from the 5x5 grid at SEG21:0x14a:
 *
 *   - 0x40: the square is outlined in colour 0xf9;
 *   - no flags at all, and a tribe's claim in SEG21:0x213: the claim marker sprite (SEG20:0x6fe6);
 *   - 0x80: the first unit on the map square whose type byte +6 is above 1, drawn at the right;
 *   - 0x08: the colony's own square -- two rows of goods icons, the first row picture 0x17 with
 *     DGROUP:0x4ba4 of them and a second, when DGROUP:0x4ba6 is not negative, of goods 0x4ba6 with
 *     0x4ba7 of them. They read as the centre square's two automatic crops.
 *
 * Then the square's own production (`compute_yield`): a row of that many goods icons, with
 * fish (picture 0x3a) for a colonist whose job is 8, packed tighter above two; a worked square
 * producing nothing shows the goods icon in a frame (SEG20:0x6a66). A worked square gets its
 * colonist's portrait, and the selected colonist's square is outlined in the colour at
 * DGROUP:0x337f -- unless DGROUP:0xa08 is set, which skips the outline with a `continue`.
 * DGROUP:0x1940 holds DGROUP:0xc0e while the squares are drawn and is cleared after. With
 * `redraw`, the panel's rectangle is copied to the screen.
 *
 * Two `(int)` casts on signed bytes tested against zero (rule 51): the claim and DGROUP:0x4ba6.
 * The unit search is a `for` with an empty body.
 *
 * RENAMED 2026-09-26 from colony_draw_work_area: the Mac build's ColonyWin::DrawColonyMap(short): this plus a redraw flag, painting the 3x3 work squares of the colony's land.
 */

#include "colonization.h"

#include "w16types.h"
#include "jobs.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









#include "records.h"

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"

extern void set_colony(int colony);

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y, int span, int mode);
extern int compute_yield(int col, int row, int *goods, int flag);
extern char get_field(int col, int row);
extern "C" int get_job(int colonist);   /* PORT: C linkage, as defined (W2) */
extern int get_icon(int colonist);
/* same object: 4-byte `push cs; call ` (rule 47) */

void ColonyWin_DrawColonyMap(ColonyWin *w, int redraw)
{
    int col;
    int row;
    int tx;
    int ty;
    int amount;
    int goods;
    int mark;
    int colonist;
    int job;
    int icon;
    int unit;
    RECT r;
    RECT src;
    RECT s1;
    RECT s2;
    RECT s3;
    register int x;
    register int y;

    set_colony(w->colony);
    SetRect(&src, 0, 0, 0xc0, 0xc0);
    SetRect(&r, 0x1a8, 0x24, 0x268, 0xe4);
    OffsetRect(&r, w->rect.left, w->rect.top);
    GRPort_CopyToPort((GRPort *)(&w->terrain), (GRPort *)w, &src, &r);
    x = w->rect.left + 0x1a8;
    y = w->rect.top + 0x24;
    TextColor(0);
    SetRect(&r, x, y, x + 0xc0, y + 0xc0);
    GRPort_DrawRect((GRPort *)w, &r);
    SetRect(&r, x - 1, y - 1, x + 0xc1, y + 0xc1);
    GRPort_DrawRect((GRPort *)w, &r);
    SetRect(&r, x - 2, y - 2, x + 0xc2, y + 0xc2);
    GRPort_DrawRect((GRPort *)w, &r);
    g_draw_labels = g_show_icon_counts;
    for (row = 1; row < 4; row++) {
        for (col = 1; col < 4; col++) {
            x = (col << 6) + w->rect.left + 0x168;
            y = (row << 6) + w->rect.top - 0x1c;
            mark = (unsigned char)g_grid_marks[col][row];
            if (mark & 0x40) {
                TextColor(0xf9);
                SetRect(&r, x, y, x + 0x40, y + 0x40);
                GRPort_DrawRect((GRPort *)w, &r);
            }
            if (mark == 0 && (int)g_tile_tribe[col][row] >= 0)
                Sprite_Draw(&s1, &g_icon_sprites[ICON_CLAIM], (GRPort *)w, x + 0x20, y + 0x10);
            if (mark & 0x80) {
                tx = g_colonies[w->colony].x + col - 2;
                ty = g_colonies[w->colony].y + row - 2;
                for (unit = top_unit_in_square(tx, ty); unit >= 0 && g_unit_types[g_units[unit].type].attack <= 1;
                     unit = next_unit(unit))
                    ;
                if (unit >= 0)
                    PieceDraw((GRPort *)w, unit, 0xe0, x + 0x30, y + 0x20, 0x20, 0x64);
            }
            if (mark & 8) {
                DrawIconRow((GRPort *)w, 0x17, g_center_food, g_center_food, x + 0x10, y, 0x36, 0x20, 0, 0);
                if ((int)g_center_kind >= 0)
                    DrawIconRow((GRPort *)w, g_center_kind + 0x17, g_center_amount, g_center_amount, x + 0x10, y + 0x1a,
                                            0x36, 0x20, 0, 0);
            }
            amount = compute_yield(col, row, &goods, 1);
            icon = goods + 0x17;
            colonist = get_field(col, row);
            job = get_job(colonist);
            if (job == JOB_FISHERMAN)
                icon = 0x3a;
            if (amount > 0)
                DrawIconRow((GRPort *)w, icon, amount, amount, x + 0x10, y, 0x36,
                                        amount > 2 ? 0x20 : 0x40, 0, 0);
            else if (goods >= 0) {
                Sprite_Draw(&s2, &g_icon_sprites[icon], (GRPort *)w, x + 0x14, y + 1);
                Sprite_Draw(&s3, &g_icon_sprites[ICON_CELL_FRAME], (GRPort *)w, x + 0x14, y + 1);
            }
            if (goods >= 0)
                PieceIconDraw(w, get_icon(colonist), x + 0x30, y + 0x20, 3);
            if (g_scout_visit != 0)
                continue;
            colonist = get_field(col, row);
            if (g_selected_colonist == colonist) {
                TextColor(g_text_colour);
                SetRect(&r, x, y, x + 0x40, y + 0x40);
                GRPort_DrawRect((GRPort *)w, &r);
            }
        }
    }
    g_draw_labels = 0;
    if (redraw != 0) {
        SetRect(&r, w->rect.left + 0x1a6, w->rect.top + 0x22, w->rect.left + 0x26a, w->rect.top + 0xe6);
        copy_rect_to_screen_1010_e6a4(w, &r);
    }
}
