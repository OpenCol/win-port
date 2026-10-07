/* colony_cursor_update: candidate for 1010:bfa6 (168 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: colony_cursor_update
 *
 * Flips the toggle at DGROUP:0xc0c and re-arms one panel state (argument 1)
 * chosen by the panel word at DGROUP:0xc06 -- two of them only when their
 * lock bytes (DGROUP:0xa0f, 0xa0d) are clear.  The trailing test of mode 2
 * against the colony's 0x80 flag has an empty body in the shipped code.
 * The five setters are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_toggle_panel: the Mac build's colony_cursor_update(void), bound by call-sequence alignment (1.0 aligned gaps in colony_select_profession leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.00x this one.
 */

#include "colonization.h"

#include "colflag.h"
#include "records.h"

extern int g_stub_a;

extern void draw_colonists(int a);
extern void draw_colony_map(int a);
extern void draw_market(int a);
extern void draw_miscellaneous(int a);
extern void draw_colony_ships(int a);

void colony_cursor_update(void)
{
    g_panel_toggle = !g_panel_toggle;
    switch (g_panel_mode) {
    case 1:
        draw_colonists(1);
        break;
    case 0:
        if (g_st_c == 0)
            draw_colony_map(1);
        break;
    case 4:
        if (g_st_b == 0)
            draw_market(1);
        break;
    case 3:
        draw_miscellaneous(1);
        break;
    case 2:
        draw_colony_ships(1);
        break;
    }
    if (g_colony_mode == 2 && (g_cur_colony_rec->status & COLONY_BUILD_DONE) != 0)
        ;
}
