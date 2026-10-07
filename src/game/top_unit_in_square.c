/* top_unit_in_square: candidate for 1038:1918 (234 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The unit at (x, y), followed to the end of its chain: -1 when none, or when the square is
 * on the map but nobody owns it. A square with an owner but no unit reports the position
 * and owner through UNITFLAG.
 *
 * RENAMED 2026-09-26 from unit_at: the Mac build's top_unit_in_square(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 10 of its Mac call edges already bound land on this function (caller DrawColonyMap; caller DrawColonyUnits; caller DrawInbound; caller DrawOutBound; caller DrawUnits), the runner-up gets 8, and the arity agrees (2).
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern int is_unit(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int top_unit(int u);

extern void popup_say(int slot, int word);

int top_unit_in_square(int x, int y)
{
    int i;
    int u = -1;
    int onmap;

    onmap = on_map(x, y);
    if (onmap != 0 && is_unit(x, y) < 0)
        goto done;
    for (i = 0; u < 0 && g_game.unit_count > i; i++)
        if (g_units[i].x == (unsigned char)x && g_units[i].y == (unsigned char)y)
            u = i;
    u = top_unit(u);
    if (u < 0 && onmap != 0) {
        set_text_arg_i_1038_86cc(0, x);
        set_text_arg_i_1038_86cc(1, y);
        popup_say(0, get_adjective(is_unit(x, y)));
        show_text_dialog_1038_865b("UNITFLAG");
    }
done:
    return u;
}
