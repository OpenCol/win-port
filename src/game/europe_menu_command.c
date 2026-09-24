/* europe_menu_command -- shipped at 1048:8901.
 *
 * Ported by tools/port.py from matched/game/europe_menu_command.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void europe_purchase(void);
void europe_recruit(col_i16 a, col_i16 b);
void recruit_at_university(void);
void set_current_nation(col_i16 n);

extern void * current_window(void);


/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */


void europe_menu_command(col_i16 cmd)
{
    Win *w;
    Win *cur;

    cur = (Win *)current_window();
    w = (cur != 0) ? (Win *)((char *)cur - 0x26) : cur;
    set_current_nation(w->nation);
    switch (cmd) {
    case 0xc8:
        europe_recruit(0, 0);
        break;
    case 0xc9:
        europe_purchase();
        break;
    case 0xca:
        recruit_at_university();
        break;
    }
}
