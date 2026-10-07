/* PORT: this file's own declarations of text_search removed: the header declares it from the definition (W1 step 3). */
/* PediaWin_DoCargoArticle: candidate for 1060:37a1 (716 bytes).  Verdict: RELOCATABLE.
 *
 * THE CARGO TYPES ARTICLE of the Colonizopedia: the selected list item's goods (+4) heads the page as
 * "(<goods>: <PEDIA line 0>)" in the heading font, centred. Rows from `PediaWin_CargoLine`
 * follow, 23 pixels apart, and then the CARGO<n> text fills the rest of the frame in the font at
 * DGROUP:0x4b1a.
 *
 * WHICH ROWS, as (goods, speciality) pairs:
 *  - food: (food, food), then (-1, -1), the row that is not a goods;
 *  - horses and trade goods: (goods, -1) alone;
 *  - silver: (silver, silver) alone;
 *  - ore, tools or muskets: the whole chain -- ore, tools, muskets;
 *  - any other raw goods (below 8): the goods and the one made from it (goods + 8);
 *  - any other made goods: its raw goods (goods - 8) and itself.
 *
 * The pairs `a[i] = b[i] = v` store the right-hand array first. The first three arms index through
 * the register count, and the rest store fixed slots and set the count after. `y = 5` is dead: it is
 * overwritten with 10 before use. It compiled RELOCATABLE on the first build.
 */

#include "colonization.h"

#include "w16types.h"
#include "goods.h"
#include "classes.h"

#include "records.h"

extern void SetGlobalSpriteScale(int num, int den);
extern void open_string(char *s);
extern void close_string(char *s);
extern void colon_string(char *buf);


extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);

extern void draw_text_resource(void *target, char *name, void *rect);

/* same object: 4-byte `push cs; call ` */
extern PediaEntry *PediaWin_CurrentEntry(PediaWin *o);

extern int font_height_1060_695e(Font *font);
extern void PediaWin_CargoLine(PediaWin *w, int goods, int spec, int y);

void PediaWin_DoCargoArticle(PediaWin *o)
{
    int y;
    int row_goods[3];
    int row_spec[3];
    int i;
    PediaEntry *item;
    RECT rc;
    char buf[0x50];
    register int n;
    register int goods;

    item = PediaWin_CurrentEntry(o);
    if (item == 0)
        return;
    goods = item->index;
    y = 5;
    TextColor(12);
    y = 10;
    buf[0] = 0;
    open_string(buf);
    say_string(buf, g_goods_name_word[goods]);
    colon_string(buf);
    text_search("PEDIA", "PEDIA", 0);
    STRCAT(buf, g_text_buf);
    close_string(buf);
    TextColor(12);
    GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, buf, port_width_1060_6886((GRPort *)o) / 2, o->rect.top + y, 0);
    y += font_height_1060_695e((Font *)&g_heading_font) + 2;
    y += 12;
    n = 0;
    if (goods == GOODS_FOOD) {
        row_goods[n] = goods;
        row_spec[n] = goods;
        n++;
        row_goods[n] = -1;
        row_spec[n] = -1;
        n++;
    } else if (goods == GOODS_HORSES || goods == GOODS_TRADE_GOODS) {
        row_goods[n] = goods;
        row_spec[n] = -1;
        n++;
    } else if (goods == GOODS_SILVER) {
        row_goods[n] = goods;
        row_spec[n] = goods;
        n++;
    } else if (goods == GOODS_ORE || goods == GOODS_TOOLS || goods == GOODS_MUSKETS) {
        row_goods[0] = row_spec[0] = GOODS_ORE;
        row_goods[1] = row_spec[1] = GOODS_TOOLS;
        row_goods[2] = row_spec[2] = GOODS_MUSKETS;
        n = 3;
    } else if (goods < 8) {
        row_goods[0] = row_spec[0] = goods;
        row_goods[1] = row_spec[1] = goods + 8;
        n = 2;
    } else {
        row_goods[0] = row_spec[0] = goods - 8;
        row_goods[1] = row_spec[1] = goods;
        n = 2;
    }
    for (i = 0; i < n; i++) {
        PediaWin_CargoLine(o, row_goods[i], row_spec[i], y);
        y += 23;
    }
    y += 10;
    STRCPY(buf, "CARGO");
    say_number(buf, goods);
    SetRect(&rc, o->rect.left + 10, o->rect.top + y, o->rect.right - 10, o->rect.bottom - 20);
    set_wrap_font(&g_number_font);
    draw_text_resource(o, buf, &rc);
    SetGlobalSpriteScale(1, 1);
}
