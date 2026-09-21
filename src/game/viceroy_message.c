/* viceroy_message -- shipped at 1038:b20f.
 *
 * Ported by tools/port.py from matched/game/viceroy_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window;                        SEG20:0x0658
 *   extern int far g_misc_words[];                       SEG20:0xdcc8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void ColChild_AddToMessage(void *w, col_i16 idx);

void viceroy_message(col_i16 idx)
{
    ColChild_AddToMessage(&g_map_window, g_misc_words[idx]);
}
