/* ColonyWin_DrawFort: candidate for 1010:9455 (472 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_DrawFort
 * name-source: sibling-build
 *
 * Draws the colonists standing OUTSIDE the colony -- the ones on the colony's own map square,
 * walked through the tile's unit chain rather than through the colony record. It is
 * `ColonyWin_DrawIndividual` written against a different source of colonists: the same
 * `ComputeIconRow` spacing from the SEG21:0x38 width, the same portraits through
 * `PieceIconDraw`, and the same half-portrait-wide frame around the selected one.
 *
 * DGROUP:0x4bb2 CAPS THE ROW. The count is incremented for every colonist on the square, and
 * one past the cap the loop stops drawing -- but keeps counting, and keeps walking the chain,
 * so the cap trims the display and not the walk.
 *
 * `x` is an in-out PARAMETER: `ComputeIconRow` may move the start through the pointer, and each
 * drawn portrait advances it by the step. The caller's copy is what moves.
 *
 * The cap test is a `goto` and not an `if`: `JNL SHORT` over a `JMP NEAR` is rule 12's two-jump
 * form, where the colonist test above it is a direct `JZ NEAR` and so a real `if` block.
 *
 * RENAMED 2026-09-26 from colony_draw_outside: the Mac build's ColonyWin::DrawFort(short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller DrawABuilding; callee DrawRect; callee TextColor; callee next_unit; callee set_colony), the runner-up gets 4, and the arity agrees (4).
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, ColonyWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-









   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

extern void set_colony(int colony);
extern int ComputeIconRow(int pic, int n, int m, int *px, int y,
                                    int w1, int w2, int *pa, int *pb, int style);
extern int top_unit_in_square(int x, int y);
extern int is_colonist(int u);
extern int guy_number_of_unit(int u);
extern int get_icon(int colonist);
extern void PieceIconDraw(void *dst, int icon, int x, int y, int flags);

extern void GRPort_DrawRect(GRPort *w, RECT *r);
extern int next_unit(int u);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int sprite_width_1010_e60f(void *sprite);

void ColonyWin_DrawFort(ColonyWin *w, int x, int y, int count)
{
    int step;
    int a;
    int slot;
    int colonist;
    int b;
    int sel;
    int n;
    RECT r;
    register int u;
    register int pic;

    n = 0;
    set_colony(w->colony);
    slot = 3;
    step = ComputeIconRow(0x67, count, count, &x, y,
                                g_slot_span[slot], g_slot_span[slot], &a, &b, 2);
    u = top_unit_in_square(g_colonies[w->colony].x, g_colonies[w->colony].y);
    while (u >= 0) {
        if (is_colonist(u) != 0) {
            n++;
            if (g_tile_colonists < n)
                goto next;
            colonist = guy_number_of_unit(u);
            pic = get_icon(colonist);
            PieceIconDraw(w, pic, w->rect.left + x, w->rect.top + y, 7);
            sel = g_selected_colonist;
            if (colonist == sel) {
                TextColor(g_text_colour);
                SetRect(&r, w->rect.left + x - (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1),
                        w->rect.top + y,
                        (sprite_width_1010_e60f(&g_icon_sprites[pic]) >> 1) + (w->rect.left + x),
                        w->rect.top + y + 0x20);
                GRPort_DrawRect((GRPort *)w, &r);
            }
            x += step;
        }
next:
        u = next_unit(u);
    }
}
