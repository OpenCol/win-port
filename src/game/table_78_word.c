/* table_78_word -- shipped at 1038:0139.
 *
 * Ported by tools/port.py from matched/game/table_78_word.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Tribe far g_tribes[];                         SEG20:0xd182
 *
 * The file's own Tribe dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 table_78_word(col_i16 t, col_i16 i)
{
    return ((col_i16 *)((unsigned char *)&(g_tribes[t]) + 0x46))[i];
}
