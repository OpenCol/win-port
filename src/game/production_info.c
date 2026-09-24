/* production_info -- shipped at 1018:5324.
 *
 * Ported by tools/port.py from matched/game/production_info.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char g_4ba5;                         DGROUP:0x4ba5
 *   extern int far g_prod[];                             SEG21:0x0122
 *   extern int far g_tbl21_132;                          SEG21:0x0132
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 job_goods(col_i16 job);
extern col_i16 job_workers(col_i16 job);

col_i16 production_info(col_i16 job, col_i16 *kind_out, col_i16 *pic_out, col_i16 *extra_out)
{
    col_i16 amount = 0;
    register col_i16 kind = -1;
    register col_i16 pic = 0;
    col_i16 t;
    col_i16 u;
    col_i16 extra = 0;

    if (job == 0x13 || job == 0x14) {
        amount = g_4ba5;
        pic = 0x3f;
    } else if (job == 0x11) {
        amount = g_tbl21_132;
        pic = 0x1f;
    } else {
        t = job_goods(job);
        if (t < 0)
            goto done;
        u = job_workers(job);
        if (u < 0)
            goto done;
        switch (t) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 14:
        case 15:
            kind = t;
            pic = t + 0x17;
            break;
        case 13:
            kind = 0x10;
            pic = 0x37;
            break;
        case 16:
            kind = 0x11;
            pic = 0x39;
            break;
        case 17:
            kind = 0x12;
            pic = 0x3f;
            break;
        }
        if (kind > 0)
            amount = g_prod[kind];
        if (t == 0x11)
            amount -= g_4ba5;
    }
done:
    if (kind_out != 0)
        *kind_out = kind;
    if (pic_out != 0)
        *pic_out = pic;
    if (extra_out != 0)
        *extra_out = extra;
    return amount;
}
