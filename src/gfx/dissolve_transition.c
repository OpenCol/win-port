/* dissolve_transition -- shipped at 1068:af72.
 *
 * Ported by tools/port.py from matched/gfx/dissolve_transition.c.
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_6_1068_d7db(void *p);
col_i16 max_int_1068_d81a(col_i16 a, col_i16 b);
col_i16 min_int_1068_d7f4(col_i16 a, col_i16 b);
col_i16 rand_scaled(col_i16 n);
col_i16 rect_height_1068_d7bb(RECT *r);
col_i16 rect_width_1068_d79c(RECT *r);
void MSDoTask_1068_d906(void);
void copy_rect_to_screen_1068_d86d(Win *w, RECT *r);
void port_copy_rect(Win *w);

extern char * GRPort_PixelMap(void *p, col_i16 x, col_i16 y);
extern void MSLockGraf(void *p);
/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */







void dissolve_transition(Win *w)
{
    col_i16 x0;
    col_i16 y0;
    col_i16 width;
    col_i16 height;
    col_i16 rb1;
    col_i16 rb2;
    col_i16 pass;
    col_i16 ry;
    char *dst;
    char *src;
    char *dp;
    char *sp;
    register col_i16 i;
    register col_i16 rx;

    width = rect_width_1068_d79c(&(*((RECT *)((unsigned char *)(w) + 0xee))));
    height = rect_height_1068_d7bb(&(*((RECT *)((unsigned char *)(w) + 0xee))));
    x0 = (*((RECT *)((unsigned char *)(w) + 0xee))).left;
    y0 = (*((RECT *)((unsigned char *)(w) + 0xee))).top;
    rb1 = get_w_6_1068_d7db(w);
    rb2 = get_w_6_1068_d7db(((char *)((unsigned char *)(w) + 0xc4)));
    MSLockGraf(w);
    MSLockGraf(((char *)((unsigned char *)(w) + 0xc4)));
    src = GRPort_PixelMap(((char *)((unsigned char *)(w) + 0xc4)), x0, y0);
    dst = GRPort_PixelMap(w, x0, y0);
    for (pass = 0; min_int_1068_d7f4(width, height) > pass; pass++) {
        for (i = 0; max_int_1068_d81a(width, height) > i; i++) {
            rx = rand_scaled(width);
            ry = rand_scaled(height);
            dp = dst + ((col_i32)ry * rb1 + rx);
            sp = src + ((col_i32)ry * rb2 + rx);
            *dp = *sp;
        }
        copy_rect_to_screen_1068_d86d(w, &(*((RECT *)((unsigned char *)(w) + 0xee))));
        MSDoTask_1068_d906();
    }
    port_copy_rect(w);
    copy_rect_to_screen_1068_d86d(w, &(*((RECT *)((unsigned char *)(w) + 0xee))));
}
