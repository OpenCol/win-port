/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* ReportWin_EconReport3: candidate for 1058:4825 (453 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: ReportWin_EconReport3
 *
 * The economy report's grid: the title (word SEG20:0xdd2c) centred at half the window's
 * width in colour 249, a vertical rule, then for each of the 16 goods its sprite (32-byte
 * records at SEG20:0x6526) at x = 150 + 30*i and the rule after it, and 18 horizontal rules
 * 20 apart from y = 72. The width wrappers are defined here because the shipped calls to
 * them are the compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from draw_econ_report_block: the Mac build's ReportWin::EconReport3(short), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (callee DrawLine; callee Draw; callee TextColor; callee set_europe; callee strings), the runner-up gets 3, and the arity agrees (2). Mac EconReport3's only caller is EconReport2, and this function's only caller is draw_econ_report_paged, bound to EconReport2 in the same batch.
 */

#include "colonization.h"

#include "w16types.h"
#include "goods.h"
#include "misc.h"
#include "classes.h"
#include "icons.h"

extern void set_europe(int n);

extern char *strings(int n);
extern void GRPort_DrawLine(GRPort *w, short x1, short y1, short x2, short y2);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern int port_width_1058_bcfb(GRPort *port);

void ReportWin_EconReport3(ReportWin *o, int nation)
{
    int y;
    RECT out;
    int i;
    int x;

    set_europe(nation);
    TextColor(0xf9);
    GRPort_String((GRPort *)o, (unsigned *)&g_report_font, strings(g_misc_words[MISC_ECONOMIC_ADVISOR_REPORT]), port_width_1058_be8e((GRPort *)o) / 2, o->rect.top + 18, 16);
    x = 150;
    y = 40;
    TextColor(0xf9);
    GRPort_DrawLine((GRPort *)o, o->rect.left + x - 3, o->rect.top + 40, o->rect.left + x - 3, o->rect.top + 412);
    SetGlobalSpriteScale(7, 8);
    for (i = 0; i < GOODS_COUNT; i++) {
        Sprite_Draw(&out, &g_icon_sprites[ICON_GOODS + i], (GRPort *)o, o->rect.left + x + 13, o->rect.top + y);
        x += 30;
        GRPort_DrawLine((GRPort *)o, o->rect.left + x - 3, o->rect.top + 40, o->rect.left + x - 3, o->rect.top + 412);
    }
    SetGlobalSpriteScale(1, 1);
    for (i = 0; i < 18; i++)
        GRPort_DrawLine((GRPort *)o, o->rect.left + 2, o->rect.top + i * 20 + 72, o->rect.left + 630, o->rect.top + i * 20 + 72);
}
