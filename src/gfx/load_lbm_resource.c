/* load_lbm_resource: candidate for 1068:3811 (1460 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_lbm_resource
 *
 * Loads an IFF picture resource (type at DGROUP:0x2afe) into a port: the LBM member of the picture
 * loaders beside `load_pcx_resource` and `load_gif_resource`. The resource must
 * start "FORM" and be "ILBM" or "PBM " ("Error: LBM Resource must have FORM - ", "... is not a
 * ILBM/PBM - "). Chunks are then walked, each size byte-swapped and rounded up to even:
 *
 *   BMHD  copied out and its words byte-swapped; more than 8 planes fails without a message
 *   CMAP  set as the palette from `first` when a palette record is given, and skipped over only then
 *   BODY  ends the walk; anything else is skipped
 *
 * A 1 by 1 image is treated as 0 by 0. The port is resized through its virtual to the header's width
 * -- rounded up to even for 4 planes or fewer -- by its height, and the body is decoded one row at a
 * time into the port's bitmap, which is advanced a row at a time and put back at the end. Only a PBM
 * body is decoded: uncompressed, or ByteRun1 (a count 0..127 of literals to follow, -1..-127 for a
 * repeat of the next byte, -128 a no-op); an ILBM image leaves the bitmap as it was. Every pixel has
 * `first` added.
 *
 * Two quirks the bytes insist on. A CMAP with no palette record is not skipped, so the walk reads
 * its colour bytes as the next chunk header. And the resource is freed at the end without being
 * unlocked, where the FORM and ILBM/PBM error paths unlock it first (the more-than-8-planes exit
 * neither unlocks nor frees it).
 *
 * Two spellings the bytes insist on too. The row counter and the run count are declared inside the
 * PBM block: at function scope the counter takes DI and the run count goes to the stack, because
 * SI is the scratch register of the -pointer row advance in that same scope (rule 68; all six
 * orderings of function-scope declarations give the 0x4a frame). And the ILBM arm is an empty
 * `else`: without it the jump shape differs.
 */

#include "colonization.h"

#include "w16types.h"
#include "grerror.h"
#include "records.h"
/* PORT: this file's own GRPort, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
typedef struct { char id[4]; unsigned long size; } IffChunk;
struct logpal;

extern void error_code_set(int code);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *p, CTable *pal);
extern int MSGrafOrient(GrafRec *graf);
extern void wing_dib_flip(void *win);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *MSGetResource_1068_94c7(void *type, int id);
extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);
extern "C" void *MSLockResource_1068_94f8(void *h);
extern "C" void MSUnlockResource_1068_952d(void *res);
extern "C" void MSReleaseResource_1068_9549(void *res);

extern "C" void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

int load_lbm_resource(GRPort *port, int id, int first, int count,
                                    struct logpal *pal)
{
    IffChunk ch;
    long type;
    void *res;
    unsigned long total;
    unsigned long pos;
    unsigned w;
    unsigned h;
    unsigned char *data;
    int pbm;
    int ilbm;
    int rowbytes;
    RECT rc;
    unsigned char *bits;
    int pixel;
    Bmhd bmhd;

    type = g_lbm_resource_type;
    res = MSGetResource_1068_94c7(&type, id);
    if (res == 0) {
        report_error_1068_94ea(&g_error_log, "Error: LBM resource not found - ", (long)id);
        error_code_set(GRERR_NOT_FOUND);
        return 0;
    }
    data = (unsigned char *)MSLockResource_1068_94f8(res);
    MSCopyMem(data, &ch, 8L);
    if (STRNCMP(ch.id, "FORM", 4) != 0) {
        report_error_1068_94ea(&g_error_log, "Error: LBM Resource must have FORM - ", (long)id);
        MSUnlockResource_1068_952d(res);
        MSReleaseResource_1068_9549(res);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    data += 8;
    MSCopyMem(data, &ch, 4L);
    ilbm = STRNCMP(ch.id, "ILBM", 4) == 0;
    pbm = STRNCMP(ch.id, "PBM ", 4) == 0;
    if (ilbm == 0 && pbm == 0) {
        report_error_1068_94ea(&g_error_log, "Error: LBM Resource is not a ILBM/PBM - ", (long)id);
        MSUnlockResource_1068_952d(res);
        MSReleaseResource_1068_9549(res);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    data += 4;
    for (;;) {
        MSCopyMem(data, &ch, 8L);
        data += 8;
        ch.size = swap_long(ch.size);
        ch.size = (ch.size + 1) & 0xfffffffeL;
        if (STRNCMP(ch.id, "BMHD", 4) == 0) {
            MSCopyMem(data, &bmhd, ch.size);
            data += ch.size;
            bmhd.w = byte_swap_1068_97f1(bmhd.w);
            bmhd.h = byte_swap_1068_97f1(bmhd.h);
            bmhd.x = byte_swap_1068_97f1(bmhd.x);
            bmhd.y = byte_swap_1068_97f1(bmhd.y);
            bmhd.transparent = byte_swap_1068_97f1(bmhd.transparent);
            bmhd.pagew = byte_swap_1068_97f1(bmhd.pagew);
            bmhd.pageh = byte_swap_1068_97f1(bmhd.pageh);
            if (bmhd.nplanes > 8) {
                error_code_set(GRERR_BAD_FORMAT);
                return 0;
            }
            continue;
        } else if (STRNCMP(ch.id, "CMAP", 4) == 0) {
            if (pal != 0) {
                if (first + count > 0x100)
                    count = 0x100 - first;
                CTable_Fill((CTable *)pal, first, count, data);
                data += ch.size;
            }
            continue;
        } else if (STRNCMP(ch.id, "BODY", 4) == 0) {
            break;
        } else {
            data += ch.size;
        }
    }
    if (bmhd.w == 1 && bmhd.h == 1)
        bmhd.w = bmhd.h = 0;
    if (pbm != 0)
        rowbytes = (bmhd.w + 1) & 0xfffe;
    else
        rowbytes = ((bmhd.w + 0xf) >> 4) * 2;
    w = bmhd.w;
    h = bmhd.h;
    if (bmhd.nplanes <= 4) {
        w++;
        w &= 0xfffe;
    }
    total = (unsigned long)h * (unsigned long)w;
    SetRect(&rc, 0, 0, w, h);
    port->ResizePort(&rc);
    bits = port->bits;
    for (pos = 0; pos < total; pos += w) {
        if (pbm != 0) {
            register int x;
            register int n;
            x = 0;
            switch (bmhd.compression) {
            case 0:
                for (; x < rowbytes; x++) {
                    port->bits[x] = *data + first;
                    data++;
                }
                break;
            case 1:
                while (x < rowbytes) {
                    n = (signed char)*data;
                    data++;
                    if (n >= 0) {
                        while (n-- >= 0) {
                            port->bits[x] = *data + first;
                            data++;
                            x++;
                        }
                    } else if (n >= -127) {
                        pixel = (signed char)*data;
                        data++;
                        while (n++ <= 0) {
                            port->bits[x] = pixel + first;
                            x++;
                        }
                    }
                }
                break;
            }
        } else {
            /* ILBM bodies are not decoded */
        }
        port->bits += (long)port->stride;
    }
    GRPort_ColorSync(port, (CTable *)pal);
    if (MSGrafOrient((GrafRec *)port->win) == 0)
        wing_dib_flip((Wing *)port->win);
    port->bits = bits;
    MSReleaseResource_1068_9549(res);
    return 1;
}

