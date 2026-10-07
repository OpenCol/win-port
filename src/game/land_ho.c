/* land_ho: candidate for 1008:cc71 (133 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The landfall tutorial step: woodcut(1), the word at DGROUP:0x251a cleared, the LANDHO
 * text formatted into the player's nation record name (0x34-byte records at SEG20:0x7828),
 * which is then overwritten from the string at data segment 31 offset 0xa4; with bit 7 of
 * the byte at SEG20:0x7784 set, TUTORIAL2 is shown.
 *
 * RENAMED 2026-09-26 from tutorial_landfall: the Mac build's land_ho(void), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller execute_move_unit; callee popm), the runner-up gets 1, and the arity agrees (0). The margin is small, so the binding also rests on the names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "woodcut.h"
#include "gameflag.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void popup_ask_nocancel(void *ctx, char *name, int max_len, char *out);
extern int popm(char *msg, int n);

void land_ho(void)
{
    woodcut(WOODCUT_DISCOVERY_OF_THE_NEW_WORLD);
    g_popup_dude = 0;
    popup_ask_nocancel(g_game_txt, "LANDHO", 0x17, g_nation_text[g_game.turn_nation].country);
    STRCPY(g_nation_text[g_game.turn_nation].country, g_name_slots[0]);
    if (g_game.game_flags & GAMEFLAG_TUTORIAL)
        popm("TUTORIAL2", 0);
}
