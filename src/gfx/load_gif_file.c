/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 81, 87, 94, 111 of the imported file. */
/* PORT: this file's own declarations of report_error_1068_9839, wing_dib_flip removed: the header declares them from the definition (W1 step 3). */
/* load_gif_file: candidate for 1068:43ec (1208 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: load_gif_file
 *
 * `load_gif_resource` READ FROM A FILE instead of a resource: the same GIF parsing, the same
 * five error messages and the same decompression, but the bytes come from a `DiskFile` opened by name
 * and held in a memory handle.
 *
 * The file is opened, a handle its size is allocated, the whole file read into it, and the block is
 * unlocked and locked again before parsing. The allocation failing returns 0 at once; the "resource not
 * found" arm tests the same handle a second time and so can never run.
 *
 * PARSING, as in the resource version: the first three bytes must be "GIF" (here the test is the right
 * way round, where the resource version's `STRCMP` test is inverted); byte 10 must have its global
 * colour map bit; the map holds `1 << ((byte 10 & 7) + 1)` entries and goes into the palette when one
 * is passed, clamped to 256 entries; zero padding bytes are skipped and the next byte must be 0x2c or 0x21; the image's
 * width and height go through the module's byte-swap (the identity on this build) and are written back; a local colour table is warned about and ignored; the
 * port's virtual `ResizePort` is given the image rectangle and can refuse; and `MSPicDecompress` unpacks the
 * pixels, after which the DIB is flipped unless the window is already the right way up.
 *
 * THE SUCCESS PATH FALLS OFF THE END without a return value, where every failure returns 0 -- so the
 * caller's `int` is whatever `MSCloseFile` left in AX. That also makes this the one exit that restores
 * the exception word through AX rather than DX (rule 41).
 *
 * The class's members sit two bytes below their real offsets because the vtable pointer takes +0 --
 * `stride` is declared at +4 and read at +6, `bits` at +0x16 and read at +0x18, `win` at +0x20 and read
 * at +0x22. Getting that wrong moves every member access by two bytes and nothing else.
 *
 * `pal` is the CTable every caller passes (2026-10-02; it was a `struct logpal *`, used
 * only cast to CTable).
 */

#include "colonization.h"

#include "w16types.h"
#include "grerror.h"
#include "records.h"
/* PORT: this file's own DiskFile, GRPort, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

struct logpal;

extern void error_code_set(int code);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void GRPort_ColorSync(GRPort *p, CTable *pal);
extern int MSGrafOrient(GrafRec *graf);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);


extern "C" void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern "C" void report_error_1068_963d(void *ctx, char *msg);

int load_gif_file(GRPort *port, char *name, register int first, int count,
                                CTable *pal)
{
    DiskFile f;
    unsigned char *data;
    ImgDesc *img;
    unsigned char code;
    RECT rc;
    int w;
    int h;
    int ncol;
    unsigned char *q;
    unsigned char *r;
    unsigned char *tmp;
    register short h_;

    file_MSOpenFile_1068_95d5(&f, name);
    h_ = MSAllocateHandle(MSFileSize_1068_9819(&f));
    if (h_ == 0)
        return 0;
    tmp = (unsigned char *)MSHLock(h_);
    save_read_field_1068_95fb(&f, tmp, MSFileSize_1068_9819(&f));
    tmp = (unsigned char *)MSHUnlock(h_);
    if (h_ == 0) {
        report_error_1068_9839(&g_error_log, (void*)("Error: GIF resource not found - "), name);
        error_code_set(GRERR_NOT_FOUND);
        return 0;
    }
    data = (unsigned char *)MSHLock(h_);
    if (STRNCMP((char *)data, "GIF", 3) != 0) {
        report_error_1068_9839(&g_error_log, (void*)("Error: Resource is not a GIF - "), name);
        MSHUnlock(h_);
        MSDisposeHandle(h_);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    if (!(data[0xa] & 0x80)) {
        report_error_1068_9839(&g_error_log, (void*)("Error: GIF contains no global color map - "), name);
        MSHUnlock(h_);
        MSDisposeHandle(h_);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    ncol = 1 << ((data[0xa] & 7) + 1);
    q = data + 0xd;
    if (pal != 0) {
        if (first + count > 0x100)
            count = 0x100 - first;
        CTable_Fill(pal, first, min_int_1068_9847(ncol, count), (unsigned char *)q);
    }
    q = data + ncol * 3 + 0xd;
    while (*q == 0)
        q++;
    if (*q != 0x2c && *q != 0x21) {
        report_error_1068_9839(&g_error_log, (void*)("Error: GIF Image Block not found - "), name);
        MSHUnlock(h_);
        MSDisposeHandle(h_);
        error_code_set(GRERR_BAD_FORMAT);
        return 0;
    }
    img = (ImgDesc *)(q + 1);
    img->w = w = swap_word_identity_1068_9518(img->w);
    img->h = h = swap_word_identity_1068_9518(img->h);
    if (img->flags & 0x80)
        report_error_1068_963d(&g_error_log, "Warning: Skipping local color table");
    SetRect(&rc, 0, 0, w, h);
    if (port->ResizePort(&rc) == 0) {
        MSDisposeHandle(h_);
        return 0;
    }
    r = (unsigned char *)((unsigned char *)img + 9);
    code = *r;
    r++;
    MSPicDecompress(r, first, code, port->stride, w, h, port->bits);
    GRPort_ColorSync(port, pal);
    if (MSGrafOrient((GrafRec *)port->win) == 0)
        wing_dib_flip((Wing *)port->win);
    MSDisposeHandle(h_);
    /* PORT: the success path fell off the end with MSCloseFile's AX as the result: returned (W4) */
    return MSCloseFile_1068_95b5(&f);
}

