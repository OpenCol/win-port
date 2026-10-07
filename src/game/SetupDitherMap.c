/* SetupDitherMap: candidate for 1008:7ec9 (555 bytes).  Verdict: RELOCATABLE.
 * symbol: SetupDitherMap
 * name-source: sibling-build
 *
 * Precomputes, for every map square and each of its four orthogonal neighbours, the terrain
 * seam to draw between them -- one byte per square per direction, in a handle of w * h * 4
 * bytes kept at DGROUP:0x4b2e and allocated on the first call.
 *
 * 0xff means no seam. Otherwise the neighbour's terrain (the low five bits of plane 0) decides:
 * below 0x18 it is a land type, and a seam is drawn only when its GROUP -- the terrain over
 * eight -- differs from this square's, as `(neighbour % 8) * 4 + direction`. 0x18 is the arctic
 * edge and gets `direction + 0x20`.
 *
 * Water is where it gets interesting. Against 0x19 -- shallow water -- a LAND square asks
 * `ScanOcean` what the water's own coastline says the terrain behind it is, and draws that
 * square's seam instead: the beach belongs to the land it touches, not to the sea. A 0x1a
 * square next to 0x19 gets `direction + 0x28`, and 0x19 next to 0x1a gets `direction + 0x24`,
 * so the deep-to-shallow boundary has its own pair of seams in each direction.
 *
 * `(int)t != -1` and the two comparisons after it need their casts: without them Borland tests
 * the `signed char` in place (`CMP BYTE`) where the shipped loads it, `CBW`s and compares the
 * word (rule 51). The `nb` comparisons against 0x18 stay in bytes because `nb` is UNSIGNED --
 * `JA`, `JNC`, which is also what says the neighbour byte is an `unsigned char`.
 *
 * `sx` and `sy` are the register locals; the three loop counters and the four terrain bytes
 * live in the frame, and the walking pointer is a plain `` -- `INC WORD [BP-0xa]` on the
 * offset alone, with no `__AHSHIFT` normalization, so a map over 64K of seams would wrap.
 *
 * RENAMED 2026-09-27 from build_terrain_seams: Mac SetupDitherMap(void), 522 bytes, against 555. Callers: Mac load_game(FSSpec*) and start_new_game(short); Windows load_game_file (the Mac load_game(FSSpec*) twin) and start_new_game. Callees: MSAllocateHandle, MSHLock, map_get x2, ScanOcean (7e2d) and MSHUnlock on both, in that order. Layout: as 7cb5. Arity: void. This also settles the const_proposal 1040:1408 terrain_tile_index -> SetupDitherMap, rejected again below: SetupDitherMap is here.
 */

#include "colonization.h"

#include "terrain.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern unsigned char map_get(int x, int y);

/* same object: 4-byte `push cs; call ` */
extern signed char ScanOcean(int x, int y);

void SetupDitherMap(void)
{
    void *p;
    unsigned char *q;
    unsigned char seam;
    signed char terrain;
    unsigned char base;
    unsigned char nb;
    int y;
    int x;
    int dir;
    register int sx;
    register int sy;

    if (g_seam_handle == 0)
        g_seam_handle = MSAllocateHandle((long)g_map_size.w * g_map_size.h * 4);
    p = MSHLock(g_seam_handle);
    q = (unsigned char *)p;
    for (y = 0; g_map_size.h > y; y++) {
        for (x = 0; g_map_size.w > x; x++) {
            base = map_get(x, y) & MAP0_TERRAIN_MASK;
            for (dir = 0; dir < 4; dir++) {
                seam = 0xff;
                sx = g_ortho_dx[dir] + x;
                sy = g_ortho_dy[dir] + y;
                if (sx >= 0 && g_map_size.w > sx && sy >= 0 && g_map_size.h > sy) {
                    nb = map_get(sx, sy) & MAP0_TERRAIN_MASK;
                    if (base != nb) {
                        if (nb <= 0x18) {
                            if (nb < 0x18) {
                                if (nb % 8 != base % 8)
                                    seam = (nb % 8) * 4 + dir;
                            } else if (nb == 0x18) {
                                seam = dir + 0x20;
                            }
                        } else {
                            if (nb == 0x19) {
                                if (base < 0x19) {
                                    terrain = ScanOcean(sx, sy);
                                    if ((int)terrain != -1) {
                                        if ((int)terrain < 0x18) {
                                            if (terrain % 8 != base % 8)
                                                seam = (terrain % 8) * 4 + dir;
                                        } else if ((int)terrain == 0x18) {
                                            seam = dir + 0x20;
                                        }
                                    }
                                } else if (base == 0x1a) {
                                    seam = dir + 0x28;
                                }
                            } else if (nb == 0x1a) {
                                if (base == 0x19)
                                    seam = dir + 0x24;
                            }
                        }
                    }
                }
                *q = seam;
                q++;
            }
        }
    }
    p = MSHUnlock(g_seam_handle);
}
