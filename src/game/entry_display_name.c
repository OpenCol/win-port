/* entry_display_name -- shipped at 1060:27a8.
 *
 * Ported by tools/port.py from matched/game/entry_display_name.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char g_display_name_buf[];                    DGROUP:0x4e14
 *   extern int far g_feature_words[];                    SEG20:0xd8d8
 *
 * The file's own Entry dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern char * nth_text_string(col_i16 n);

extern void append_space(char *s);
extern void say_string(char *buf, col_i16 word);

char * entry_display_name(void *unused, Entry *e)
{
    char *s;

    s = nth_text_string(e->name);
    strcpy(g_display_name_buf, s);
    if (e->kind == 2 && e->value >= 8 && e->value < 0x10) {
        append_space(g_display_name_buf);
        say_string(g_display_name_buf, g_feature_words[0]);
    }
    return g_display_name_buf;
}
