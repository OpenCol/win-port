/* PORT: this file's own declarations of GameWin_Init, win_center_1020_abcb removed: the header declares them from the definition (W1 step 3). */
/* PopupBoxDraw: candidate for 1020:669e (540 bytes).  Verdict: RELOCATABLE.
 * symbol: PopupBoxDraw
 * name-source: sibling-build
 *
 * Makes a plain bordered window with a tiled background: `GameWin_Init`
 * with an empty title and style 0x801, the whole of it filled with colour 0x24, sprite 383
 * tiled over it on a 0x3e by 0x2f grid inside a three-pixel margin, and a six-line border --
 * four rectangles in colour 0x21 and two more in 0x19, each one pixel inside the last.
 *
 * Then three handler slots on the window's +0x33 sub-object get the two `refresh_port_30`
 * functions, and +0x26 is centred. Its one caller builds a 0x190 by 0xf0 window at SEG22:0xa.
 *
 * THE FILL RECTANGLE IS SQUARE AND SHOULD NOT BE: `SetRect(&r, 0, 0, w, w)` -- the height
 * argument is the WIDTH, where the border's rectangle two calls later correctly uses `h`. With
 * the only caller's 400 by 240 the fill runs 160 pixels past the bottom and is clipped away,
 * so nothing shows. It is in the shipped bytes and is left there.
 *
 * The border is six copies of the same three statements rather than a loop, which is what the
 * shipped does: the colour changes between the fourth and the fifth, and a loop would need the
 * count.
 *
 * `px` and `py` are the register locals -- SI then DI in declaration order, so the INNER
 * loop's counter takes SI even though the outer loop's is assigned first (rule 53).
 *
 * `g_4bec` IS WRITTEN AND NEVER READ. Three instructions in the image touch 0x4bec -- the
 * `mov word [0x4bec],0x0` in `PopupBoxDraw` and the two `mov word [0x4bec],0x1` in the
 * twin refresh functions -- and nothing anywhere loads it. So the flag records that the window
 * was dismissed and no one asks. It is vestigial, which is why it is not named for the question
 * it would have answered; see `tools/check_dead_dgroup.py`.
 *
 * RENAMED 2026-09-26 from build_bordered_window: the Mac build's PopupBoxDraw(GameWin&, short, short, short, short), bound by call-sequence alignment (1.0 aligned gaps in combat_analysis leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.92x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern MainWin g_main_win;             /* SEG20:0x5ca */


extern void GRPort_FillColor(GRPort *w, RECT *r, int color);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_ResetClip(GRPort *port);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern void bordered_win_dismiss_1020_667c(void);
extern void bordered_win_dismiss_1020_665a(void);

/* same object: 4-byte `push cs; call ` */
/* PORT: the in-file stubs of set_on_lbutton_down_1020_ab20, set_on_rbutton_down_1020_ab59, set_on_keydown_1020_ab92 removed: they shaped the call for the byte match, and
   src/ defines each function (W1). */

void PopupBoxDraw(char *win, int x, int y, int w, int h)
{
    RECT r;
    RECT tmp;
    register int px;
    register int py;

    g_4bec = 0;
    GameWin_Init((GameWin *)win, "", 0x801, x, y, w, h,
                                     &g_game_palette, (Screen *)&g_main_win);
    SetRect(&r, 0, 0, w, w);
    GRPort_FillColor((GRPort *)win, &r, 0x24);
    SetRect(&r, 3, 3, w - 3, h - 3);
    GRPort_SetClip((GRPort *)win, &r);
    for (py = 3; h - 3 > py; py += 0x2f)
        for (px = 3; w - 3 > px; px += 0x3e)
            Sprite_Draw(&tmp, &g_background_tile, (GRPort *)win, px, py);
    GRPort_ResetClip((GRPort *)win);
    SetRect(&r, 0, 0, w, h);
    TextColor(0x21);
    GRPort_DrawRect((GRPort *)win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect((GRPort *)win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect((GRPort *)win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect((GRPort *)win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    TextColor(0x19);
    GRPort_DrawRect((GRPort *)win, &r);
    r.left++; r.right--; r.top++; r.bottom--;
    GRPort_DrawRect((GRPort *)win, &r);
    set_on_lbutton_down_1020_ab20((EventTable *)(win + 0x33), bordered_win_dismiss_1020_667c);
    set_on_rbutton_down_1020_ab59((EventTable *)(win + 0x33), bordered_win_dismiss_1020_667c);
    set_on_keydown_1020_ab92((EventTable *)(win + 0x33), bordered_win_dismiss_1020_665a);
    win_center_1020_abcb((DialogWin *)(win + 0x26));
}
