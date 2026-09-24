/* win_draw_background -- shipped at 1050:41ad.
 *
 * Ported by tools/port.py from matched/game/win_draw_background.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_background_tile;                   SEG20:0x7506
 *   extern int far g_sel_c;                              DGROUP:0x2518
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 port_height_1050_8ba9(void *w);
col_i16 port_width_1050_8b8a(void *w);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void sprite_draw(void *out, void *rec, void *dst, col_i16 x, col_i16 y);
extern void port_set_clip(void *w, RECT *r);
extern void port_apply_rect(void *w);
extern void GRPort_DrawRect(void *w, RECT *r);
extern void FillColor(void *w, RECT *r, col_i16 color);
extern void TextColor(col_i16 v);


/* same object: 4-byte `push cs; call ` */


extern void draw_indicators(Win *w);

void win_draw_background(Win *w, col_i16 unused1, col_i16 unused2, col_i16 unused3, col_i16 unused4)
{
    col_i16 width;
    col_i16 height;
    RECT rc;
    RECT r1;
    RECT r2;
    RECT r3;
    register col_i16 a;
    register col_i16 b;

    if ((*((col_i16 *)((unsigned char *)(w) + 0x340))) != 0) {
        width = port_width_1050_8b8a((*((void * *)((unsigned char *)(w) + 0x0))));
        height = port_height_1050_8ba9((*((void * *)((unsigned char *)(w) + 0x0))));
        for (a = 0; a < height; a += 0x20)
            for (b = 0; b < width; b += 0x20)
                sprite_draw(&r1, ((char *)((unsigned char *)(w) + 0x4e)), (*((void * *)((unsigned char *)(w) + 0x0))), b, a);
        SetRect(&rc, (*((col_i16 *)((unsigned char *)(w) + 0x33c))), (*((col_i16 *)((unsigned char *)(w) + 0x33e))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + (*((col_i16 *)((unsigned char *)(w) + 0x88))), (*((col_i16 *)((unsigned char *)(w) + 0x33e))) + (*((col_i16 *)((unsigned char *)(w) + 0x8a))));
        if (g_sel_c == 5)
            draw_indicators(w);
        port_set_clip((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        for (b = (*((col_i16 *)((unsigned char *)(w) + 0x33e))) + 3; height - 3 > b; b += 0x2f)
            for (a = (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + 3; width - 3 > a; a += 0x3e)
                sprite_draw(&r2, &g_background_tile, (*((void * *)((unsigned char *)(w) + 0x0))), a, b);
        port_apply_rect((*((void * *)((unsigned char *)(w) + 0x0))));
        SetRect(&rc, (*((col_i16 *)((unsigned char *)(w) + 0x33c))), (*((col_i16 *)((unsigned char *)(w) + 0x33e))), (*((col_i16 *)((unsigned char *)(w) + 0x33c))) + (*((col_i16 *)((unsigned char *)(w) + 0x88))), (*((col_i16 *)((unsigned char *)(w) + 0x33e))) + (*((col_i16 *)((unsigned char *)(w) + 0x8a))));
        TextColor(0x21);
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        TextColor(0x19);
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
    } else {
        width = port_width_1050_8b8a((*((void * *)((unsigned char *)(w) + 0x0))));
        height = port_height_1050_8ba9((*((void * *)((unsigned char *)(w) + 0x0))));
        SetRect(&rc, 0, 0, width, height);
        FillColor((*((void * *)((unsigned char *)(w) + 0x0))), &rc, 0x24);
        SetRect(&rc, 3, 3, width - 3, height - 3);
        port_set_clip((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        for (b = 3; height - 3 > b; b += 0x2f)
            for (a = 3; width - 3 > a; a += 0x3e)
                sprite_draw(&r3, &g_background_tile, (*((void * *)((unsigned char *)(w) + 0x0))), a, b);
        port_apply_rect((*((void * *)((unsigned char *)(w) + 0x0))));
        SetRect(&rc, 0, 0, width, height);
        TextColor(0x21);
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        TextColor(0x19);
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
        rc.left++;
        rc.right--;
        rc.top++;
        rc.bottom--;
        GRPort_DrawRect((*((void * *)((unsigned char *)(w) + 0x0))), &rc);
    }
}
