/* colony_cancel_then_message -- shipped at 1010:c0b0.
 *
 * Ported by tools/port.py from matched/game/colony_cancel_then_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_colony_window;                     SEG20:0x25be
 *   extern int far g_colony_message_words[];             SEG20:0xd996
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void colony_activate_message(char kind, col_i32 arg);
void colony_message(col_i16 idx);

extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void ColChild_AddToMessage(void *w, col_i16 idx);
extern void ColChild_ActivateMessage(void *w, char kind, col_i32 arg);
extern void ColChild_DrawAnyMessage(void *w, col_i16 mode, col_i32 arg);





void colony_cancel_then_message(col_i16 idx, col_i32 arg)
{
    ColChild_CancelMessage(&(*g_colony_window), 1);
    colony_message(idx);
    colony_activate_message(3, arg);
}
