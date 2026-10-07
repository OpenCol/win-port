/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 33, 34, 36, 40, 42, 47, 48 of the imported file. */
/* Popup_DrawDude: candidate for 1050:45f0 (569 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws the window's indicator sprite (+0x2e) into its target (+0) at a position chosen by
 * whichever of three selectors is active, each a dense `switch` Borland emits as a jump
 * table: plain x, x plus a width (+0x88), or x plus half the size (+0x338); the third
 * selector draws at (10, 10) except case 4. Seven scratch buffers, one per call site.

 * SIZE: the census says 531 and the function is 569. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from draw_indicators: the Mac build's Popup::DrawDude(void): this only, drawing the popup's figure sprite (+0x2e) at the position its layout selects; Popup_draw and Popup_fill_area call it. It sits in the Popup module (1050) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

extern int g_popup_nation;   /* DGROUP:0x251c */
extern int g_popup_tribe;   /* DGROUP:0x2518 */
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void Popup_DrawDude(Popup *w)
{
    char b1[8], b2[8], b3[8], b4[8], b5[8], b6[8], b7[8];

    if (g_popup_dude != -1) {
        switch (g_popup_dude) {
        case 0: Sprite_Draw((RECT *)b1, (Sprite *)(&w->portrait), w->win, w->box_x, w->box_y); break;
        case 1: Sprite_Draw((RECT *)b2, (Sprite *)(&w->portrait), w->win, w->box_x + w->box_w, w->box_y); break;
        case 2: case 3: case 4: case 5:
            Sprite_Draw((RECT *)b3, (Sprite *)(&w->portrait), w->win, w->box_x + (w->win_w >> 1), w->box_y); break;
        }
    } else if (g_popup_nation != -1) {
        switch (g_popup_nation) {
        case 0: Sprite_Draw((RECT *)b4, (Sprite *)(&w->portrait), w->win, w->box_x + w->box_w, w->box_y); break;
        case 1: case 2: case 3:
            Sprite_Draw((RECT *)b5, (Sprite *)(&w->portrait), w->win, w->box_x + (w->win_w >> 1), w->box_y); break;
        }
    } else if (g_popup_tribe != -1) {
        switch (g_popup_tribe) {
        case 0: case 1: case 2: case 3: case 5: case 6: case 7: case 8:
            Sprite_Draw((RECT *)b6, (Sprite *)(&w->portrait), w->win, 10, 10); break;
        case 4: Sprite_Draw((RECT *)b7, (Sprite *)(&w->portrait), w->win, w->box_x + w->box_w, 10); break;
        }
    }
}
