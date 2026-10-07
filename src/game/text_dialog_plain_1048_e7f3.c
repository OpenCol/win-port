/* text_dialog_plain_1048_e7f3: candidate for 1048:e7f3 (62 bytes).  Verdict: RELOCATABLE.
 *
 * Popup_start with its four long arguments zero: the plain
 * TEXT dialog call used by the colony module.
 *
 * TRANSFERRED: byte-identical to `text_dialog_plain_1010_ef3c` (1010:ef3c), one of 7 copies of this
 * function in the image. Same source, this segment's copy.
 *
 * NOT TWELVE SCALARS. The callee is
 * `(void *p, void *a, void *b, int c, void *z, void *d, void *e,
 * void *f, int g)` -- the shape `run_text_dialog` and `popup_ask_nocancel`
 * declare, the second of which pushes a real pointer into the `z` slot as two words and so
 * proves it. A forwarder that passes everything through can never show a split (§119).
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
#include "w16types.h"
#include "classes.h"
extern int Popup_start(Popup *p, void *ctx, char *name, int sel, char *dflt, GameWin *win, GRPort *port, RECT *rect, int kind);

int text_dialog_plain_1048_e7f3(void *p, void *dialog_ctx, void *name, int sel, int kind)
{
    return Popup_start((Popup *)p, dialog_ctx, (char *)name, sel, 0L, 0L, 0L, 0L, kind);
}
