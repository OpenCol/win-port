/* popup_run_default -- shipped at 1050:8e63.
 *
 * Ported by tools/port.py from matched/game/popup_run_default.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * The file's own Popup dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 popup_run(Popup *p, char *text);

/* same object: 4-byte `push cs; call ` (rule 47) */



col_i16 popup_run_default(Popup *p)
{
    return popup_run(p, 0);
}
