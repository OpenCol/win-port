/* possible_popup: candidate for 1018:8cc0 (381 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: possible_popup
 *
 * The colony announcer. `produce_something` and `colony_depletion` both call it and both
 * thread the same flag byte through it: passed in NEGATED as `ask` and ORed back out of the
 * return. So the two extra choices are offered only while that byte is clear, and picking
 * event 3 sets it -- the "do not tell me again" of a message the game would otherwise repeat
 * every turn.
 *
 * Nothing happens at all unless the current colony is the player's (DGROUP:0x4baa). The map
 * is scrolled to the message's square first, and x == 0 means "use the colony's own
 * position" rather than a real coordinate of zero. The colony's NAME goes into text slot 0,
 * `mode` goes into DGROUP:0x251a for the duration and is reset to -1 on the way out --
 * including on the early exit, which is why that exit is a `goto` to a label before the
 * reset rather than a `return`.
 *
 * `remember` is stored at DGROUP:0xc0f when the player suppresses the message, and only when
 * it is not negative; both callers pass a real id or -1.
 *
 * Three spellings the bytes fixed, all of them about WHERE something happens rather than
 * what it is.
 *
 * `x` and `y` are `register` PARAMETERS: the shipped prologue loads them into SI and DI
 * BEFORE `__InitExceptBlock_rtl`, which is the parameter-copy prologue, where locals copied
 * from parameters would be loaded after the exception block is set up. Rule 40's "takes it
 * from the parameter", seen from the other side.
 *
 * `r` is initialised in its DECLARATION, not assigned in the body: the shipped code writes
 * its zero before the `6` that sets up the exception frame, so it happens before the Popup
 * is constructed.
 *
 * And the early exit is the two-jump form (rule 12), so `goto out` and not the whole body
 * wrapped in a positive `if`.
 *
 * RENAMED 2026-09-26 from colony_event_message: the Mac build's possible_popup(char*, short, short, short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller new_turn_colony; callee item; callee sound_play; callee strings), the runner-up gets 2, and the arity agrees (7).
 */

#include "colonization.h"

#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern char *strings(int n);
extern "C" int possibly_center(int x1, int y1, int x2, int y2, int mode);   /* PORT: C linkage, as defined (W2) */
extern "C" void sound_play(int n);   /* PORT: C linkage, as defined (W2) */

char possible_popup(char *msg, int ask, int remember, register int x,
                              register int y, int mode, int sound)
{
    int r = 0;
    int ev;
    Popup p(0x800);

    if (!g_cur_colony_is_players)
        goto out;
    if (g_colony_window_open == 0) {
        if (x == 0) {
            x = g_cur_colony_rec->x;
            y = g_cur_colony_rec->y;
        }
        possibly_center(x, y, x, y, 0);
    }
    set_name_slot_1018_b3f6(0, g_cur_colony_rec->name);
    g_popup_dude = mode;
    show_plain_dialog_1018_b41f(&p, msg, 0);
    if (ask != 0 && g_colony_window_open == 0) {
        Popup_item(&p, strings(g_misc_words[MISC_CONTINUE_TURN]), 1);
        Popup_item(&p, strings(g_misc_words[MISC_ZOOM_TO_COLONY]), 2);
    }
    if (sound > 0)
        sound_play(sound);
    ev = popup_run_thunk_1018_b44f(&p);
    if (ev == 3) {
        r = 1;
        if (remember >= 0)
            g_colony_mode = (char)remember;
    }
out:
    g_popup_dude = -1;
    return (char)r;
}

