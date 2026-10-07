/* PORT: this file's own declarations of MSDrawText removed: the header declares it from the definition (W1 step 3). */
/* draw_text_curfont: candidate for 1068:5b30 (96 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: draw_text_curfont
 * name-source: descriptive
 *
 * `MSDrawText` with the CURRENT font -- the pointer at DGROUP:0x2ac6, whose null is
 * refused with "Error: No current font selected" and its own epilogue. The `GRPort_Text` pair
 * at 1068:5b30 and 5b90 is the `draw_string` family's plain sibling: the same two ways of
 * getting a font, and no shadow pass.
 *
 * The null test is a 32-bit compare of the whole pointer, not of its offset.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void report_error_1068_963d(void *ctx, char *msg);

void draw_text_curfont(GRPort *port, const char *s, RECT *r, int flags)
{
    if (g_current_font == 0) {
        report_error_1068_963d(&g_error_log, "Error: No current font selected");
        return;
    }
    MSDrawText((GrafRec *)port->win, g_current_font->hfont, s, r, flags);
}
