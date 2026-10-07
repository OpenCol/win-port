/* Popup_item_x_size: candidate for 1050:0aac (42 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * A switch on the record's first word with only a default arm: returns 32.
 *
 * RENAMED 2026-09-26 from default_width: the Mac build's Popup::item_x_size(void): this only, the item width -- 0x20 whatever the kind -- which Popup_item_y_size takes as its starting bound. It sits in the Popup module (1050) and the width matches.
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

int Popup_item_x_size(Popup *popup)
{
    int w;

    switch ((int)popup->win) {
    default:
        w = 0x20;
        break;
    }
    return w;
}
