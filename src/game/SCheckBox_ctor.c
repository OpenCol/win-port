/* SCheckBox_ctor: candidate for 1050:86d6 (66 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: SCheckBox::SCheckBox()
 * name-source: sibling-build
 *
 * The constructor of SCheckBox, whose destructor at 1050:8115 (MSDisposeCheck) was already in
 * the corpus. Its whole body is the base class: allocate 0x22 when `this` arrives null, run
 * `DlgControl::DlgControl()`, count the object, hand `this` back. There is nothing of its own
 * to initialise -- `checkbox_init` 66 bytes below writes all three derived fields.
 *
 * WHICH CLASS: its address is the vector-new constructor pointer at 1050:384a inside
 * Popup_make_controls (+0x2a0), the `new SCheckBox[n]` that builds the check-box array its
 * flag bit 2 selects, each element then handed to `checkbox_init`; the single `new SListBox`
 * of that function calls 1050:860c. (The earlier header read 0x384a as a vtable.)
 *
 * The 0x22 is `sizeof(SCheckBox)`: `DlgControl`'s 0x1a plus eight bytes of its own -- the
 * `on_change` long at +0x1a, `checked` at +0x1e and `enabled` at +0x20 that `checkbox_init`
 * sets. Nothing in this function names them.
 *
 * RENAMED 2026-09-28 from SListBox_ctor: the two constructor files were swapped. tools/check_data_refs.py: Popup_make_controls's `new SCheckBox[n]` (the check-box array, 0x22-byte elements, handed to checkbox_init) names 1050:86d6 as the vector-new constructor -- the reference at SEG11:0x384a the old header called a vtable is that constructor pointer inside Popup_make_controls (1050:35aa+0x2a0) -- and its `new SListBox` calls 1050:860c. 0x22 = DlgControl 0x1a + on_change, checked, enabled, the three fields checkbox_init (66 bytes below) writes; the list box is the 0x20-byte class at 860c
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* Defined here, not merely declared: rule 47 covers the calls the compiler writes,
 * so the base constructor is reached through the four-byte `PUSH CS / CALL NEAR` the
 * shipped has rather than a five-byte call the comparator has to collapse. */

SCheckBox::SCheckBox()
{
}
