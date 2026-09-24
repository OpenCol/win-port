/* colony_panels_reset -- shipped at 1010:bf45.
 *
 * Ported by tools/port.py from matched/game/colony_panels_reset.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1010_bddc(void);
void empty_1010_bdea(col_i16 a);
void empty_1010_bdf8(col_i16 a);
void empty_1010_be3e(col_i16 a);
void empty_1010_be4c(col_i16 a);
void empty_1010_bee3(col_i16 a);
void empty_1010_bef1(col_i16 a);
void empty_1010_bf0d(col_i16 a);
void empty_1010_bf37(col_i16 a, col_i16 b);

extern void colony_refresh_screen(void);











void colony_panels_reset(void)
{
    colony_refresh_screen();
    empty_1010_bddc();
    empty_1010_bdf8(0);
    empty_1010_bdea(0);
    empty_1010_be4c(0);
    empty_1010_bf0d(0);
    empty_1010_bf37(0, 0);
    empty_1010_bee3(0);
    empty_1010_bef1(0);
    empty_1010_be3e(0);
}
