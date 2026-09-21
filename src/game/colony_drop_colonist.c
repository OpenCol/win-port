/* colony_drop_colonist -- shipped at 1018:4aad.
 *
 * Ported by tools/port.py from matched/game/colony_drop_colonist.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int far g_colony_screen_open;                 DGROUP:0x0a12
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 colonist_spec_nibble(col_i16 c);
void colonist_set_spec_nib(col_i16 c, col_i16 v);

/* 0xca bytes */



extern void colony_flag_set(col_i16 a, col_i16 b);




void colony_drop_colonist(col_i16 c)
{
    register col_i16 j;
    register col_i16 k;

    for (j = c; j < g_cur_colony_rec->pop - 1; j++) {
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x20))[j] = ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x20))[j + 1];
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x40))[j] = ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x40))[j + 1];
        colonist_set_spec_nib(j, colonist_spec_nibble(j + 1));
    }
    for (k = 0; k < 20; k++) {
        if (((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[k] == (char)c)
            ((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[k] = -1;
        else if (((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[k] > (char)c)
            ((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[k]--;
    }
    g_cur_colony_rec->pop--;
    g_cur_colony_rec->bells_cap -= 100;
    if (g_colony_screen_open != 0 && (col_i16)g_cur_colony_rec->pop < 3)
        colony_flag_set(0, 0);
}
