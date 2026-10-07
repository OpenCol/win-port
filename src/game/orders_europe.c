/* orders_europe: candidate for 1048:a37e (63 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * With bit 0 of the byte at SEG20:0x7784 set, shows the EUROPENOTLEAVE text; otherwise
 * publishes the selected unit's position and sends the unit at SEG20:0x7794 through
 * tell_unit_to_europe.
 *
 * RENAMED 2026-09-27 from return_to_europe: the Mac build's orders_europe, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: The Europe order: shows EUROPENOTLEAVE after independence, otherwise send_unit_to_europe; Mac orders_europe calls a text dialog then send_unit_to_europe from MainMenuProc (case 413); 0 args, ratio 1.46; a sibling of orders_route.
 */

#include "colonization.h"

#include "gameflag.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void send_unit_to_europe(void);
extern void tell_unit_to_europe(int u);

void orders_europe(void)
{
    if (g_game.game_flags & GAMEFLAG_INDEPENDENCE) {
        ask_text_resource("EUROPENOTLEAVE");
    } else {
        send_unit_to_europe();
        tell_unit_to_europe(g_game.selected_unit);
    }
}
