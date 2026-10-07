/* edit_key_allowed: candidate for 1070:9aa0 (52 bytes).  Verdict: EXACT.
 *
 * Returns what the handler at +0x1e gives for (table word +2, a), or 1 when no
 * handler is set.
 *
 * RENAMED 2026-09-26 from call_handler_slot_30_or_1: asks the edit box's +0x1e key filter whether a key is
 * accepted, passing its id and the translated key; with no filter installed every key is (1).
 * editbox_subclass_proc gates WM_KEYDOWN, WM_CHAR and WM_KEYUP on it.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: edit_key_allowed
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int edit_key_allowed(SEditBox *table, int key)
{
    if (table->key_filter != 0)
        return ((int (*)(int, int))table->key_filter)(table->id, key);
    else
        return 1;
}
