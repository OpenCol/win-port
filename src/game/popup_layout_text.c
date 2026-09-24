/* popup_layout_text -- shipped at 1050:1757.
 *
 * Ported by tools/port.py from matched/game/popup_layout_text.c.
 *
 * STRCHR -> strchr, STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * The file's own FontRec, Item, Popup dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: flags -> b
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ctl_line_height(FontRec *f);
col_i16 font_text_extent_1050_8391(FontRec *f, const char *s);

/* same object: 4-byte `push cs; call ` */



col_i16 popup_layout_text(Popup *w)
{
    char *s;
    char *p;
    char *prev;
    col_i16 height;
    Item *item;
    col_i16 width;
    col_i16 line_h;
    col_i16 x;
    col_i16 space_w;
    col_i16 line_w;
    col_i16 y;
    char stuck;
    register col_i16 n;
    register col_i16 ww;

    stuck = 0;
    x = (*((col_i16 *)((unsigned char *)(w) + 0x346))) + 6;
    line_w = 0;
    space_w = font_text_extent_1050_8391((*((FontRec * *)((unsigned char *)(w) + 0x6e))), " ");
    width = w->width;
    height = 0;
    y = (*((col_i16 *)((unsigned char *)(w) + 0x348)));
    line_h = ctl_line_height((*((FontRec * *)((unsigned char *)(w) + 0x6e))));
    item = (*((Item * *)((unsigned char *)(w) + 0x376)));
    n = 0;
    while (item != 0) {
        if (item->b & 3) {
            s = (*((char * *)((unsigned char *)(item) + 0x2)));
            if (item->b & 1)
                x = (width >> 1) - (font_text_extent_1050_8391((*((FontRec * *)((unsigned char *)(w) + 0x6e))), s) >> 1);
            else
                x = 0;
            ((char * *)((unsigned char *)(w) + 0xac))[n] = s;
            ((col_i16 *)((unsigned char *)(w) + 0x1ac))[n] = strlen(s);
            ((col_i16 *)((unsigned char *)(w) + 0x22c))[n] = (*((col_i16 *)((unsigned char *)(w) + 0x346))) + x + 6;
            ((col_i16 *)((unsigned char *)(w) + 0x2ac))[n] = y + 3;
            n++;
            height += line_h;
            y += line_h;
            x = (*((col_i16 *)((unsigned char *)(w) + 0x346))) + 6;
        } else {
            s = (*((char * *)((unsigned char *)(item) + 0x2)));
            while (s != 0) {
                while ((col_i16)*s == ' ')
                    s++;
                p = s;
                while (p != 0) {
                    prev = p;
                    p = strchr(p, ' ');
                    if (p != 0) {
                        *p = 0;
                        ww = font_text_extent_1050_8391((*((FontRec * *)((unsigned char *)(w) + 0x6e))), s);
                        *p = ' ';
                        p++;
                    } else {
                        ww = font_text_extent_1050_8391((*((FontRec * *)((unsigned char *)(w) + 0x6e))), s);
                    }
                    if (ww + line_w > width - 6) {
                        if (s == prev)
                            stuck = 1;
                        prev--;
                        p = prev;
                        break;
                    }
                }
                if (stuck) {
                    stuck = 0;
                    height += line_h;
                    y += line_h;
                    x = (*((col_i16 *)((unsigned char *)(w) + 0x346))) + 6;
                    line_w = 0;
                    continue;
                }
                if (p != 0) {
                    ((char * *)((unsigned char *)(w) + 0xac))[n] = s;
                    ((col_i16 *)((unsigned char *)(w) + 0x1ac))[n] = p - s;
                    ((col_i16 *)((unsigned char *)(w) + 0x22c))[n] = x;
                    ((col_i16 *)((unsigned char *)(w) + 0x2ac))[n] = y + 3;
                    n++;
                    height += line_h;
                    y += line_h;
                    s = p;
                    x = (*((col_i16 *)((unsigned char *)(w) + 0x346))) + 6;
                    line_w = 0;
                } else {
                    ((char * *)((unsigned char *)(w) + 0xac))[n] = s;
                    ((col_i16 *)((unsigned char *)(w) + 0x1ac))[n] = strlen(s);
                    ((col_i16 *)((unsigned char *)(w) + 0x22c))[n] = x;
                    ((col_i16 *)((unsigned char *)(w) + 0x2ac))[n] = y + 3;
                    n++;
                    line_w += ww + space_w;
                    x += ww + space_w;
                    s = p;
                }
            }
        }
        item = (*((void * *)((unsigned char *)(item) + 0x6)));
    }
    height += line_h;
    ((char * *)((unsigned char *)(w) + 0xac))[n] = 0;
    return height;
}
