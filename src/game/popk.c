/* popk: candidate for 1050:64a2 (43 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * One of five convenience entries onto the segment's TEXT dialog. Each presets ONE of the
 * three selector globals the popup reads -- g_popup_tribe (DGROUP:0x2518), g_popup_dude (0x251a)
 * and g_popup_nation (0x251c) -- and then calls `text_dialog_1050_8e85(g_game_txt, name, a, 0)`.
 * `Popup_compute_size` is what consumes them: `Sprite_Init(p->portrait, g_popup_nation + 0xdc)` and the
 * matching g_popup_dude case pick the picture at the top of the popup, and `Popup_exec`
 * resets all three to -1.
 * This one hard-codes the selector to 8 and takes only the name.
 *
 * `text_dialog_1050_8e85` is at 1050:8e85, the SAME segment, so the shipped call is the
 * four-byte `push cs; call ` and rule 47 makes it an in-file definition, not an extern.
 *
 * RENAMED 2026-09-27 from text_dialog_sel_c8: it presets g_popup_tribe = 8 -- Popup_compute_size's one fixed portrait frame, the King's (raise_taxes sets 8 before KINGTAX/KINGRAISE/KINGLOWER) -- and its callers open KINGNEWWAR and LOSENOCOLONIES.
 *
 * RENAMED 2026-09-27 from text_dialog_king: the Mac build's popk, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Presets the King's portrait (g_popup_tribe = 8) and runs the text dialog on a name. Mac popk(char*) (popup-King, beside popm) is called by end_of_game_check and europe_new_wars, and the Windows callers open LOSENOCOLONIES and KINGNEWWAR; 1 arg.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

extern int g_popup_tribe;      /* DGROUP:0x2518 */

int popk(void *name)
{
    g_popup_tribe = 8;
    return text_dialog_1050_8e85(g_game_txt, name, 0, 0);
}
