/* PORT: this file's own declarations of MSShowControl_1058_c26f, sprite_height_1058_c24c removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_TradeRoute2: candidate for 1058:7df4 (441 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_TradeRoute2
 *
 * The trade-route report's background: the window is tiled with the sprite at SEG20:0x7506
 * (as many columns and rows as fit), and on the first call a "Close" SButton (id 0xca,
 * 200x50 at the top right) is created, wired to ReportWin_OnTradeRouteExtraClick and shown. The
 * module's small wrappers and SButton's constructor are defined here because the shipped
 * calls to them are the compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void DialogWin_AddControl(void *win, DlgControl *ctl);
extern short MSInitButton(RECT *rgn, SButton *obj, char *title, int visible);

extern void ReportWin_OnTradeRouteExtraClick(void);

extern "C" int sprite_width_1058_c203(void *sprite);


extern "C" void dlg_control_init_1058_c3f6(SButton *ctl, void *win, int kind, int id, RECT *r);

extern "C" void sbutton_init_1058_c11b(SButton *button, void *win, int id, RECT *pos, char *title);

extern "C" void set_button_action_1058_c17f(SButton *button, void *h);


extern "C" void ReportWin_TradeRoute2(ReportWin *o)
{
    SButton *button;
    RECT rc;
    int row, col;

    for (col = 0; col < rect_width_1058_bd2d(&o->rect) / sprite_width_1058_c203(&g_background_tile); col++)
        for (row = 0; row < rect_height_1058_bd4c(&o->rect) / sprite_height_1058_c24c(&g_background_tile); row++)
            Sprite_Draw(&rc, &g_background_tile, o, o->rect.left + sprite_width_1058_c203(&g_background_tile) * col, o->rect.top + sprite_height_1058_c24c(&g_background_tile) * row);
    if (o->extra_btn == 0) {
        SetRect(&rc, 0, 0, 200, 50);
        OffsetRect(&rc, o->rect.right - 210, o->rect.top + 10);
        o->extra_btn = new SButton;
        sbutton_init_1058_c11b(o->extra_btn, ((ReportWinBase *)o)->win, 0xca, &rc, "Close");
        set_button_action_1058_c17f(o->extra_btn, (void *)ReportWin_OnTradeRouteExtraClick);
        MSShowControl_1058_c26f(o->extra_btn);
    }
}
