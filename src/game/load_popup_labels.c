/* load_popup_labels -- shipped at 1050:002f.
 *
 * Ported by tools/port.py from matched/game/load_popup_labels.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_popup_label_id[];                       DGROUP:0x4cf8
 *   extern char far *far g_popup_label[];                SEG31:0x0098
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 load_text_resource(char *file, char *group);
extern col_i16 read_next_text_value(void);
extern char * nth_text_string(col_i16 n);



void load_popup_labels(void)
{
    register col_i16 i;

    if (load_text_resource("LABELS", "POPUPS") != 0)
        goto out;
    for (i = 0; i < 3; i++) {
        g_popup_label_id[i] = read_next_text_value();
        g_popup_label[i] = nth_text_string(g_popup_label_id[i]);
    }
out:
    ;
}
