/* text_dialog_tribe: candidate for 1050:6472 (48 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * One of five convenience entries onto the segment's TEXT dialog. Each presets ONE of the
 * three selector globals the popup reads -- g_popup_tribe (DGROUP:0x2518), g_popup_dude (0x251a)
 * and g_popup_nation (0x251c) -- and then calls `text_dialog_1050_8e85(g_game_txt, name, a, 0)`.
 * `Popup_compute_size` is what consumes them: `Sprite_Init(p->portrait, g_popup_nation + 0xdc)` and the
 * matching g_popup_dude case pick the picture at the top of the popup, and `Popup_exec`
 * resets all three to -1.
 * This one takes the selector from its third argument and passes the caller's `a` through.
 *
 * `text_dialog_1050_8e85` is at 1050:8e85, the SAME segment, so the shipped call is the
 * four-byte `push cs; call ` and rule 47 makes it an in-file definition, not an extern.
 *
 * RENAMED 2026-09-27 from text_dialog_sel_c: it presets g_popup_tribe, the tribe (0..7) whose leader tops the popup, drawn by its anger with the player; 8 is popk.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

extern int g_popup_tribe;      /* DGROUP:0x2518 */

int text_dialog_tribe(void *name, int sel, int tribe)
{
    g_popup_tribe = tribe;
    return text_dialog_1050_8e85(g_game_txt, name, sel, 0);
}
