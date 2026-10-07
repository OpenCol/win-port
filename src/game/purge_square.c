/* purge_square: candidate for 1038:4df4 (342 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Royal Navy seizes everything on one square. Every unit at (x, y) that does not belong
 * to `g_game.mother_country_idx` is freed, walking the array from the top down so the frees do not
 * disturb the indices still to come.
 *
 * Only a unit of the HUMAN player's nation is announced -- `NATION(u) < 4 &&
 * g_nation_text[...].control == 0`, the idiom §120 settled -- and which message depends on
 * where it stood. `SEIZURESEA` needs the square to be ocean AND the unit type in 0xd..0x12,
 * the ship range; anything not on ocean gets `SEIZURELAND`. A land unit on ocean, which
 * should not happen, gets neither, and the `delete_unit` below runs for all of them
 * regardless.
 *
 * The two identifiers are read out of the image at DGROUP:0x19b2 and 0x19bd, and
 * analysis/function-anchors.json binds both to this address.
 *
 * Three spellings the bytes fixed, all of them about 8-bit arithmetic. The coordinates are
 * `unsigned char` parameters: the shipped compare is `CMP AL,[bp+6]`, where `char` or `int`
 * promotes both sides and costs six bytes more. The nation nibble masks BEFORE it promotes
 * -- `AND AL,0xF` then `MOV AH,0` -- which is the `NATION()` macro three other matched files
 * already carry, and writing `(g_units[u].flags & 0xf)` inline promotes first and masks in
 * 16 bits. Both were visible as one extra `MOV AH,0` at the front of the expression.
 *
 * All three tests are `continue` and not nested ifs: the shipped code inverts and jumps over
 * a `JMP` (rule 12). colony_depletion, matched an hour earlier, is the opposite and the two are
 * four bytes apart per test.
 *
 * RENAMED 2026-09-26 from seize_goods: the Mac build's purge_square(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller king_invasion; callee delete_unit; callee popm; callee popup_say), the runner-up gets 3, the arity agrees (2), and the Mac body is 1.08x this one's size. The margin is small, so the binding also rests on the body doing what the Mac name says.
 */

#include "colonization.h"

#include "control.h"
#include "units.h"
#include "records.h"

#define NATION(u) ((unsigned char)((u).flags & 0xf))

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
extern void popup_say(int slot, int v);
extern int terrain_is_ocean(int x, int y);
extern int popm(char *msg, int n);

extern void delete_unit(int u);

void purge_square(unsigned char x, unsigned char y)
{
    register int u;

    for (u = g_game.unit_count - 1; u >= 0; u--) {
        if (g_units[u].x != x)
            continue;
        if (g_units[u].y != y)
            continue;
        if ((char)NATION(g_units[u]) == ((char)g_game.mother_country_idx))
            continue;
        if (NATION(g_units[u]) < 4 && g_nation_text[NATION(g_units[u])].control == CONTROL_HUMAN) {
            popup_say(0, g_unit_types[g_units[u].type].name);
            if (terrain_is_ocean(g_units[u].x, g_units[u].y)) {
                if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                    popm("SEIZURESEA", 0);
            } else {
                popm("SEIZURELAND", 1);
            }
        }
        delete_unit(u);
    }
}
