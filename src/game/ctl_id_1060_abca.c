/* ctl_id_1060_abca: candidate for 1060:abca (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 * size-source: 25, not the census 21. The trailing 4 bytes are the dead second epilogue.
 *
 * The control's ID: the `int` at +0x2. A one-line accessor, 20-odd bytes of prologue and
 * epilogue around a single move.
 *
 * `DialogWin_RemoveControl` compares it against its `id` parameter and `controls_unlink_all`
 * feeds it in; the widget layer passes it as the second argument of every
 * `call_handler_slot_*`. It is `DlgControl`'s `id` field, which the initialisers fill
 * from ids like 0xc8 and `k + 0xc8`. +0x0 of the same record is `ctl_kind_*`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int ctl_id_1060_abca(DlgControl *ctl)
{
    return ctl->id;
}
