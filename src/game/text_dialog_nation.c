/* text_dialog_nation: candidate for 1050:655f (44 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * One of five convenience entries onto the segment's TEXT dialog. Each presets ONE of the
 * three selector globals the popup reads -- g_popup_tribe (DGROUP:0x2518), g_popup_dude (0x251a)
 * and g_popup_nation (0x251c) -- and then calls `text_dialog_1050_8e85(g_game_txt, name, a, 0)`.
 * `Popup_compute_size` is what consumes them: `Sprite_Init(p->portrait, g_popup_nation + 0xdc)` and the
 * matching g_popup_dude case pick the picture at the top of the popup, and `Popup_exec`
 * resets all three to -1.
 * This one passes the caller's `a` through; `text_dialog_sel_b0` is the same store with a
 * literal 0 there instead.
 *
 * `text_dialog_1050_8e85` is at 1050:8e85, the SAME segment, so the shipped call is the
 * four-byte `push cs; call ` and rule 47 makes it an in-file definition, not an extern.
 *
 * RENAMED 2026-09-27 from text_dialog_sel_b: it presets g_popup_nation, the European nation whose leader portrait tops the popup, before opening the dialog; its siblings are popk and text_dialog_dude.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

extern int g_popup_nation;      /* DGROUP:0x251c */

int text_dialog_nation(void *name, int sel, int nation)
{
    g_popup_nation = nation;
    return text_dialog_1050_8e85(g_game_txt, name, sel, 0);
}
