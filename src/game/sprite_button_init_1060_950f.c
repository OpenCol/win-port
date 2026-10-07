/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 58 of the imported file. */
/* PORT: this file's own declarations of Sprite_GetHotSpot, ctl_window_1060_ab51, dlg_control_init_1060_aa64, get_ctable_1060_aa47, port_height_1060_ab38, port_width_1060_ab1f, sprite_height_1060_aafc, sprite_width_1060_aad9 removed: the header declares them from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 64 of the imported file. */
/* sprite_button_init_1060_950f: candidate for 1060:950f (811 bytes).  Verdict: RELOCATABLE.
 *
 * Builds a w-by-h button from a sprite rather than from a picture resource: the control record is
 * registered at (x, y), and the two ports inside it -- the up face at +0x22 and the down face at
 * +0x48 -- are sized to the button and filled with `color`. The sprite is drawn centred in both,
 * its hot spot moved to its own centre for the purpose and put back afterwards, at map scale 1:1
 * with the previous scale restored. The down face is then remade from the up face: it is cleared
 * to the colour at DGROUP:0x337e, and everything of the up face but its last two rows and columns
 * is copied into it two pixels down and right -- the pressed look. Both ports take the palette of
 * the control's window, and the control window is created visible or not as `visible` says.
 *
 * The twin of `sprite_button_init_1060_91e5` directly above it, identical but for that last
 * argument, which the other hard-codes to 1.
 *
 * `r` is SET and then offset by its own corner, so it ends up at the origin; the two copies of it
 * are plain struct assignments.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GetGlobalSpriteScale(int *num, int *den);
extern void SetGlobalSpriteScale(int num, int den);

extern void GRPort_FillColor_all(GRPort *p, int color);
extern void Sprite_SetHotSpot(Sprite *s, int x, int y);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void GRPort_ColorSync(GRPort *port, CTable *table);

/* same object: 4-byte `push cs; call ` (rule 47) */

void sprite_button_init_1060_950f(SImageButton *ctrl, void *win, int id, register int x, register int y,
                                      int w, int h, void *sprite, unsigned char color,
                                      int visible)
{
    RECT r;
    RECT dst;
    RECT src;
    int cx;
    int cy;
    int hx;
    int hy;
    int sw;
    int sh;
    int sx;
    int sy;
    RECT out1;
    RECT out2;

    ctrl->reinit_faces = 0;
    ctrl->ctable = get_ctable_1060_aa47((Screen *)win);
    SetRect(&r, x, y, x + w, y + h);
    dlg_control_init_1060_aa64((DlgControl *)((SImageButton *)((DlgControl *)ctrl)), win, 6, id, &r);
    ctrl->on_click = 0L;
    GetGlobalSpriteScale(&sx, &sy);
    SetGlobalSpriteScale(1, 1);
    GRPort_Init_wh(&ctrl->down, w, h);
    GRPort_Init_wh(&ctrl->up, w, h);
    GRPort_FillColor_all((GRPort *)&ctrl->down, color);
    GRPort_FillColor_all((GRPort *)&ctrl->up, color);
    cx = w >> 1;
    cy = h >> 1;
    Sprite_GetHotSpot((Sprite *)sprite, &hx, &hy);
    sw = sprite_width_1060_aad9((Sprite *)sprite) >> 1;
    sh = sprite_height_1060_aafc((Sprite *)sprite) >> 1;
    Sprite_SetHotSpot((Sprite *)sprite, -sw, -sh);
    Sprite_Draw(&out1, (Sprite *)sprite, (GRPort *)&ctrl->down, cx, cy);
    Sprite_Draw(&out2, (Sprite *)sprite, (GRPort *)&ctrl->up, cx, cy);
    Sprite_SetHotSpot((Sprite *)sprite, hx, hy);
    SetGlobalSpriteScale(sx, sy);
    SetRect(&r, x, y, port_width_1060_ab1f((GRPort *)&ctrl->down) + x - 1, port_height_1060_ab38((GRPort *)&ctrl->down) + y - 1);
    OffsetRect(&r, -r.left, -r.top);
    dst = r;
    SetRect(&dst, r.left, r.top, r.right - 2, r.bottom - 2);
    src = dst;
    OffsetRect(&dst, 2, 2);
    GRPort_FillColor_all((GRPort *)&ctrl->down, g_draw_color);
    GRPort_CopyToPort((GRPort *)&ctrl->up, (GRPort *)&ctrl->down, &src, &dst);
    GRPort_ColorSync((GRPort *)&ctrl->down, (CTable *)(get_ctable_1060_aa47((Screen *)(ctl_window_1060_ab51((DlgControl *)ctrl)))));
    GRPort_ColorSync((GRPort *)&ctrl->up, (CTable *)(get_ctable_1060_aa47((Screen *)(ctl_window_1060_ab51((DlgControl *)ctrl)))));
    ctrl->hwnd = create_ctrl_win(x, y, w, h, ctrl, visible);
}
