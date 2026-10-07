/* SListBox_ctor: candidate for 1050:860c (78 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SListBox::SListBox()
 * name-source: descriptive
 *
 * The constructor of SListBox: allocate 0x20 when `this` arrives null, run
 * `DlgControl::DlgControl()`, clear the long at +0x1c. `listbox_init` 78 bytes below is
 * what actually builds the control (kind 7, the MSListBox window, the word at +0x1a); this
 * only makes the object. `SListBox_dtor` at 1050:8229 releases the handle.
 *
 * WHICH CLASS: Popup_make_controls's `new SListBox` -- the single list box its flag bit 12
 * selects, handed straight to `listbox_init` -- is a call to this address (1050:36eb),
 * and its `new SCheckBox[n]` names 1050:86d6 as the vector-new constructor. The earlier
 * by-elimination argument (six constructors against six destructors, SCheckBox "left over")
 * had TWO names left over on both sides and paired them the wrong way round; the call sites
 * settle it, and the sizes agree with them per address. Layout agrees too: this constructor,
 * then `listbox_init`, then the SCheckBox constructor, then `checkbox_init` -- each class's
 * constructor directly before its Init.
 *
 * `sizeof` is `DlgControl`'s 0x1a plus six of its own: the word at +0x1a `listbox_init`
 * clears and the long at +0x1c this zeroes with one 32-bit store, which is what `-3` gives.
 *
 * RENAMED 2026-09-28 from SCheckBox_ctor: the two constructor files were swapped. tools/check_data_refs.py: Popup_make_controls's `new SListBox` (the bit-12 single list box, 0x20 bytes, handed to listbox_init) calls 1050:860c, and its `new SCheckBox[n]` (the bit-2 check-box array, 0x22-byte elements, handed to checkbox_init) names 1050:86d6 as the vector-new constructor. The sizes agree per address: 860c allocates 0x20 = DlgControl 0x1a + the word at +0x1a listbox_init clears + the long at +0x1c this constructor zeroes; 86d6 allocates 0x22 = DlgControl + on_change, checked, enabled, exactly the three fields checkbox_init writes. Layout agrees too: 860c, listbox_init (865a), 86d6, checkbox_init (8718) -- each class's constructor directly before its Init. The old by-elimination argument had two names left over on both sides and picked the wrong pairing
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

SListBox::SListBox()
{
    f1c = 0;
}
