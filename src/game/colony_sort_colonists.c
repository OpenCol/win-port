/* colony_sort_colonists -- shipped at 1018:604c.
 *
 * Ported by tools/port.py from matched/game/colony_sort_colonists.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far *g_cur_colony_rec;                 DGROUP:0x4b9e
 *   extern int g_selected_colonist;                      DGROUP:0x4bbc
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



extern void sort_pairs_by_key(col_i16 n, unsigned char *idx, unsigned char *keys);




void colony_sort_colonists(void)
{
    col_i16 i;
    char slot;
    unsigned char keys[0x20];
    unsigned char idx[0x20];
    unsigned char order[0x20];
    unsigned char nib[0x20];
    unsigned char spec[0x20];

    if ((col_i16)g_cur_colony_rec->pop < 2)
        goto done;
    for (i = 0; i < g_cur_colony_rec->pop; i++) {
        idx[i] = i;
        spec[i] = ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x40))[i];
        nib[i] = colonist_spec_nibble(i);
        keys[i] = ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x20))[i];
    }
    sort_pairs_by_key(g_cur_colony_rec->pop, idx, keys);
    for (i = 0; i < g_cur_colony_rec->pop; i++)
        order[idx[i]] = i;
    for (i = 0; i < g_cur_colony_rec->pop; i++) {
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x40))[order[i]] = spec[i];
        ((unsigned char *)((unsigned char *)(g_cur_colony_rec) + 0x20))[i] = keys[i];
        colonist_set_spec_nib(order[i], nib[i]);
    }
    g_selected_colonist = order[g_selected_colonist];
    for (i = 0; i < 20; i++) {
        slot = ((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[i];
        if ((col_i16)slot >= 0)
            ((signed char *)((unsigned char *)(g_cur_colony_rec) + 0x70))[i] = order[slot];
    }
done:
    ;
}
