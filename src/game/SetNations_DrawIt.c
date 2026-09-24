/* SetNations_DrawIt -- shipped at 1030:00b7.
 *
 * Ported by tools/port.py from matched/game/SetNations_DrawIt.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_nation_name[];                      SEG20:0xd896
 *   extern int far g_word_de20[];                        SEG20:0xde20
 *   extern unsigned char far g_choice_colour[];          DGROUP:0x0099
 *   extern char far g_font_small;                        DGROUP:0x4b1e
 *   extern char far g_heading_font;                      DGROUP:0x4b22
 *
 * The file's own Screen dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rect_width_1030_e4e5(RECT *r);
void copy_all_to_screen_1030_e504(Screen *s);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal CopyRect(RECT *dst, RECT *src);
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);
extern void draw_bevel(Screen *s, RECT *r, col_i16 depth, col_i16 c1, col_i16 c2);
extern void TextColor(unsigned char v);
extern void draw_string_shadowed(Screen *s, void *font, char *t,
                                     col_i16 x, col_i16 y, col_i16 a);
extern void GRPort_DrawRect(Screen *s, RECT *r);
extern void say_string(char *s, col_i16 word);
extern char * STRUPR(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */



void SetNations_DrawIt(Screen *s)
{
    RECT r;
    char buf[0x50];
    register col_i16 x;
    register col_i16 y;

    SetRect(&r, 0, 0, 0x280, 0x1e0);
    CopyToPort(((char *)((unsigned char *)(s) + 0x12c)), s, &r, &r);
    SetRect(&r, 0, 0, 0xa0, 0x78);
    OffsetRect(&r, 0x20, 0x32);
    draw_bevel(s, &r, -2, 0xff, 0);
    SetRect(&r, 0, 0, 0x280, 0x1e0);
    switch ((*((col_i16 *)((unsigned char *)(s) + 0xe6)))) {
    case 1:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[0]);
        break;
    case 2:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[1]);
        break;
    case 3:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[2]);
        break;
    case 4:
        CopyRect(&r, &((RECT *)((unsigned char *)(s) + 0xc4))[3]);
        break;
    }
    TextColor(0xfa);
    x = 0x70;
    y = 0x3c;
    draw_string_shadowed(s, &(*g_font_small), "Select", x, y, 0x10);
    draw_string_shadowed(s, &(*g_font_small), "European Power", x, y + 0x19, 0x10);
    TextColor(g_choice_colour[(*((col_i16 *)((unsigned char *)(s) + 0xe6)))]);
    GRPort_DrawRect(s, &r);
    x = (rect_width_1030_e4e5(&r) >> 1) + r.left;
    y = r.top + 0x12;
    buf[0] = 0;
    say_string(buf, g_nation_name[(*((col_i16 *)((unsigned char *)(s) + 0xe6))) - 1]);
    STRUPR(buf);
    draw_string_shadowed(s, &g_heading_font, buf, x, y, 0x10);
    y = r.bottom - 0x12;
    buf[0] = 0;
    say_string(buf, g_word_de20[(*((col_i16 *)((unsigned char *)(s) + 0xe6)))]);
    draw_string_shadowed(s, &g_heading_font, buf, x, y, 0x10);
    copy_all_to_screen_1030_e504(s);
}
