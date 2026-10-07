/* PORT: this file's own declarations of KingRoom removed: the header declares it from the definition (W1 step 3). */
/* KingHowdy: candidate for 1008:bcfb (81 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Plays the King's-room speech (KingRoom(1, 1, key)) whose text key is "VICEROY" -- or
 * "VICEROY2" for the Dutch (nation 3, g_game.player_nation at SEG20:0x779a), whose say_number(buf, 2)
 * appends the 2. (Corrected 2026-09-27: this said KINGLOSE, which nothing here refers to.)
 *
 * RENAMED 2026-09-27 from say_viceroy_intro: Mac KingHowdy(void), 78 bytes, against 81. Caller: start_new_game on both. Callees: say_number and KingRoom (Windows adds a STRCPY). Layout: Mac KingRoom, KingHowdy, get_game_options, start_new_game; Windows KingRoom, bcfb, get_game_options, start_new_game, one for one. Arity: void. The body is the VICEROY audience text into KingRoom.
 */

#include "colonization.h"

#include "nations.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

/* same object: 4-byte `push cs; call ` (rule 47) */
void KingHowdy(void)
{
    char buf[20];

    STRCPY(buf, "VICEROY");
    if (g_game.player_nation == NATION_NETHERLANDS)
        say_number(buf, 2);
    KingRoom(1, 1, buf);
}
