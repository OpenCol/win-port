/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: this file's own declarations of Sprite_Init, port_height_1060_86e2, port_width_1060_86c9, sprite_height_1060_8842, sprite_width_1060_881f removed: the header declares them from the definition (W1 step 3). */
/* AboutWin_UpdateCredits: candidate for 1060:790c (1505 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: AboutWin_UpdateCredits
 * name-source: sibling-build
 *
 * ONE TICK OF THE ABOUT BOX, called from `AboutWin_DoCredits`'s idle loop. The whole animation is
 * a single counter at DGROUP:0x4e66 and a deadline in the window at +0xba0: nothing moves until
 * `MSGetTicks_1060_87d0()` passes the last frame's stamp by 60, and each frame that does fire
 * bumps the counter and stamps the clock again.
 *
 * STEPS 0 TO 13 REVEAL THE CREDITS a group at a time. `g_step_group[step]` names a row of the
 * eight-wide table at SEG32:0, and every member of that row that is not -1 turns on a flag in the
 * window's 25-entry `shown[]`. A step whose group is -1 reveals nobody and is a beat of silence.
 * The page is then repainted from scratch: two `GRPort_CopyToPort`s lay the background and its overlay
 * back down, and the 25 credit slots are walked, each one drawn only if its art id's flag is on,
 * at the fixed position the pair of tables at SEG22:0xe8 and SEG22:0x11a give it.
 *
 * STEP 14 is a pause -- it only advances the counter and pushes the deadline out by another 60 --
 * and step 15 draws the final page: everything visible, the tile sprite (+0xb7c) tiled into a box sized from the
 * logo sprite plus 0x10 by 0x20 and centred 0xb6 up from the bottom, the logo over it, and the
 * copyright line 0x2c from the bottom. Step 16 is the way back in: it turns on all 25 flags and
 * sets the counter to 15, so whatever calls it again gets the finished page redrawn rather than
 * the sequence restarted.
 *
 * THE COSTLY LINE: the two `Sprite_Draw` calls that put the step's banner and footer on the page
 * sit INSIDE the 25-slot loop and depend only on the step, so each frame draws them twenty-five
 * times over. Both also re-read `g_step_banner[step]` and call `get_w_2` again every iteration.
 * Nothing shows, because each redraw lands on the same pixels; it is the same frame drawn 25
 * times.
 *
 * `SAR AX,1` with no rounding correction in front of it is a SHIFT, so the centring is `>> 1` and
 * not `/ 2`. The tick tests are the 386 long form: the call's DX:AX is folded into EAX with
 * `SHL EAX,16 / SHRD EAX,EDX,16` before the compare.
 *
 * RENAMED 2026-09-26 from about_animate: the Mac build's AboutWin::UpdateCredits(void): this only, one tick of the credits animation, called from AboutWin_DoCredits' idle loop. The width matches.
 */

#include "colonization.h"

#include "fathers.h"
#include "w16types.h"
/* PORT: this file's own Font, Sprite, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, CTable, AboutWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-



class DlgControl;





extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void DoFizzle(GRPort *port, GameWin *w, RECT *r,
                              int x, int y, int mode, int step);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);


/* same object: 4-byte `push cs; call ` (rule 47) */



void AboutWin_UpdateCredits(AboutWin *o)
{
    register int i;
    register int y;
    Sprite spr;
    RECT box;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    RECT r5;
    RECT r6;

    if (g_about_step < 0xe && MSGetTicks_1060_87d0() > o->last + 0x3c) {
        if (g_step_group[g_about_step] != -1)
            for (i = 0; i < 8; i++)
                if (g_group_members[g_step_group[g_about_step]][i] != -1)
                    o->shown[g_group_members[g_step_group[g_about_step]][i]] = 1;
        GRPort_CopyToPort(&o->page, &o->pic, &o->top_strip, &o->top_strip);
        GRPort_CopyToPort(&o->overlay, &o->pic, &o->top_strip, &o->bottom_strip);
        for (i = 0; i < FATHER_COUNT; i++) {
            if (o->shown[g_father_order[i]] == 1) {
                Sprite_Init(&spr, g_father_order[i] + 0x2bc);
                Sprite_Draw(&r1, &spr, &o->pic, g_father_x[i], g_father_y[i]);
            }
            Sprite_Draw(&r2, &o->letters[g_step_banner[g_about_step]], &o->pic,
                        port_width_1060_86c9(&o->pic) >> 1, 0x1e);
            Sprite_Draw(&r3, &o->letters[g_step_footer[g_about_step]], &o->pic,
                        port_width_1060_86c9(&o->pic) >> 1, port_height_1060_86e2(&o->pic) - 0x3c);
        }
        DoFizzle(&o->pic, o, &o->full, 0, 0, -1, 0x1e);
        g_about_step++;
        o->last = MSGetTicks_1060_87d0();
    } else if (g_about_step == 0xe && MSGetTicks_1060_87d0() > o->last + 0x3c) {
        g_about_step++;
        o->last = MSGetTicks_1060_87d0() + 0x3c;
    } else if (g_about_step == 0x10) {
        for (i = 0; i < FATHER_COUNT; i++)
            o->shown[i] = 1;
        g_about_step = 0xf;
    }
    if (g_about_step == 0xf && MSGetTicks_1060_87d0() > o->last + 0x3c) {
        GRPort_CopyToPort(&o->page, &o->pic, &o->top_strip, &o->top_strip);
        GRPort_CopyToPort(&o->overlay, &o->pic, &o->top_strip, &o->bottom_strip);
        for (i = 0; i < FATHER_COUNT; i++) {
            Sprite_Init(&spr, g_father_order[i] + 0x2bc);
            Sprite_Draw(&r4, &spr, &o->pic, g_father_x[i], g_father_y[i]);
        }
        SetRect(&box, 0, 0, sprite_width_1060_881f(&o->logo) + 0x10,
                sprite_height_1060_8842(&o->logo) + 0x20);
        OffsetRect(&box, (port_width_1060_86c9(&o->pic) - rect_width_1060_8865(&box)) >> 1,
                   port_height_1060_86e2(&o->pic) - 0xb6);
        GRPort_SetClip(&o->pic, &box);
        for (i = box.left; i < box.right; i += sprite_width_1060_881f(&o->tile))
            for (y = box.top; y < box.bottom; y += sprite_height_1060_8842(&o->tile))
                Sprite_Draw(&r5, &o->tile, &o->pic, i, y);
        GRPort_ResetClip(&o->pic);
        Sprite_Draw(&r6, &o->logo, &o->pic, port_width_1060_86c9(&o->pic) >> 1,
                    port_height_1060_86e2(&o->pic) - 0xfa);
        TextColor(0xfb);
        GRPort_String(&o->pic, (unsigned *)&g_font_small,
                             "Copyright\251 1994, 1995 by MicroProse Software Inc.",
                             port_width_1060_86c9(&o->pic) >> 1,
                             port_height_1060_86e2(&o->pic) - 0x2c, 0x10);
        DoFizzle(&o->pic, o, &o->full, 0, 0, -1, 0x1e);
        g_about_step++;
        o->last = MSGetTicks_1060_87d0();
    }
}

