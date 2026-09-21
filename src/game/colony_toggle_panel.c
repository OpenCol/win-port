/* colony_toggle_panel -- shipped at 1010:bfa6.
 *
 * Ported by tools/port.py from matched/game/colony_toggle_panel.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_panel_toggle;                       DGROUP:0x0c0c
 *   extern int far g_c06;                                DGROUP:0x0c06
 *   extern unsigned char far g_colony_mode;              DGROUP:0x0c0f
 *   extern char g_st_c;                                  DGROUP:0x0a0f
 *   extern char g_st_b;                                  DGROUP:0x0a0d
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1010_bdea(col_i16 a);
void empty_1010_be4c(col_i16 a);
void empty_1010_bee3(col_i16 a);
void empty_1010_bef1(col_i16 a);
void empty_1010_bf0d(col_i16 a);

#include "colflag.h"
void colony_toggle_panel(void)
{
    g_panel_toggle = !g_panel_toggle;
    switch (g_c06) {
    case 1:
        empty_1010_be4c(1);
        break;
    case 0:
        if (g_st_c == 0)
            empty_1010_bdea(1);
        break;
    case 4:
        if (g_st_b == 0)
            empty_1010_bf0d(1);
        break;
    case 3:
        empty_1010_bef1(1);
        break;
    case 2:
        empty_1010_bee3(1);
        break;
    }
    if (g_colony_mode == 2 && (g_cur_colony_rec->status & COLONY_BUILD_DONE) != 0)
        ;
}
