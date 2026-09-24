/* draw_active_animations -- shipped at 1010:245e.
 *
 * Ported by tools/port.py from matched/game/draw_active_animations.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void sprite_draw(void *buf, void *rec, void *dst, col_i16 x, col_i16 y);

void draw_active_animations(Win *w)
{
    char b1[8], b2[8], b3[8], b4[8], b5[8], b6[8];

    if ((col_i16)w->flag1 == 1) {
        sprite_draw(b1, ((char *)((unsigned char *)(w) + 0x7cc)), ((char *)((unsigned char *)(w) + 0xc4)), 0x2b4, 0xe6);
        if (w->count1 == 0) w->flag1 = 0;
    }
    if ((col_i16)w->flag2 == 1) {
        sprite_draw(b2, ((char *)((unsigned char *)(w) + 0x102c)), ((char *)((unsigned char *)(w) + 0xc4)), 0x499, 0x62);
        if (w->count2 == 0) w->flag2 = 0;
    }
    if ((col_i16)w->flag3 == 1) {
        sprite_draw(b3, ((char *)((unsigned char *)(w) + 0x122c)), ((char *)((unsigned char *)(w) + 0xc4)), 0x499, 0x62);
        if (w->count3 == 0) w->flag3 = 0;
    }
    if ((col_i16)w->flag4 == 1) {
        sprite_draw(b4, ((char *)((unsigned char *)(w) + 0x13ec)), ((char *)((unsigned char *)(w) + 0xc4)), 0x2a1, 0x6d);
        if (w->count4 == 0) w->flag4 = 0;
    }
    if ((col_i16)w->flag5 == 1) {
        sprite_draw(b5, ((char *)((unsigned char *)(w) + 0x18ac)), ((char *)((unsigned char *)(w) + 0xc4)), 0x63d, 0x61);
        if (w->count5 == 0) w->flag5 = 0;
    }
    if ((col_i16)w->flag6 == 1) {
        sprite_draw(b6, (char *)w + 0x1a0c, ((char *)((unsigned char *)(w) + 0xc4)), 0x4ab, 0x118);
        if (w->count6 == 0) w->flag6 = 0;
    }
}
