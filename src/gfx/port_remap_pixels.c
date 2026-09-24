/* port_remap_pixels -- shipped at 1068:62fe.
 *
 * Ported by tools/port.py from matched/gfx/port_remap_pixels.c.
 *
 * The file's own Port dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: height -> hdc, width -> hwnd
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void port_remap_pixels(Port *p, unsigned char *table, col_i16 d, col_i16 count)
{
    unsigned char *row;
    col_i16 y;
    col_i16 x;

    if ((*((unsigned char * *)((unsigned char *)(p) + 0x18))) != 0) {
        row = (*((unsigned char * *)((unsigned char *)(p) + 0x18)));
        for (y = 0; p->hdc > y; y++, row += (*((col_i16 *)((unsigned char *)(p) + 0x6))) - p->hwnd) {
            for (x = 0; p->hwnd > x; x++, row++) {
                if (*row - d >= 0)
                    *row = table[*row - d];
            }
        }
    }
}
