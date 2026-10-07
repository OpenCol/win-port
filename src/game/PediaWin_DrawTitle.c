/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 51 of the imported file. */
/* PediaWin_DrawTitle: candidate for 1060:203f (215 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Colonizopedia's prompt line: build "Choose a '<kind>' Entry:" from the window's kind
 * word at +0x84a and draw it centred the top.
 *
 * The six kinds are the pedia's six sections, and the literals are read out of DGROUP rather
 * than guessed (§119): 'Cargo', 'Unit', 'Terrain', 'Job skill', 'Building', 'Founding
 * Father'. Each piece keeps the closing quote and the trailing space that the layout needs,
 * so the pieces concatenate without any separator logic -- which is why this is six `strcat`
 * calls and not a table lookup.
 *
 * Dense 0..5, so Borland emits a jump table and puts it in the CODE segment past the last
 * instruction: 215 bytes against a census of 203, the twelve extra being six two-byte
 * entries.
 *
 * The x is `port_width_1060_6886(o) / 2` -- the window's width halved, the shipped `CWD /
 * SUB AX,DX / SAR AX,1` being the signed divide -- and the y the window's own +0x76c plus
 * 0x14. `TextColor(0xc)` is not put back afterwards.
 *
 * The three message-buffer helpers and `port_width_1060_6886` are -called and are stubs here (rule 47); `TextColor`
 * and `GRPort_String` are calls into segment 1068 and are `extern`.
 *
 * RENAMED 2026-09-26 from pedia_prompt: the Mac build's PediaWin::DrawTitle(void): this only, the "Choose a '<kind>' Entry:" line centred at the top. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_String(GRPort *port, unsigned *hf, const char *s,
                                     int x, int y, int align);

/* same object: 4-byte `push cs; call ` */

void PediaWin_DrawTitle(PediaWin *o)
{
    message_buf_set_1060_682a("Choose a '");
    switch (o->kind) {
    case 0: message_buf_cat_1060_684c("Cargo' "); break;
    case 1: message_buf_cat_1060_684c("Unit' "); break;
    case 2: message_buf_cat_1060_684c("Terrain' "); break;
    case 3: message_buf_cat_1060_684c("Job skill' "); break;
    case 4: message_buf_cat_1060_684c("Building' "); break;
    case 5: message_buf_cat_1060_684c("Founding Father' "); break;
    }
    message_buf_cat_1060_684c("Entry:");
    TextColor(0xc);
    GRPort_String((GRPort *)o, (unsigned *)&g_font_small, message_buf_1060_686e(),
                         port_width_1060_6886((GRPort *)(o)) / 2, o->rect.top + 0x14, 0);
}
