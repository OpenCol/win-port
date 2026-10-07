/* Popup_check: candidate for 1050:13d7 (83 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 83, not the census 76. The trailing 7 bytes are the dead second epilogue.
 *
 * `Popup_item` with two extra effects: bits 0 and 2 go on in the popup's flag word at +0x86
 * before the item is added, and when `mark` is set bit 2 goes on in the new item's own flag
 * word. It returns the item, as `Popup_item` does.
 *
 * The `|= 5` is one instruction on memory -- `OR WORD [es:bx+0x86],5` -- so both bits are set
 * together rather than in two statements.
 *
 * Item and Popup layouts are `Popup_item`'s, 427 bytes above (1050:122c), unchanged.
 *
 * RENAMED 2026-09-26 from popup_add_item_marked: the Mac build's Popup::check(char*, int, int): this plus a caption, an id and the initial mark, in that order -- adding a check item. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern PopupItem *Popup_item(Popup *popup, char *s, int id);
PopupItem *Popup_check(Popup *popup, char *s, int id, int mark)
{
    PopupItem *it;

    popup->flags |= 5;
    it = Popup_item(popup, s, id);
    if (mark != 0)
        it->flags |= 4;
    return it;
}
