/* popup_ask_nocancel: candidate for 1050:622c (292 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: popup_ask_nocancel
 *
 * A text dialog that will not take an empty answer. It parses the directives into a stack
 * Popup of 0x800 entries, runs the dialog, and compares what came back in the buffer at
 * SEG31:0xa4 against the EMPTY STRING at DGROUP:0x25f0 -- and only a match sends it round
 * again, re-parsing from the top. Cancel (-1) and any non-empty answer both leave.
 *
 * Three globals at DGROUP:0x2518, 0x251a and 0x251c are saved on entry and restored after
 * each re-parse, before the dialog runs again, so the second pass sees the same state the first one did. They are what makes
 * the loop safe to run twice.
 *
 * `popup.flags &= ~1` TWICE, back to back, at the top of the loop. Both stores are in the
 * shipped code; only one of them does anything.
 *
 * THE FIFTH ARGUMENT OF `Popup_start` IS A POINTER, not the `long` the corpus had.
 * This function passes its own fourth parameter straight through and the shipped pushes it as
 * two words, `[BP+0x12]` then `[BP+0x10]` -- the segment first, which is a pointer push and
 * not `PUSH DWORD`. `run_text_dialog` passes a literal 0 there and so could never tell (§119).
 *
 * The three globals are reached DS-relative here where other modules need an explicit segment
 * load for the same addresses (rule 9).
 *
 * Every exit is a `goto` to the single `return` -- the first parser call and the second both
 * use rule 12's two-jump form -- but the two tests in between are DIRECT conditionals, so the
 * body after them is a positive `if` and not another `goto`. Reading that pair the other way
 * round is worth three bytes and two jumps.
 *
 * RENAMED 2026-09-26 from prompt_text_name: the Mac build's popup_ask_nocancel(char*, char*, int, char*), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller land_ho; callee exec), the runner-up gets 1, the arity agrees (4), and the Mac body is 0.98x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 * Popup::Popup, Popup_start, Popup_exec and Popup::~Popup are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern int g_popup_tribe;                    /* DGROUP:0x2518, NEAR in this module */

extern int g_popup_nation;                    /* DGROUP:0x251c */

extern "C" int Popup_start(Popup *p, void *ctx, char *name, int sel, char *dflt, GameWin *win, GRPort *port, RECT *rect, int kind);
extern "C" int Popup_exec(Popup *popup, char *buf);

extern "C" int popup_ask_nocancel(void *text_ctx, void *resource_name, int max_len, void *name)
{
    register int sel = 0;
    Popup popup(0x800);
    int s2518;
    int s251c;
    int s251a;

    s2518 = g_popup_tribe;
    s251c = g_popup_nation;
    s251a = g_popup_dude;
    if (Popup_start(&popup, text_ctx, (char *)resource_name, max_len, (char *)name, 0L, 0L, 0L, 1) != 0)
        goto done;
again:
    popup.flags &= ~1;
    popup.flags &= ~1;
    sel = Popup_exec(&popup, g_name_slots[0]);
    if (sel != -1) {
        if (STRCMP(g_name_slots[0], "") == 0) {
            if (Popup_start(&popup, text_ctx, (char *)resource_name, max_len, (char *)name, 0L, 0L, 0L, 1) != 0)
                goto done;
            g_popup_tribe = s2518;
            g_popup_nation = s251c;
            g_popup_dude = s251a;
            goto again;
        }
    }
done:
    return sel;
}
