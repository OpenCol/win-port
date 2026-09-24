/* colony_activate_message -- shipped at 1010:c04f.
 *
 * Ported by tools/port.py from matched/game/colony_activate_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_window;                     SEG20:0x25be
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void ColChild_ActivateMessage(void *w, char kind, col_i32 arg);
extern void ColChild_DrawAnyMessage(void *w, col_i16 mode, col_i32 arg);

void colony_activate_message(char kind, col_i32 arg)
{
    ColChild_ActivateMessage(&(*g_colony_window), kind, arg);
    ColChild_DrawAnyMessage(&(*g_colony_window), 1, 0L);
}
