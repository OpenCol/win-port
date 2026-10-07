/* ReportWin_TradeRoute1: candidate for 1058:7baa (549 bytes).  Verdict: RELOCATABLE.
 * symbol: ReportWin_TradeRoute1
 *
 * The trade-route report's route list from `y0`: for every stop of the current route
 * (DGROUP:0x4c18, count at +0x21) the line "<n>. <stop name>" in colour 249 at x 10, then
 * the goods UNLOADED (get_num_orders(0) / get_order(j)) as sprites from x 231 and the
 * goods LOADED (get_num_orders(1) / get_order(j + 6)) from x 431 -- under the "Unload Cargo"
 * and "Load Cargo" headings ReportWin_TradeRoutes draws at x+0xe1 and x+0x1a9 (corrected 2026-09-26;
 * this said loaded/unloaded the other way round), each sprite advancing by half its width plus 6 on
 * either side; rows are 75 apart. The width wrappers are defined here because the shipped
 * calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

#include "icons.h"

extern char *dest_name(int stop);
extern int get_num_orders(int dir);
extern int get_order(int slot);

extern void period_string(char *buf);

extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern int sprite_width_1058_c203(void *sprite);

void ReportWin_TradeRoute1(ReportWin *o, int y0)
{
    int y;
    int j;
    int i;
    int n;
    RECT rc1;
    RECT rc2;
    char buf[0x50];
    int x;
    int goods;

    for (i = 0; g_cur_route_rec->count > i; i++) {
        set_route(i);
        x = 10;
        y = y0;
        buf[0] = 0;
        say_number(buf, i + 1);
        period_string(buf);
        STRCAT(buf, dest_name(i));
        TextColor(0xf9);
        GRPort_String((GRPort *)o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y + 37, 1);
        x = 231;
        n = get_num_orders(0);
        for (j = 0; j < n; j++) {
            goods = get_order(j);
            if (goods >= 0) {
                x += sprite_width_1058_c203(&g_icon_sprites[ICON_GOODS + goods]) / 2 + 6;
                Sprite_Draw(&rc1, &g_icon_sprites[ICON_GOODS + goods], (GRPort *)o, o->rect.left + x, o->rect.top + y + 27);
                x += sprite_width_1058_c203(&g_icon_sprites[ICON_GOODS + goods]) / 2 + 6;
            }
        }
        x = 431;
        n = get_num_orders(1);
        for (j = 0; j < n; j++) {
            goods = get_order(j + 6);
            if (goods >= 0) {
                x += sprite_width_1058_c203(&g_icon_sprites[ICON_GOODS + goods]) / 2 + 6;
                Sprite_Draw(&rc2, &g_icon_sprites[ICON_GOODS + goods], (GRPort *)o, o->rect.left + x, o->rect.top + y + 27);
                x += sprite_width_1058_c203(&g_icon_sprites[ICON_GOODS + goods]) / 2 + 6;
            }
        }
        y0 += 75;
    }
}
