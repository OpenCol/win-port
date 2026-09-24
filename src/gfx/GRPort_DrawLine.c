/* GRPort_DrawLine -- shipped at 1068:5bc6.
 *
 * Ported by tools/port.py from matched/gfx/GRPort_DrawLine.c.
 *
 * The file's own GRPort dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: left -> rect
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSDrawLine(void *hdc, col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2);

void GRPort_DrawLine(GRPort *p, col_i16 x1, col_i16 y1, col_i16 x2, col_i16 y2)
{
    if (p->rect > x1 && p->rect > x2)
        return;
    if ((*((col_i16 *)((unsigned char *)(p) + 0xc))) < x2 && p->rect < x1)
        return;
    if ((*((col_i16 *)((unsigned char *)(p) + 0xa))) > y1 && (*((col_i16 *)((unsigned char *)(p) + 0xa))) > y2)
        return;
    if ((*((col_i16 *)((unsigned char *)(p) + 0xe))) < y1 && (*((col_i16 *)((unsigned char *)(p) + 0xe))) < y2)
        return;
    if (p->rect > x1) x1 = p->rect;
    if ((*((col_i16 *)((unsigned char *)(p) + 0xc))) < x1) x1 = (*((col_i16 *)((unsigned char *)(p) + 0xc)));
    if (p->rect > x2) x2 = p->rect;
    if ((*((col_i16 *)((unsigned char *)(p) + 0xc))) < x2) x2 = (*((col_i16 *)((unsigned char *)(p) + 0xc)));
    if ((*((col_i16 *)((unsigned char *)(p) + 0xa))) > y1) y1 = (*((col_i16 *)((unsigned char *)(p) + 0xa)));
    if ((*((col_i16 *)((unsigned char *)(p) + 0xe))) < y1) y1 = (*((col_i16 *)((unsigned char *)(p) + 0xe)));
    if ((*((col_i16 *)((unsigned char *)(p) + 0xa))) > y2) y2 = (*((col_i16 *)((unsigned char *)(p) + 0xa)));
    if ((*((col_i16 *)((unsigned char *)(p) + 0xe))) < y2) y2 = (*((col_i16 *)((unsigned char *)(p) + 0xe)));
    MSDrawLine((*((void * *)((unsigned char *)(p) + 0x22))), x1, y1, x2, y2);
}
