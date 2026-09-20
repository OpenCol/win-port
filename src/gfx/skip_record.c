/* skip_record -- shipped at 1068:9921.
 *
 * Ported by tools/port.py from matched/gfx/skip_record.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * skip_record(char *p)
{
    return p + *(col_i16 *)(p + 2) + 4;
}
