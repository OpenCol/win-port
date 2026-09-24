/* viceroy_unit_msg6 -- shipped at 1038:b3fd.
 *
 * Ported by tools/port.py from matched/game/viceroy_unit_msg6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern UnitType far g_unit_types[];                  SEG20:0x7ac8
 *   extern char far g_map_window[];                      SEG20:0x0658
 *   extern int far g_nation_adjective[];                 SEG20:0xd89e
 *   extern int far g_nation_name_word[];                 SEG20:0xd8ae
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern int g_stub_a;                                 
 *
 * The file's own Unit, UnitType dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void viceroy_activate_message(col_i16 kind, col_i32 ms);
void viceroy_message(col_i16 n);

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */
/* 14 bytes at SEG20:0x7ac8 */







extern void ColChild_CancelMessage(void *w, col_i16 a);
extern void ColChild_AddToMessage(void *w, col_i16 idx);




void viceroy_unit_msg6(col_i16 u)
{
    ColChild_CancelMessage((&g_map_window), 1);
    ColChild_AddToMessage((&g_map_window), g_nation_adjective[(unsigned char)(g_units[u].flags & UNIT_NATION_MASK)]);
    ColChild_AddToMessage((&g_map_window), g_unit_types[g_units[u].type].name);
    viceroy_message(6);
    ColChild_AddToMessage((&g_map_window), g_nation_name_word[g_turn_nation]);
    viceroy_activate_message(1, 120L);
}
