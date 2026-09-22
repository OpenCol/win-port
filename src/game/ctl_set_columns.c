/* ctl_set_columns -- shipped at 1050:0bd9.
 *
 * Ported by tools/port.py from matched/game/ctl_set_columns.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: text -> value
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ctl_line_height(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */

void ctl_set_columns(Ctl *c, col_i16 n, col_i16 w)
{
    (*((col_i16 *)((unsigned char *)(c) + 0x86))) |= 0x1000;
    (*((col_i16 *)((unsigned char *)(c) + 0x8c))) = w;
    (*((col_i16 *)((unsigned char *)(c) + 0x90))) = n;
    (*((col_i16 *)((unsigned char *)(c) + 0x8e))) = ctl_line_height(c->value) * n + 2;
}
