/* PORT: this file's own declarations of ColonyWin_UpdateColony, copy_rect_to_screen_1010_e6a4 removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_DrawMiscellaneous: candidate for 1010:8190 (316 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawMiscellaneous
 * name-source: sibling-build
 *
 * Paints the colony window's 186x115 panel at (414, 272): backdrop cell,
 * clip rectangle (GRPort_SetClip), then by the mode byte at DGROUP:0xc0f the
 * mode-0 or mode-1 painter with the two controls at +0x950/+0x972, or the
 * mode-2 painter; the clip is dropped (GRPort_ResetClip) and, when `redraw`
 * is set, the rectangle goes to the screen.  The five module helpers are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_draw_panel6: the Mac build's ColonyWin::DrawMiscellaneous(short): this plus a redraw flag, painting the bottom-right panel in the mode ColonyWin_MiscButtonClick selects.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;
extern void set_colony(int colony);

extern void GRPort_SetClip(GRPort *port, RECT *r);
extern void GRPort_ResetClip(GRPort *port);

extern "C" void ColonyWin_DrawMakeList(ColonyWin *w);
extern "C" void ColonyWin_DrawColonyUnits(ColonyWin *w);
extern "C" void ColonyWin_DrawProduction(ColonyWin *w);
extern "C" void MSHideControl_1010_e54d(void *control);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void ColonyWin_DrawMiscellaneous(ColonyWin *w, int redraw)
{
    RECT r;

    set_colony(w->colony);
    ColonyWin_UpdateColony(w, 0x19e, 0x110, 0xba, 0x73);
    SetRect(&r, w->rect.left + 0x19e, w->rect.top + 0x110, w->rect.left + 0x258, w->rect.top + 0x183);
    GRPort_SetClip((GRPort *)w, &r);
    switch (g_colony_mode) {
    case 0:
        ColonyWin_DrawMakeList(w);
        MSHideControl_1010_e54d(&w->b[0]);
        MSHideControl_1010_e54d(&w->b[1]);
        break;
    case 1:
        ColonyWin_DrawColonyUnits(w);
        MSHideControl_1010_e54d(&w->b[0]);
        MSHideControl_1010_e54d(&w->b[1]);
        break;
    case 2:
        ColonyWin_DrawProduction(w);
        break;
    }
    GRPort_ResetClip((GRPort *)w);
    if (redraw != 0)
        copy_rect_to_screen_1010_e6a4(w, &r);
}
