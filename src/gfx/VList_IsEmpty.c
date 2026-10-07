/* VList_IsEmpty: candidate for 1068:9dbb (35 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: VList_IsEmpty
 * name-source: sibling-build
 *
 * Whether the pointer the record starts with is null.
 *
 * RENAMED 2026-09-26 from ptr_is_null: the Mac build's VList::IsEmpty(void): this only, whether the list's first pointer is null; VList_Add, VList_Remove and VList_insert ask it of their list. The width matches.
 */

#include "colonization.h"

#include "classes.h"

int VList_IsEmpty(VList *list)
{
    return list->first == 0;
}
