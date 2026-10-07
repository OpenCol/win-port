/* ctl_id_1070_96c5: candidate for 1070:96c5 (25 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ctl_id_1070_96c5
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
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

int ctl_id_1070_96c5(DlgControl *ctl)
{
    return ctl->id;
}
