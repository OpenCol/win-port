/* PORT: this file's own declarations of Popup_find_item removed: the header declares it from the definition (W1 step 3). */
/* Popup_set_active: candidate for 1050:0fad (43 bytes).  Verdict: RELOCATABLE.
 * symbol: Popup_set_active
 * name-source: sibling-build
 *
 * Stores what Popup_find_item finds for a in the pointer at +0x366
 * of the record.  The finder is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from store_entry_ptr: the Mac build's Popup::set_active(int): this plus an item key; it looks the item up (Popup_find_item) and stores it as the popup's selected item (+0x366), and every caller does that just before running the popup to preselect the current choice -- the colonist's job, the build item, the trade-route stop. The width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern char g_stub_s[];


void Popup_set_active(Popup *popup, int key)
{
    popup->sel_item = Popup_find_item(popup, key);
}
