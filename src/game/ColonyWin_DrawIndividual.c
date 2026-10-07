/* ColonyWin_DrawIndividual: candidate for 1010:92c0 (405 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawIndividual
 * name-source: sibling-build
 *
 * Draws the colonists holding `job` in a row at (x, y) of the colony
 * window: ComputeIconRow lays the row out (count from job_count,
 * the SEG21:0x38 width for `slot`) and returns the step; each colonist's
 * portrait goes through PieceIconDraw, and the selected one
 * (DGROUP:0x4bbc) gets a frame half a portrait wide either side.  The
 * portrait-width reader is -called and defined in-file as a stub.
 *
 * `g_slot_span` (SEG21:0x38) is the pixel width a slot's row of portraits gets. It is passed
 * to `ComputeIconRow` as BOTH `span` and `span2`, which is how that function is told the row and
 * the centring width are the same, and `ColonyWin_ColonyMouseBuildings` passes it twice as well when
 * it hit-tests the same row.
 *
 * RENAMED 2026-09-26 from colony_draw_job_row: the Mac build's ColonyWin::DrawIndividual(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DrawABuilding; callee DrawRect; callee TextColor; callee get_icon; callee get_job), the runner-up gets 4, and the arity agrees (5).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

extern int g_stub_a;
extern void set_colony(int colony);
extern int job_count(int job);
extern int ComputeIconRow(int pic, int n, int m, int *px, int y, int w1, int w2, int *pa, int *pb, int style);
extern int get_job(int colonist);
extern int get_icon(int colonist);
extern void PieceIconDraw(void *dst, int icon, int x, int y, int flags);

extern void GRPort_DrawRect(GRPort *w, RECT *r);

extern "C" int sprite_width_1010_e60f(void *sprite);

void ColonyWin_DrawIndividual(ColonyWin *w, int job, int x, int y, int slot)
{
    register int i;
    register int pic;
    int step;
    int a;
    int n;
    int sel;
    int shift;
    RECT r;

    set_colony(w->colony);
    n = job_count(job);
    step = ComputeIconRow(job + 0x52, n, n, &x, y, g_slot_span[slot], g_slot_span[slot], &a, &shift, 2);
    for (i = 0; i < g_colonies[w->colony].pop; i++) {
        if (get_job(i) == job) {
            pic = get_icon(i);
            PieceIconDraw(w, pic, w->rect.left + x, w->rect.top + y + 1, 7);
            sel = g_selected_colonist;
            if (i == sel) {
                TextColor(g_text_colour);
                SetRect(&r, w->rect.left + x - (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1) - 4, w->rect.top + y,
                        (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1) + (w->rect.left + x) + 1, w->rect.top + y + 0x20);
                GRPort_DrawRect((GRPort *)w, &r);
            }
            x += step;
        }
    }
}
