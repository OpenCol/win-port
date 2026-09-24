/* block_size -- shipped at 1078:0409.
 *
 * Ported by tools/port.py from matched/platform/block_size.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_u32 MSBlockSize(col_i16 handle);

col_u32 block_size(void *res)
{
    return MSBlockSize((col_i16)res);
}
