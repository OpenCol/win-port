/* PORT: this file's own declarations of ColonyWin_UpdateColony, GRPort_String, copy_rect_to_screen_1010_e6a4 removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of MSGetTextLength_1010_e2d2, font_height_1010_e73d removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_DrawMarket: candidate for 1010:8ef4 (972 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawMarket
 * name-source: sibling-build
 *
 * Draws the colony window's warehouse strip: the sixteen goods, 38 pixels apart along the bottom, each
 * as its icon at 7/8 size (cigars and muskets at full size, two pixels higher) with the colony's stock
 * written under it. The strip `ColonyWin_ColonyMouseMarket` and the goods popup
 * (`label_cargo`) work on.
 *
 * A stock of 100 or more is written in two parts, hundreds and the two-digit remainder, in two colours
 * and centred on the column; a stock below 100 is written whole in the second colour. The colours are
 * 0x67 and 0x84, with two exceptions:
 *
 *   - above the warehouse's capacity (`get_warehouse`), for any goods but food, both parts
 *     turn to 0x3d / 0x3c;
 *   - when the colony has building 0x12 -- BUILDING18, "CUSTOM HOUSE" -- and the goods' bit is set in
 *     the bitmap `custom_sell` reads, the second part is 0xfa. That reads as the goods the
 *     custom house exports.
 *
 * The first thing the loop does with the text buffer is dead: the stock is formatted into `num` and
 * appended to an emptied `buf`, and both are overwritten before either is drawn. The redraw rectangle
 * covers the strip, and the scale is reset to 1:1 on the way out.
 *
 * RENAMED 2026-09-26 from colony_draw_warehouse: the Mac build's ColonyWin::DrawMarket(short): this plus a redraw flag, painting the sixteen-goods strip -- what EuropeWin_DrawMarket paints on the Europe screen.
 */

#include "colonization.h"

#include "w16types.h"
#include "building.h"
#include "goods.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Font, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"

   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "icons.h"

extern void set_colony(int colony);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern int has_building(int bit);
extern int custom_sell(int n);
extern int get_warehouse(void);


/* same object: 4-byte `push cs; call ` (rule 47) */

void ColonyWin_DrawMarket(ColonyWin *w, int redraw)
{
    int colx;
    int iy;
    int ty;
    int x;
    int ty2;
    int hundreds;
    int rem;
    int c1;
    int c2;
    RECT r;
    RECT s1;
    RECT s2;
    char num[20];
    char buf[0x50];
    register int i;
    register int tx;

    set_colony(w->colony);
    ColonyWin_UpdateColony(w, 0, 0x186, 0x262, 0x32);
    colx = w->rect.left + 0x14;
    iy = w->rect.top + 0x18b;
    SetGlobalSpriteScale(7, 8);
    for (i = 0; i < GOODS_COUNT; i++) {
        x = colx;
        if (i == GOODS_CIGARS || i == GOODS_MUSKETS) {
            SetGlobalSpriteScale(1, 1);
            Sprite_Draw(&s1, &g_icon_sprites[ICON_GOODS + i], (GRPort *)w, x, iy - 2);
            SetGlobalSpriteScale(7, 8);
        } else
            Sprite_Draw(&s2, &g_icon_sprites[ICON_GOODS + i], (GRPort *)w, x, iy);
        SetGlobalSpriteScale(7, 8);
        buf[0] = 0;
        LTOA1(g_colonies[w->colony].stock[i], num, 10);
        STRCAT(buf, num);
        ty = w->rect.top + 0x1ad;
        tx = colx;
        hundreds = g_colonies[w->colony].stock[i] / 100;
        rem = g_colonies[w->colony].stock[i] % 100;
        c1 = 0x67;
        c2 = 0x84;
        if (has_building(BUILDING_CUSTOM_HOUSE) != 0 && custom_sell(i) != 0)
            c2 = 0xfa;
        if (i != GOODS_FOOD && get_warehouse() < g_colonies[w->colony].stock[i]) {
            c1 = 0x3d;
            c2 = 0x3c;
        }
        if (hundreds > 0) {
            LTOA1(hundreds, buf, 10);
            STRCPY(num, buf);
            append_number_padded(num, rem, 2);
            tx -= MSGetTextLength_1010_e2d2(&g_font_tiny, num) >> 1;
            TextColor(c1);
            buf[0] = 0;
            LTOA1(hundreds, buf, 10);
            ty2 = ty - (font_height_1010_e73d(&g_font_tiny) >> 1);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_tiny, buf, tx, ty2, 5);
            tx += MSGetTextLength_1010_e2d2(&g_font_tiny, buf);
            buf[0] = 0;
            append_number_padded(buf, rem, 2);
            TextColor(c2);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_tiny, buf, tx, ty2, 5);
            tx += MSGetTextLength_1010_e2d2(&g_font_tiny, buf) + 2;
        } else {
            LTOA1(rem, buf, 10);
            TextColor(c2);
            GRPort_String((GRPort *)w, (unsigned *)&g_font_tiny, buf, tx + 1, ty, 0);
            tx += MSGetTextLength_1010_e2d2(&g_font_tiny, buf) + 2;
        }
        colx += 0x26;
    }
    if (redraw != 0) {
        SetRect(&r, w->rect.left, w->rect.top + 0x186, w->rect.left + 0x262, w->rect.top + 0x1b8);
        copy_rect_to_screen_1010_e6a4(w, &r);
    }
    SetGlobalSpriteScale(1, 1);
}
