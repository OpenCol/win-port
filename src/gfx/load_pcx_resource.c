/* PORT: this file's own declarations of wing_dib_flip removed: the header declares it from the definition (W1 step 3). */
/* load_pcx_resource: candidate for 1068:40d2 (766 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_pcx_resource
 * size-source: 766, through the dead last epilogue to the next function's exception data.
 *
 * Loads a PCX resource (type at DGROUP:0x2b02) into a port -- the PCX member of the four picture
 * loaders `sprite_load_*` choose between, and `GRPort_LoadPicture`'s structure throughout. The
 * 128-byte header is copied out and checked: "Error: PCX resource not found - ", "Error: Not a PCX
 * file - " (manufacturer byte not 10 and encoding 0) or "Error: Not a 256 color or 1plane PCX
 * resource - " (bits per pixel not 8 and planes not 1) go to the warning context with the id. The
 * port is resized through its virtual to (xmax + 1) by (ymax + 1); then the run-length data is
 * decoded row by row into the port's bitmap -- a byte with both top bits set is a count of the byte
 * that follows -- with `first` added to every pixel. A 0x0C marker after the image introduces the
 * 768-byte palette, set from `first` for at most `count` entries. The port's palette is selected,
 * its DIB flipped unless it is already the right way up, and the resource freed.
 *
 * A port that refuses the new size returns 0 without freeing the resource; the two header errors
 * unlock and free it first. Both header tests use `&&` where `||` would reject more files: a PCX
 * is refused only when BOTH of its bytes are wrong.
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

int load_pcx_resource(GRPort *port, int id, int first, int count,
                                    struct logpal *pal)
{
    long type;
    void *res;
    RECT rc;
    unsigned w;
    unsigned h;
    char run;
    unsigned char pixel;
    unsigned char *data;
    PcxHeader hdr;
    register int x;
    register int y;

    type = g_pcx_resource_type;
    res = MSGetResource_1068_94c7(&type, id);
    if (res == 0) {
        report_error_1068_94ea(&g_error_log, "Error: PCX resource not found - ", (long)id);
        error_code_set(GRERR_NOT_FOUND);
        return 0;
    }
    data = (unsigned char *)MSLockResource_1068_94f8(res);
    MSCopyMem(data, &hdr, 0x80L);
    data += 0x80;
    if ((int)hdr.manufacturer != 10 && hdr.encoding == 0) {
        report_error_1068_94ea(&g_error_log, "Error: Not a PCX file - ", (long)id);
        MSUnlockResource_1068_952d(res);
        MSReleaseResource_1068_9549(res);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    if ((int)hdr.bpp != 8 && (int)hdr.planes != 1) {
        report_error_1068_94ea(&g_error_log, "Error: Not a 256 color or 1plane PCX resource - ", (long)id);
        MSUnlockResource_1068_952d(res);
        MSReleaseResource_1068_9549(res);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    w = hdr.xmax + 1;
    h = hdr.ymax + 1;
    SetRect(&rc, 0, 0, w, h);
    if (port->ResizePort(&rc) == 0)
        return 0;
    run = 0;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            if (run == 0) {
                run = *data;
                data++;
                pixel = run;
                if (((unsigned char)run & 0xc0) == 0xc0) {
                    run &= 0x3f;
                    pixel = *data;
                    data++;
                } else {
                    run = 1;
                }
            }
            port->bits[(unsigned)(port->stride * y + x)] = pixel + first;
            run--;
        }
    }
    pixel = *data;
    data++;
    if (pixel == 0xc) {
        if (first + count > 0x100)
            count = 0x100 - first;
        CTable_Fill((CTable *)pal, first, count, data);
        data += 0x300;
    }
    GRPort_ColorSync(port, (CTable *)pal);
    if (MSGrafOrient((GrafRec *)port->win) == 0)
        wing_dib_flip((Wing *)port->win);
    MSReleaseResource_1068_9549(res);
    return 1;
}

