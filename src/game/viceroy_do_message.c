/* viceroy_do_message -- shipped at 1038:b23a.
 *
 * Ported by tools/port.py from matched/game/viceroy_do_message.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_map_window;                        SEG20:0x0658
 *   extern int far g_misc_words[];                       SEG20:0xdcc8
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void viceroy_activate_message(char kind, col_i32 arg);
void viceroy_message(col_i16 idx);

extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void ColChild_AddToMessage(void *w, col_i16 idx);
extern void ColChild_ActivateMessage(void *w, char kind, col_i32 arg);
extern void ColChild_DrawAnyMessage(void *w, col_i16 mode, col_i32 arg);





void viceroy_do_message(col_i16 idx, char kind)
{
    ColChild_CancelMessage(&g_map_window, 1);
    viceroy_message(idx);
    viceroy_activate_message(kind, 120L);
}
