/* PORT: this file's own declarations of wing_dib_flip removed: the header declares it from the definition (W1 step 3). */
/* load_gif_resource: candidate for 1068:48a4 (865 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_gif_resource
 * size-source: 865, not the census 856. The trailing 9 bytes are the dead last epilogue.
 *
 * Loads a GIF resource (type at DGROUP:0x2b06) into a port, the GIF member of the picture loaders
 * beside `load_pcx_resource` and `GRPort_LoadPicture`. The global colour map follows the
 * 13-byte screen descriptor; with a palette record given, its first min(2^(bits+1), count) entries
 * are set from `first`. Padding zeros are skipped to the image descriptor, whose width and height
 * are passed through the module's byte-swap (the identity on this build) and written back; a local
 * colour table only earns a warning. The port is resized through its virtual -- the answer is not
 * checked -- and the LZW data after the descriptor, headed by its code size, goes to
 * `MSPicDecompress`. The resource is unlocked and freed.
 *
 * THE SIGNATURE TEST IS BACKWARDS IN EFFECT. "Error: Resource is not a GIF - " is reported when
 * `STRCMP(data, "GIF")` returns ZERO -- that is, only for data that is exactly the three bytes and a
 * NUL. A real file starts "GIF87a" or "GIF89a", compares unequal, and passes, which is why the loader
 * works; so would most data that is not a GIF at all. The other errors: no resource, no global colour
 * map (bit 7 of byte +0xa), and no image block (neither ',' nor '!' after the padding).
 *
 * `ncol * 3` compiles to `ADD AX,AX / ADD AX,[ncol]`, which reads like a doubling written by hand;
 * every hand spelling of it adds the pointer in AX instead of BX, and only the multiply matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "grerror.h"
#include "records.h"
/* PORT: this file's own GRPort, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
struct logpal;

extern void error_code_set(int code);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *p, CTable *pal);
extern int MSGrafOrient(GrafRec *graf);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void *MSGetResource_1068_94c7(void *type, int id);
extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);
extern "C" void *MSLockResource_1068_94f8(void *h);
extern "C" void MSUnlockResource_1068_952d(void *res);
extern "C" void MSReleaseResource_1068_9549(void *res);

extern "C" void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern "C" void report_error_1068_963d(void *ctx, char *msg);

int load_gif_resource(GRPort *port, register int id, register int first, int count,
                                    struct logpal *pal)
{
    long type;
    void *res;
    unsigned char *data;
    ImgDesc *img;
    unsigned char code;
    RECT rc;
    int w;
    int h;
    int ncol;
    unsigned char *q;
    unsigned char *r;

    type = g_gif_resource_type;
    res = MSGetResource_1068_94c7(&type, id);
    if (res == 0) {
        report_error_1068_94ea(&g_error_log, "Error: GIF resource not found - ", (long)id);
        error_code_set(GRERR_NOT_FOUND);
        return 0;
    }
    data = (unsigned char *)MSLockResource_1068_94f8(res);
    if (STRCMP((char *)data, "GIF") == 0) {
        report_error_1068_94ea(&g_error_log, "Error: Resource is not a GIF - ", (long)id);
        MSUnlockResource_1068_952d(res);
        MSReleaseResource_1068_9549(res);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    if (!(data[0xa] & 0x80)) {
        report_error_1068_94ea(&g_error_log, "Error: GIF contains no global color map - ", (long)id);
        MSUnlockResource_1068_952d(res);
        MSReleaseResource_1068_9549(res);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    ncol = 1 << ((data[0xa] & 7) + 1);
    q = data + 0xd;
    if (pal != 0) {
        if (first + count > 0x100)
            count = 0x100 - first;
        CTable_Fill((CTable *)pal, first, min_int_1068_9847(ncol, count), (unsigned char *)q);
    }
    q = data + ncol * 3 + 0xd;
    while (*q == 0)
        q++;
    if (*q != 0x2c && *q != 0x21) {
        report_error_1068_94ea(&g_error_log, "Error: GIF Image Block not found - ", (long)id);
        MSUnlockResource_1068_952d(res);
        MSReleaseResource_1068_9549(res);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    img = (ImgDesc *)(q + 1);
    img->w = w = swap_word_identity_1068_9518(img->w);
    img->h = h = swap_word_identity_1068_9518(img->h);
    if (img->flags & 0x80)
        report_error_1068_963d(&g_error_log, "Warning: Skipping local color table");
    SetRect(&rc, 0, 0, w, h);
    port->ResizePort(&rc);
    r = (unsigned char *)img + 9;
    code = *r;
    r++;
    MSPicDecompress(r, first, code, port->stride, w, h, port->bits);
    GRPort_ColorSync(port, (CTable *)pal);
    if (MSGrafOrient((GrafRec *)port->win) == 0)
        wing_dib_flip((Wing *)port->win);
    MSUnlockResource_1068_952d(res);
    MSReleaseResource_1068_9549(res);
    return 1;
}

