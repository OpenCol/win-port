/* next_colony_name -- shipped at 1028:a078.
 *
 * Ported by tools/port.py from matched/game/next_colony_name.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *
 * The file's own NationRec52 dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL, +2 counts colony names taken */


extern col_i16 load_text_resource(char *type, char *section);
extern char * next_text_line(void);
extern char * next_csv_field(void);
extern void empty_1040_6ce6(void);

void next_colony_name(col_i16 nation, char *out)
{
    char section[20];
    char line[80];
    col_i16 i;

    strcpy(out, "?");
    switch (nation) {
    case 0: strcpy(section, "ENGLISH"); break;
    case 1: strcpy(section, "FRENCH"); break;
    case 2: strcpy(section, "SPANISH"); break;
    case 3: strcpy(section, "DUTCH"); break;
    }
    if (load_text_resource("COLONY", section))
        goto done;
    for (i = 0; g_nation52[nation].colony_names_used >= i; i++) {
        next_text_line();
        strcpy(line, next_csv_field());
        if ((col_i16)line[0] == '@')
            goto done;
        if (i == 0 || g_nation52[nation].colony_names_used == i)
            strcpy(out, line);
    }
    out[23] = 0;
    g_nation52[nation].colony_names_used++;
done:
    empty_1040_6ce6();
}
