/* PORT: this file's own declarations of MSDrawText removed: the header declares it from the definition (W1 step 3). */
/* GRPort_Text: candidate for 1068:5b90 (54 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_Text
 * name-source: sibling-build
 *
 * `draw_text_curfont`'s twin with the font as a PARAMETER: pass the port's window, the font
 * handle from the caller's record, and everything else straight through to `MSDrawText`.
 * There is no null check here, because there is no global to be null.
 *
 * The font argument is a pointer to a two-word record and only its first word is used,
 * which is the same shape `GRPort_String` reads its font through.
 *
 * RENAMED 2026-09-26 from draw_text: the Mac build's GRPort::Text(Font&, char*, Rect*, short): this plus a font, a string, a rectangle and flags, in that order. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"


void GRPort_Text(GRPort *p, unsigned *hf, const char *s,
                             RECT *r, int flags)
{
    MSDrawText((GrafRec *)p->win, *hf, s, r, flags);
}
