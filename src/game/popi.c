/* popi: candidate for 1050:6443 (43 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The sixth member of segment 1050's text-dialog family (1050:6443 .. 655f): presets `g_popup_tribe`
 * (DGROUP:0x2518) from its second argument and shows the TEXT dialog for `name` with `a` zero,
 * the g_popup_tribe counterpart of `text_dialog_sel_b0`. `Popup_compute_size` reads the selector to pick the
 * picture at the top of the popup.
 *
 * The three selector globals are NEAR in this module. `text_dialog_1050_8e85` is in the same
 * segment and reached by the four-byte form, so it is defined here (rule 47).
 *
 * RENAMED 2026-09-26 from tribe_message_show: the Mac build's popi(char*, short), bound by call-graph agreement (tools/mac_callgraph.py): 11 of its Mac call edges already bound land on this function (caller combat_fight; caller negotiate_tribe; caller orders_clear; caller orders_road; caller tribe_camp), the runner-up gets 10, and the arity agrees (2). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

extern int g_popup_tribe;      /* DGROUP:0x2518 */

int popi(void *name, int sel)
{
    g_popup_tribe = sel;
    return text_dialog_1050_8e85(g_game_txt, name, 0, 0);
}
