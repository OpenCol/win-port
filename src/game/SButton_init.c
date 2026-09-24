/* SButton_init -- shipped at 1058:c11b.
 *
 * Ported by tools/port.py from matched/game/SButton_init.c.
 *
 * The file's own SButton dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: shown -> f1a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dlg_control_init_1058_c3f6(SButton *b, col_i16 a, col_i16 c, col_i16 kind, col_i16 d, void *pos);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern col_i16 MSInitButton(void *pos, SButton *b, char *title, col_i16 style);

void SButton_init(SButton *b, col_i16 a, col_i16 c, col_i16 d, void *pos, char *title)
{
    dlg_control_init_1058_c3f6(b, a, c, 6, d, pos);
    b->f1c = 0;
    b->handle = MSInitButton(pos, b, title, 1);
    b->f1a = 1;
}
