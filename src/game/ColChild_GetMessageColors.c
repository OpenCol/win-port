/* ColChild_GetMessageColors: candidate for 1048:1059 (109 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColChild_GetMessageColors
 * name-source: sibling-build
 *
 * Fills two out-parameters with the origin of one of four fixed layouts, selected by an index that falls back to layout 0.

 * SIZE: the census says 101 and the function is 109. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-27 from layout_origin: CONST PROPOSAL. Mac ColChild::GetMessageColors(short, short*, short*), CODE 30 0x6644, 120 bytes, called only by DrawAnyMessage. 1059 (void *unused_this, int sel, int *a, int *b) is called only by ColChild_DrawAnyMessage. Leaf on both; it is a switch storing two words through the pointers. Layout: Mac MinButton, DrawAll, DrawTitleBar, GetMessageColors, CancelMessage; Windows ColChild_MinButton, ColChild_DrawAll, ColChild_DrawTitleBar, 1059, ColChild_CancelMessage. Size 120 against 109. tools/check_jump_tables.py:30 mentions layout_origin in prose only.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
void ColChild_GetMessageColors(ColChild *unused, int sel, int *a, int *b)
{
    switch (sel) {
    case 1:
        *a = 0x95;
        *b = 0x22;
        break;
    case 2:
        *a = 0x95;
        *b = 0x22;
        break;
    case 3:
        *a = 0xc;
        *b = 0x22;
        break;
    case 0:
    default:
        *a = 0x44;
        *b = 0x22;
        break;
    }
}
