/* colony_owns_tile: candidate for 1018:3f85 (166 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: colony_owns_tile
 *
 * 1 when (x, y) is the current colony's own tile or one of the ring tiles
 * around it: the ring size comes from colony_map_status through the byte table
 * at DGROUP:0xc00, the ring offsets from SEG27:0x12/0x27.  0 off the map.
 * colony_map_status is -called and defined in-file as a stub.
 */

#include "colonization.h"

#include "records.h"

/* DEFINED here: DGROUP:0xbfe-0xc2a is this module's own _DATA (its string literals
   follow at 0xc2a), so the ring table is its data. Defined after colony_plan and the
   other five functions that reach it with a segment fixup, it is from here on:
   Borland C++ keeps an extern array's `` for the rest of the file unless a
   definition follows. The shipped bytes at 0xc01 are 04 08 0c 14; 0xc05 is 00 and
   may be a fifth entry. */
/* A plain byte array, not a one-byte record: segment 4's _DATA holds it at the odd 0xc01,
   where Borland word-aligns any struct-typed global (tools/check_module_data.py, 2026-10-02). */
unsigned char g_ring_tile_count[] = { 4, 8, 12, 20 };   /* DGROUP:0xc01 */

extern int g_stub_a;

extern "C" int colony_map_status(void);

int colony_owns_tile(int x, int y)
{
    register int found = 0;
    register int i;
    int n;

    if (on_map(x, y) == 0)
        goto done;
    n = g_ring_tile_count[colony_map_status() - 1];
    if (g_cur_colony_rec->x == (unsigned char)x && g_cur_colony_rec->y == (unsigned char)y)
        found = 1;
    for (i = 0; found == 0 && i < n; i++) {
        if ((unsigned char)(g_ring_dx[i] + g_cur_colony_rec->x) == (unsigned char)x
            && (unsigned char)(g_cur_colony_rec->y + g_ring_dy[i]) == (unsigned char)y)
            found = 1;
    }
done:
    return found;
}
