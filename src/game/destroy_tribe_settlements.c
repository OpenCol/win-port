/* destroy_tribe_settlements -- shipped at 1030:848a.
 *
 * Ported by tools/port.py from matched/game/destroy_tribe_settlements.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void native_settlement_delete(col_i16 i);

typedef struct { char pad[2]; unsigned char owner; char rest[0x12 - 3]; } Settlement_local;


/* same object: 4-byte `push cs; call ` (rule 47) */

void destroy_tribe_settlements(col_i16 tribe)
{
    col_i16 nation = tribe + 4;
    col_i16 i;

    for (i = g_settlement_count - 1; i >= 0; i--)
        if (g_settlements[i].owner == (unsigned char)nation)
            native_settlement_delete(i);
}
