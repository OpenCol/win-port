/* on_info_window_pointer -- shipped at 1058:8b67.
 *
 * Ported by tools/port.py from matched/game/on_info_window_pointer.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_message_pending;                    SEG20:0x77c8
 *   extern int far g_message_flag;                       SEG20:0x77c6
 *   extern int far g_view_mode;                          SEG20:0x7792
 *   extern char far g_game_record2;                      SEG20:0x40d6
 *
 * The file's own InfoOwner dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void win_show_and_focus_1058_bc47(void *w);

extern char * current_window(void);




/* same object: 4-byte `push cs; call ` (rule 47) */

extern void select_next_unit(void *rec, col_i16 a);
extern void view_pieces(void *rec);
extern void repaint_info_window(void);

void on_info_window_pointer(col_i16 x, col_i16 y)
{
    InfoOwner *o;
    char *w = current_window();

    o = (InfoOwner *)(w != 0 ? w - 0x26 : w);
    if (o->left < x && o->right > x && o->top < y && o->bottom > y) {
        win_show_and_focus_1058_bc47((char *)o + 0x26);
        if (g_message_pending != 0) {
            g_message_flag = 0;
            return;
        }
        if (g_view_mode == 1) {
            select_next_unit(&(*g_game_record2), 0);
            return;
        }
        view_pieces(&(*g_game_record2));
        repaint_info_window();
    }
}
