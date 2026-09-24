/* sprite_draw_records -- shipped at 1068:1b11.
 *
 * Ported by tools/port.py from matched/gfx/sprite_draw_records.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own RecHdr, Sprite, Resource dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSCopyMem(void *src, void *dst, col_i32 n);
extern void unpack_palette_runs(unsigned char *src, unsigned char *dst);
extern void unpack_palette_runs_x4(unsigned char *src, unsigned char *dst);
extern void wing_call_0000(unsigned char *src, unsigned char *dst, col_i16 x, col_i16 y, col_i16 stride, char add);
extern void wing_call_00e4(unsigned char *src, unsigned char *dst, col_i16 x, col_i16 y, col_i16 stride, char add);
extern void wing_call_0168(unsigned char *src, unsigned char *dst, col_i16 x, col_i16 y, col_i16 stride, char add);
extern void blit_rle_offset(unsigned char *src, unsigned char *dst, col_i16 stride, unsigned char add);
extern void GRPort_FillColor_all(Sprite *s, col_i16 color);


void sprite_draw_records(Sprite *s, unsigned char *data, Resource *res)
{
    RecHdr hdr;
    unsigned char *body;
    col_i16 i;

    for (i = 0; res->count > i; i++) {
        MSCopyMem(data, &hdr, 6L);
        body = data + 6;
        data += hdr.size;
        switch (hdr.kind) {
        case 4:
            unpack_palette_runs(body, ((char *)((unsigned char *)(s) + 0x252)));
            (*((unsigned char *)((unsigned char *)(s) + 0x556))) = 1;
            break;
        case 7:
            wing_call_0000(body, (*((unsigned char * *)((unsigned char *)(s) + 0x18))), (*((col_i16 *)((unsigned char *)(s) + 0x2e))), (*((col_i16 *)((unsigned char *)(s) + 0x30))), (*((col_i16 *)((unsigned char *)(s) + 0x6))), (*((unsigned char *)((unsigned char *)(s) + 0x552))));
            break;
        case 11:
            unpack_palette_runs_x4(body, ((char *)((unsigned char *)(s) + 0x252)));
            (*((unsigned char *)((unsigned char *)(s) + 0x556))) = 1;
            break;
        case 12:
            blit_rle_offset(body, (*((unsigned char * *)((unsigned char *)(s) + 0x18))), (*((col_i16 *)((unsigned char *)(s) + 0x6))), (*((unsigned char *)((unsigned char *)(s) + 0x552))));
            break;
        case 13:
            GRPort_FillColor_all(s, g_draw_color);
            break;
        case 15:
            wing_call_00e4(body, (*((unsigned char * *)((unsigned char *)(s) + 0x18))), (*((col_i16 *)((unsigned char *)(s) + 0x2e))), (*((col_i16 *)((unsigned char *)(s) + 0x30))), (*((col_i16 *)((unsigned char *)(s) + 0x6))), (*((unsigned char *)((unsigned char *)(s) + 0x552))));
            break;
        case 16:
            wing_call_0168(body, (*((unsigned char * *)((unsigned char *)(s) + 0x18))), (*((col_i16 *)((unsigned char *)(s) + 0x2e))), (*((col_i16 *)((unsigned char *)(s) + 0x30))), (*((col_i16 *)((unsigned char *)(s) + 0x6))), (*((unsigned char *)((unsigned char *)(s) + 0x552))));
            break;
        default:
            break;
        }
    }
}
