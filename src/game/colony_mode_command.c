/* colony_mode_command -- shipped at 1010:4690.
 *
 * Ported by tools/port.py from matched/game/colony_mode_command.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_colony_mode;              DGROUP:0x0c0f
 *   extern int g_stub_a;                                 
 *
 * The file's own ColChild dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: buy_shown -> mode_a, change_shown -> mode_b
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void control_hide_1010_e54d(void *c);
void control_show_1010_e574(void *c);
void peek_message_only_1010_e5aa(void);

extern char * current_window(void);
extern void ResetTimer(col_i16 id);
extern void colony_draw_panel6(void *w, col_i16 redraw);





void colony_mode_command(col_i16 cmd)
{
    ColChild *w;
    char *cw;

    cw = current_window();
    w = (ColChild *)(cw != 0 ? cw - 0x26 : cw);
    if (w->timer != -1) {
        ResetTimer(w->timer);
        w->timer = -1;
    }
    switch (cmd) {
    case 0x64:
        g_colony_mode = 0;
        control_hide_1010_e54d((char *)w + 0x950);
        control_hide_1010_e54d((char *)w + 0x972);
        w->mode_a = 0;
        w->mode_b = 0;
        break;
    case 0x65:
        g_colony_mode = 1;
        control_hide_1010_e54d((char *)w + 0x950);
        control_hide_1010_e54d((char *)w + 0x972);
        w->mode_a = 0;
        w->mode_b = 0;
        break;
    case 0x66:
        g_colony_mode = 2;
        control_show_1010_e574((char *)w + 0x950);
        control_show_1010_e574((char *)w + 0x972);
        w->mode_a = 1;
        w->mode_b = 1;
        break;
    }
    peek_message_only_1010_e5aa();
    colony_draw_panel6(w, 1);
}
