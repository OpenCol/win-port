/* colony_message: candidate for 1010:c085 (43 bytes).  Verdict: RELOCATABLE.
 *
 * Appends the colony window's message text number `idx` (word table at SEG20:0xd996) to the
 * colony window record at SEG20:0x25be.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char pad[1]; } ColonyWin;

#include "classes.h"
extern void ColChild_AddToMessage(ColChild *child, int idx);

void colony_message(int idx)
{
    ColChild_AddToMessage((ColChild *)&g_colony_window, g_colony_message_words[idx]);
}
