/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 58 of the imported file. */
/* Popup_fill_area: candidate for 1050:41ad (1091 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * PAINTS A WINDOW'S BACKGROUND, for `1050:4829`. The flag at +0x340 picks between two nearly identical
 * arms, and the difference is what is behind the parchment.
 *
 * With the flag set, the window's own 32x32 tile at +0x4e is laid over the whole port first, and the
 * parchment area is the rectangle at the origin (+0x33c, +0x33e) of size (+0x88, +0x8a). With it clear,
 * the port is filled with colour 0x24 and the parchment area is the whole port inset by 3.
 *
 * Either way the parchment sprite at SEG20:0x7506 is tiled inside that rectangle in steps of 62 by 47
 * under a clip, and the border is six nested rectangles drawn one pixel apart: four in colour 0x21 and
 * two in 0x19.
 *
 * The indicators (`Popup_DrawDude`, reached by the NOP form and so extern) are drawn only in the
 * flagged arm, and only when DGROUP:0x2518 is 5.
 *
 * TWO REGISTER VARIABLES, and their roles SWAP between the loops: the first pair is (row, column) and
 * the parchment loops take (column, row), so the shipped code puts the wallpaper's row and the
 * parchment's column in SI. That is why they are spelled `a` and `b` here.
 *
 * RENAMED 2026-09-26 from win_draw_background: the Mac build's Popup::fill_area(int, int, int, int): this plus an area -- Popup_draw passes the popup's own (x, y, w, h) -- painting the background tile and the parchment inside it. The Windows body reads those fields itself and ignores the four arguments. It sits in the Popup module (1050) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

extern int g_popup_tribe;                     /* DGROUP:0x2518, declared */
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);
extern void GRPort_DrawRect(GRPort *p, RECT *r);
extern void GRPort_FillColor(GRPort *p, RECT *r, int color);

/* same object: 4-byte `push cs; call ` */

extern void Popup_DrawDude(Popup *w);

void Popup_fill_area(Popup *popup, int unused1, int unused2, int unused3, int unused4)
{
    int width;
    int height;
    RECT rc;
    RECT r1;
    RECT r2;
    RECT r3;
    register int a;
    register int b;

    if (popup->framed != 0) {
        width = port_width_1050_8b8a(popup->win);
        height = port_height_1050_8ba9(popup->win);
        for (a = 0; a < height; a += 0x20)
            for (b = 0; b < width; b += 0x20)
                Sprite_Draw(&r1, (Sprite *)(&popup->tile), popup->win, b, a);
        SetRect(&rc, popup->box_x, popup->box_y, popup->box_x + popup->box_w, popup->box_y + popup->box_h);
        if (g_popup_tribe == 5)
            Popup_DrawDude(popup);
        GRPort_SetClip(popup->win, &rc);
        for (b = popup->box_y + 3; height - 3 > b; b += 0x2f)
            for (a = popup->box_x + 3; width - 3 > a; a += 0x3e)
                Sprite_Draw(&r2, &g_background_tile, popup->win, a, b);
        GRPort_ResetClip(popup->win);
        SetRect(&rc, popup->box_x, popup->box_y, popup->box_x + popup->box_w, popup->box_y + popup->box_h);
        TextColor(0x21);
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        TextColor(0x19);
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
    } else {
        width = port_width_1050_8b8a(popup->win);
        height = port_height_1050_8ba9(popup->win);
        SetRect(&rc, 0, 0, width, height);
        GRPort_FillColor(popup->win, &rc, 0x24);
        SetRect(&rc, 3, 3, width - 3, height - 3);
        GRPort_SetClip(popup->win, &rc);
        for (b = 3; height - 3 > b; b += 0x2f)
            for (a = 3; width - 3 > a; a += 0x3e)
                Sprite_Draw(&r3, &g_background_tile, popup->win, a, b);
        GRPort_ResetClip(popup->win);
        SetRect(&rc, 0, 0, width, height);
        TextColor(0x21);
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        TextColor(0x19);
        GRPort_DrawRect(popup->win, &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect(popup->win, &rc);
    }
}
