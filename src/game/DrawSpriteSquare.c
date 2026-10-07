/* DrawSpriteSquare: candidate for 1050:306b (370 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * PUTS THE HIGHLIGHT ON THE LIST'S CURRENT ROW, the other half of `RedrawSpriteSquare`.
 * `sprite_click` and `key_click` call it after the selection has moved.
 *
 * Same opening as the unhighlight: the current item's rectangle is copied out and grown by one pixel.
 * Then the blit runs the OTHER WAY -- the window's contents are saved INTO the dialog's backing port
 * at `(char *)g_cur_dialog + 8`, so the unhighlight has something to put back -- and the rectangle is
 * outlined in the highlight colour at DGROUP:0x337f. The same three row controls are redrawn
 * afterwards, with the same mix of subscript and pointer addressing (rule 79).
 *
 * RENAMED 2026-09-27 from list_highlight: BEYOND THE PROPOSALS. Mac DrawSpriteSquare(void), CODE 19 0x393a, 388 bytes. Its seq is GRPort_CopyToPort, TextColor, DrawRect, then Show/DrawControl x3. Its callers are Popup::draw, and (bsr) sprite_click and key_click. 306b (void), 370 bytes, calls GRPort_CopyToPort, TextColor, GRPort_DrawRect and control_show x3; its callers are Popup_draw, list_click and list_key x2. Layout: after RedrawSpriteSquare / list_unhighlight on both.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;
typedef struct {
    void *win;                    /* +0     */
    char pad4[0x7c - 4];
    int n_items;                          /* +0x7c  */
    char pad7e[0xa8 - 0x7e];
    int result;                           /* +0xa8  */
    char padaa[0x37a - 0xaa];
    PopupSprite *sprites;                   /* +0x37a */
    PopupSprite *cur_sprite;                 /* +0x37e */
    char pad382[0x3b4 - 0x382];
    SRegion *regions;              /* +0x3b4 */
} Popup;
extern Popup *g_cur_dialog;                 /* DGROUP:0x2500 */

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

extern void GRPort_DrawRect(GRPort *p, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void MSShowControl_1050_8459(DlgControl *control);

void DrawSpriteSquare(void)
{
    RECT r;
    RECT r2;

    r = g_cur_dialog->cur_sprite->rect;
    InflateRect(&r, 1, 1);
    r2 = r;
    OffsetRect(&r2, -r2.left, -r2.top);
    GRPort_CopyToPort(g_cur_dialog->win, (GRPort *)((char *)g_cur_dialog + 8), &r, &r2);
    TextColor(g_text_colour);
    GRPort_DrawRect(g_cur_dialog->win, &g_cur_dialog->cur_sprite->rect);
    MSShowControl_1050_8459((DlgControl *)(&g_cur_dialog->regions[g_cur_dialog->result - 1]));
    if (g_cur_dialog->result > 1 && g_cur_dialog->cur_sprite->prev != 0)
        MSShowControl_1050_8459((DlgControl *)(&g_cur_dialog->regions[g_cur_dialog->result - 2]));
    if (g_cur_dialog->cur_sprite->next != 0)
        MSShowControl_1050_8459((DlgControl *)(g_cur_dialog->regions + g_cur_dialog->result));
}
