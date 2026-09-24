/* cd_draw_time -- shipped at 1040:9eab.
 *
 * Ported by tools/port.py from matched/game/cd_draw_time.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *   extern char far g_font_small;                        DGROUP:0x4b1e
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void copy_rect_to_screen_1040_af03(Win *w, RECT *r);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void FillColor(Win *w, RECT *r, col_i16 color);
extern void TextColor(col_i16 v);
extern col_i16 MSGetCDInfo(col_i16 *min, col_i16 *sec, void *p);
extern char * LTOA1(col_i16 v, char *s, col_i16 radix);


extern void draw_string_shadowed(Win *w, void *font, char *s,
                                     col_i16 x, col_i16 y, col_i16 a);

/* same object: 4-byte `push cs; call ` */


void cd_draw_time(Win *w)
{
    char buf[10];
    col_i16 min;
    col_i16 sec;
    RECT r;
    char out[24];

    SetRect(&r, 0x64, 0xc, 0xc6, 0x3c);
    OffsetRect(&r, w->ox, w->oy);
    FillColor(w, &r, g_draw_color);
    TextColor(0xc);
    if (MSGetCDInfo(&min, &sec, 0) != 0) {
        LTOA1(min, buf, 10);
        strcpy(out, buf);
        strcat(out, ":");
        buf[0] = 0;
        LTOA1(sec, buf, 10);
        if (sec < 10)
            strcat(out, "0");
        strcat(out, buf);
    } else {
        strcpy(out, "00:00");
    }
    draw_string_shadowed(w, &(*g_font_small), out, w->ox + 0x96, w->oy + 0x23, 0);
    copy_rect_to_screen_1040_af03(w, &r);
}
