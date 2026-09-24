/* handle_fill_bytes -- shipped at 1040:3ea6.
 *
 * Ported by tools/port.py from matched/game/handle_fill_bytes.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern col_u16 MSBlockSize(col_i16 handle);
extern void * memset_rtl(void *p, col_i16 c, col_u16 n);

void handle_fill_bytes(col_i16 handle, char c)
{
    col_i16 h = handle;
    void *p;

    p = MSHLock(h);
    memset_rtl(p, (unsigned char)c, MSBlockSize(h));
    p = MSHUnlock(h);
}
