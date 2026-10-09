/* PORT: a member reached by the game's byte offset is reached by name or by its class sizes (PORT_EVENT_TABLE, PORT_SIZEOF_*): the offsets move on 64-bit Windows (W9). */
/* RedrawSpriteSquare: candidate for 1050:2f30 (315 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * TAKES THE HIGHLIGHT OFF THE LIST'S CURRENT ROW. `sprite_click` and `key_click`
 * both call it before they move the selection, and `DrawSpriteSquare` puts the highlight on the new row
 * afterwards.
 *
 * The current item's rectangle is copied out, grown by one pixel, and the same rectangle -- moved to
 * the origin -- is blitted back from the dialog's own backing port at `(char *)g_cur_dialog + 8` over
 * the dialog's window. That restores whatever was under the highlight. The row control itself is then
 * redrawn, and so is the one above it when there is a row above (`result > 1` and the item has a `prev`)
 * and the one below when the item has a `next`.
 *
 * WHAT THE BYTES SETTLED: the two rows above are addressed as `&g_cur_dialog->regions[g_cur_dialog->result - 1]` and
 * `[... - 2]`, the row below as `g_cur_dialog->regions + g_cur_dialog->result`. All of them fold to the same arithmetic,
 * but a subscript evaluates the index first and re-loads the object pointer, while the pointer
 * addition evaluates the pointer first and pushes `ES:BX` around the index -- three bytes apart, and
 * the only thing that distinguishes them (rule 79). The item's links are at +0x14 and +0x18 with the
 * rectangle at +8, so four bytes of the record sit unused between them.
 *
 * RENAMED 2026-09-27 from list_unhighlight: BEYOND THE PROPOSALS. Mac RedrawSpriteSquare(void), CODE 19 0x37dc, 350 bytes. Its seq is GRPort_CopyToPort, then MSShowControl/MSDrawControl x3, and its bsr callers are sprite_click and key_click. 2f30 (void), 315 bytes, calls GRPort_CopyToPort and control_show x3; its callers are list_click and list_key x2. Layout: directly after key_click on both.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
extern Popup *g_cur_dialog;                 /* DGROUP:0x2500 */

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void MSShowControl_1050_8459(DlgControl *control);

void RedrawSpriteSquare(void)
{
    RECT r;
    RECT r2;

    r = g_cur_dialog->cur_sprite->rect;
    InflateRect(&r, 1, 1);
    r2 = r;
    OffsetRect(&r2, -r2.left, -r2.top);
    GRPort_CopyToPort(&g_cur_dialog->port, g_cur_dialog->win, &r2, &r);
    MSShowControl_1050_8459((DlgControl *)(&g_cur_dialog->regions[g_cur_dialog->result - 1]));
    if (g_cur_dialog->result > 1 && g_cur_dialog->cur_sprite->prev != 0)
        MSShowControl_1050_8459((DlgControl *)(&g_cur_dialog->regions[g_cur_dialog->result - 2]));
    if (g_cur_dialog->cur_sprite->next != 0)
        MSShowControl_1050_8459((DlgControl *)(g_cur_dialog->regions + g_cur_dialog->result));
}
