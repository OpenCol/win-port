/* refresh_current_window_port_if -- shipped at 1010:ae63.
 *
 * Ported by tools/port.py from matched/game/refresh_current_window_port_if.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 return_one_1010_ae40(void *obj);

extern char * current_window(void);
extern void empty_1070_dad8(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void end_modal_1060_9fad(void *port);

void refresh_current_window_port_if(col_i16 unused1)
{
    char *c;
    char *w = current_window();

    c = w != 0 ? w - 0x26 : w;
    if (c == 0)
        return;
    empty_1070_dad8();
    if (return_one_1010_ae40(c) != 0)
        end_modal_1060_9fad(c + 0x26);
}
