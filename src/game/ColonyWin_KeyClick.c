/* ColonyWin_KeyClick: candidate for 1010:4ccd (771 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_KeyClick
 *
 * A key pressed in the colony window, guarded against re-entry by the word at DGROUP:0x9fa and
 * ignored unless the colony screen is open (DGROUP:0xc1c). While DGROUP:0xa08 is set only 0xd2
 * does anything. Otherwise:
 *
 *   0xd2      leave the screen (ColonyWin_OnOKClick)   0xd0  open the colonist's profession popup
 *   N         toggle DGROUP:0xc0e and run `colony_change_selected`
 *   L / U     load the best goods onto, or run 1028:b728 on, the selected carrier in port
 *   M         step the colony display mode 0 -> 1 -> 2 -> 0
 *   1, 2, 3   set the display mode to 0, 1 or 2
 *   B / C     switch to mode 2, then pass the window to 1010:7388 (B) or 1010:6f3a (C)
 *
 * Every change of mode also keeps a copy at DGROUP:0xc10, moves DGROUP:0xc06 off 3 unless the new
 * mode is 1, and redraws the window. The cases are laid out in the source's order, not in value
 * order (rule 36).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

extern void ColonyWin_OnOKClick(int a);
extern void colony_select_profession(int unit_roles_only);
extern int nth_cargo_vessel(int carrier);
extern int perform_orders_load(int u);
extern int perform_orders_unload(int u, int flag);
extern void ColonyWin_colony_hammer_buy(ColonyWin *w, int a);
extern void ColonyWin_colony_hammer_change(ColonyWin *w, int a);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void colony_change_selected(void);
extern "C" void colony_change_mode(void);

void ColonyWin_KeyClick(int key)
{
    ColonyWin *o;
    EventWin *w;

    if (g_colony_key_busy != 0)
        return;
    g_colony_key_busy = 1;
    if (g_open_flag == 0)
        goto done;
    w = (EventWin *)GetEventWindow();
    o = (ColonyWin *)w;
    if (g_scout_visit != 0) {
        if (key == 0xd2)
            ColonyWin_OnOKClick(0);
    } else {
        switch (key) {
        case 0xd2:
            ColonyWin_OnOKClick(0);
            break;
        case 0xd0:
            colony_select_profession(0);
            break;
        case 'N':
            g_show_icon_counts = !g_show_icon_counts;
            colony_change_selected();
            break;
        case 'L':
            if (g_carrier_count != 0) {
                perform_orders_load(nth_cargo_vessel(g_selected_carrier));
                colony_change_mode();
            }
            break;
        case 'M':
            g_colony_mode = g_chosen_colony_mode = (g_colony_mode + 1) % 3;
            if (g_panel_mode == 3 && g_colony_mode != 1)
                g_panel_mode = 1;
            colony_change_mode();
            break;
        case 'U':
            if (g_carrier_count != 0) {
                perform_orders_unload(nth_cargo_vessel(g_selected_carrier), 0);
                colony_change_mode();
            }
            break;
        case '1':
            g_colony_mode = g_chosen_colony_mode = 0;
            if (g_panel_mode == 3 && g_colony_mode != 1)
                g_panel_mode = 1;
            colony_change_mode();
            break;
        case '2':
            g_colony_mode = g_chosen_colony_mode = 1;
            if (g_panel_mode == 3 && g_colony_mode != 1)
                g_panel_mode = 1;
            colony_change_mode();
            break;
        case '3':
            g_colony_mode = g_chosen_colony_mode = 2;
            if (g_panel_mode == 3 && g_colony_mode != 1)
                g_panel_mode = 1;
            colony_change_mode();
            break;
        case 'B':
        case 'C':
            if (g_colony_mode != 2) {
                g_colony_mode = 2;
                if (g_panel_mode == 3)
                    g_panel_mode = 1;
                colony_change_mode();
            }
            if (key == 'B')
                ColonyWin_colony_hammer_buy(o, 0);
            else
                ColonyWin_colony_hammer_change(o, 0);
            break;
        default:
            break;
        }
    }
done:
    MSDoAllTasks_1010_e5bd();
    g_colony_key_busy = 0;
}
