/* set_nation: candidate for 1030:0842 (130 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: set_nation
 * name-source: sibling-build
 *
 * Runs the SetNations dialog and stores the chosen nation, less one, at SEG20:0x779a, the
 * player's own nation (`g_game.player_nation`), which `liberty_declared` and `get_name`
 * both read.
 * Returns 1 when the dialog came back with 0 (nothing chosen) and 0 otherwise, so the
 * return is "cancelled", not "chosen".
 *
 * The dialog's answer is a BYTE widened on the way in (`MOV AH,0` after the call) and
 * narrowed again on the way out (`ADD AL,0xff` then `MOV AH,0` for the minus one), so the
 * subtraction happens in 8 bits and wraps rather than going negative.
 *
 * THE OBJECT IS DECLARED LAST (§130). `SetNations` sits at the top of the frame at bp-0x158
 * and the answer word below it at bp-0x178, which is the order Borland gives when every
 * scalar is declared before the object.
 *
 * The return value is carried across the destructor by `PUSH AX` / `POP AX`, and the unwind
 * restore is `MOV AX` -- so the exit falls off the end rather than being an explicit
 * `return` (rule 41).
 *
 * The class is `SetNations_dtor`'s, unchanged.
 *
 * RENAMED 2026-09-27 from choose_nation: LEADS3 (verified by another agent, accepted; checked for conflicts with this batch: none). LEAD 1, the twin of 116a. Call graph: Mac set_nation(void) is called only by get_game_options, and 0842 only by 1008:bd72 get_game_options, second of the pair on both sides. Callees: Mac seq is EH-init, inlined SetNations ctor, ClearEventTable, SetNations::Init, EH-exit; 0842 calls SetNations_ctor, SetNations_Init and SetNations_dtor inside __InitExceptBlock. Arity: (void) on both sides. Body: it runs SetNations and stores the answer less one in g_game.player_nation. Layout: Mac CODE 17 runs SetNations OkButtonClick, LeftMouseClick, DrawIt, Init, TrackMouse, DoMessage, then set_nation at 0x3fbc. Windows 1030 runs SetNations_OkButtonClick (0000) .. SetNations_DoMessage (063b), then 0842. Size: 870 Mac body (ctor inlined) against 130. Mac set_nation and set_difficulty are the same 890-byte span, just as 0842 and 116a are 130 and 128 bytes. No strings. No collision; the four earlier token-similarity rejections of 'set_nation' were other functions. Also update tools/check_call_forms.py KNOWN ('choose_nation.c').
 * SetNations::SetNations, SetNations_Init and SetNations::~SetNations are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;

extern "C" extern "C" int SetNations_Init(SetNations *s);
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern "C" int set_nation(void)
{
    register int cancelled;
    int answer;
    SetNations sn;

    cancelled = 0;
    answer = (unsigned char)SetNations_Init(&sn);
    if (answer == 0)
        cancelled = 1;
    else
        g_game.player_nation = (unsigned char)(answer - 1);
    return cancelled;
}
