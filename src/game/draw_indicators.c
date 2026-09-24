/* draw_indicators -- shipped at 1050:45f0.
 *
 * Ported by tools/port.py from matched/game/draw_indicators.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_text_value;                         DGROUP:0x251a
 *   extern int far g_sel_b;                              DGROUP:0x251c
 *   extern int far g_sel_c;                              DGROUP:0x2518
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void sprite_draw(void *buf, void *rec, void *dst, col_i16 x, col_i16 y);

void draw_indicators(Win *w)
{
    char b1[8], b2[8], b3[8], b4[8], b5[8], b6[8], b7[8];

    if (g_text_value != -1) {
        switch (g_text_value) {
        case 0: sprite_draw(b1, ((char *)((unsigned char *)(w) + 0x2e)), (*((void * *)((unsigned char *)(w) + 0x0))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))), (*((col_i16 *)((unsigned char *)(w) + 0x33e)))); break;
        case 1: sprite_draw(b2, ((char *)((unsigned char *)(w) + 0x2e)), (*((void * *)((unsigned char *)(w) + 0x0))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + (*((col_i16 *)((unsigned char *)(w) + 0x88))), (*((col_i16 *)((unsigned char *)(w) + 0x33e)))); break;
        case 2: case 3: case 4: case 5:
            sprite_draw(b3, ((char *)((unsigned char *)(w) + 0x2e)), (*((void * *)((unsigned char *)(w) + 0x0))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + ((*((col_i16 *)((unsigned char *)(w) + 0x338))) >> 1), (*((col_i16 *)((unsigned char *)(w) + 0x33e)))); break;
        }
    } else if (g_sel_b != -1) {
        switch (g_sel_b) {
        case 0: sprite_draw(b4, ((char *)((unsigned char *)(w) + 0x2e)), (*((void * *)((unsigned char *)(w) + 0x0))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + (*((col_i16 *)((unsigned char *)(w) + 0x88))), (*((col_i16 *)((unsigned char *)(w) + 0x33e)))); break;
        case 1: case 2: case 3:
            sprite_draw(b5, ((char *)((unsigned char *)(w) + 0x2e)), (*((void * *)((unsigned char *)(w) + 0x0))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + ((*((col_i16 *)((unsigned char *)(w) + 0x338))) >> 1), (*((col_i16 *)((unsigned char *)(w) + 0x33e)))); break;
        }
    } else if (g_sel_c != -1) {
        switch (g_sel_c) {
        case 0: case 1: case 2: case 3: case 5: case 6: case 7: case 8:
            sprite_draw(b6, ((char *)((unsigned char *)(w) + 0x2e)), (*((void * *)((unsigned char *)(w) + 0x0))), 10, 10); break;
        case 4: sprite_draw(b7, ((char *)((unsigned char *)(w) + 0x2e)), (*((void * *)((unsigned char *)(w) + 0x0))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + (*((col_i16 *)((unsigned char *)(w) + 0x88))), 10); break;
        }
    }
}
