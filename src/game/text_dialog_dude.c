/* text_dialog_dude: candidate for 1050:6500 (44 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * One of five convenience entries onto the segment's TEXT dialog. Each presets ONE of the
 * three selector globals the popup reads -- g_popup_tribe (DGROUP:0x2518), g_popup_dude (0x251a)
 * and g_popup_nation (0x251c) -- and then calls `text_dialog_1050_8e85(g_game_txt, name, a, 0)`.
 * `Popup_compute_size` is what consumes them: `Sprite_Init(p->portrait, g_popup_nation + 0xdc)` and the
 * matching g_popup_dude case pick the picture at the top of the popup, and `Popup_exec`
 * resets all three to -1.
 * This one presets g_popup_dude rather than either g_sel_ global.
 *
 * `text_dialog_1050_8e85` is at 1050:8e85, the SAME segment, so the shipped call is the
 * four-byte `push cs; call ` and rule 47 makes it an in-file definition, not an extern.
 *
 * RENAMED 2026-09-27 from text_dialog_value: it presets g_popup_dude (DGROUP:0x251a), the popup's top picture (Popup_compute_size: Sprite_Init(spr_top, g_popup_dude + 0xd2)), before opening the dialog, like its text_dialog_sel_* siblings.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

int text_dialog_dude(void *name, int sel, int value)
{
    g_popup_dude = value;
    return text_dialog_1050_8e85(g_game_txt, name, sel, 0);
}
