/* colony_message -- shipped at 1010:c085.
 *
 * Ported by tools/port.py from matched/game/colony_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern int far g_colony_message_words[];             SEG20:0xd996
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void ColChild_AddToMessage(void *w, col_i16 idx);

void colony_message(col_i16 idx)
{
    ColChild_AddToMessage(&(*g_colony_window), g_colony_message_words[idx]);
}
