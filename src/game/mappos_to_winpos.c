/* mappos_to_winpos -- shipped at 1038:a221.
 *
 * Ported by tools/port.py from matched/game/mappos_to_winpos.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: cx -> f77a, cy -> f77c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 MapWin_TileSize(MapWin *w);
col_i16 rect_height_1038_dfdc(RECT *r);
col_i16 rect_width_1038_dfbd(RECT *r);

#define ABS(v) ((v) > 0 ? (v) : ~(v) + 1)


/* same object: 4-byte `push cs; call ` */




void mappos_to_winpos(MapWin *w, register col_i16 x, col_i16 y, col_i16 *px, col_i16 *py)
{
    col_i16 tile;
    col_i16 half_w;
    col_i16 half_h;
    col_i16 dx;
    col_i16 dy;

    tile = MapWin_TileSize(w);
    half_w = (rect_width_1038_dfbd(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - tile) / 2;
    half_h = (rect_height_1038_dfdc(&(*((RECT *)((unsigned char *)(w) + 0x76a)))) - tile) / 2;
    if (ABS(x - w->f77a) > ABS(x - w->f77a - (*g_map_width)))
        x -= (*g_map_width);
    else if (ABS(x - w->f77a) > ABS(x - w->f77a + (*g_map_width)))
        x += (*g_map_width);
    dx = x - w->f77a;
    dy = y - w->f77c;
    *px = dx * tile + half_w;
    *py = dy * tile + half_h;
    *px += (*((RECT *)((unsigned char *)(w) + 0x76a))).left;
    *py += (*((RECT *)((unsigned char *)(w) + 0x76a))).top;
}
