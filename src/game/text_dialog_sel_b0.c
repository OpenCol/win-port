/* text_dialog_sel_b0: candidate for 1050:6530 (43 bytes).  Verdict: RELOCATABLE.
 *
 * One of five convenience entries onto the segment's TEXT dialog. Each presets ONE of the
 * three selector globals the popup reads -- g_popup_tribe (DGROUP:0x2518), g_popup_dude (0x251a)
 * and g_popup_nation (0x251c) -- and then calls `text_dialog_1050_8e85(g_game_txt, name, a, 0)`.
 * `Popup_compute_size` is what consumes them: `Sprite_Init(p->portrait, g_popup_nation + 0xdc)` and the
 * matching g_popup_dude case pick the picture at the top of the popup, and `Popup_exec`
 * resets all three to -1.
 * This one passes a literal 0 for `a`, which is why it has one parameter fewer than
 * `text_dialog_nation`.
 *
 * `text_dialog_1050_8e85` is at 1050:8e85, the SAME segment, so the shipped call is the
 * four-byte `push cs; call ` and rule 47 makes it an in-file definition, not an extern.
 *
 * NOT RENAMED: the Mac build's popf(char*, short) by call-sequence alignment (4 aligned gaps, none against), beside popm and popi -- but HELD under this name: 'popf' is an x86 mnemonic, so nasm would sanitise the label to popf_ (as FBSTP_), and check_findings keeps that ceiling at one. tools/mac_bind.py anchors it through HELD.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

extern int g_popup_nation;      /* DGROUP:0x251c */

int text_dialog_sel_b0(void *name, int sel)
{
    g_popup_nation = sel;
    return text_dialog_1050_8e85(g_game_txt, name, 0, 0);
}
