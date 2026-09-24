/* colony_employ_unit -- shipped at 1018:ae62.
 *
 * Ported by tools/port.py from matched/game/colony_employ_unit.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_colonist_index(col_i16 u);
void colony_native_claims(void);
void colony_refresh_screen(void);
void set_current_colony(col_i16 c);

extern col_i16 colony_assign_job(col_i16 c, col_i16 a);






col_i16 colony_employ_unit(col_i16 colony, col_i16 u)
{
    col_i16 colonist;
    col_i16 result;

    set_current_colony(colony);
    colony_native_claims();
    colony_refresh_screen();
    colonist = unit_colonist_index(u);
    result = colony_assign_job(colonist, 0);
    return result;
}
