/* find_entry -- shipped at 1040:7360.
 *
 * Ported by tools/port.py from matched/game/find_entry.c.
 *
 * The file's own Entry, Table dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: key -> name
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 12 bytes */
Entry * find_entry(Table *t, col_i16 key)
{
    col_i16 i;

    for (i = 0; (*((col_i16 *)((unsigned char *)(t) + 0x8))) > i; i++) {
        if (((Entry *)((unsigned char *)(t) + 0xc))[i].name == key)
            return &((Entry *)((unsigned char *)(t) + 0xc))[i];
    }
    return 0;
}
