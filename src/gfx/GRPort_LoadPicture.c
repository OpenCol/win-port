/* PORT: this file's own declarations of wing_dib_flip removed: the header declares it from the definition (W1 step 3). */
/* GRPort_LoadPicture: candidate for 1068:4c05 (437 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_LoadPicture
 *
 * Loads a picture resource (type at DGROUP:0x2b0a) into a port: a miss is reported ("Error:
 * Picture resource not found - ", id) and error_code_set(3) called; otherwise the locked data
 * gives the byte-swapped width and height, the port is resized through its virtual (a
 * refusal frees the resource), the palette entries that follow the 6-byte header are set
 * from `first` for `count` when a palette record is given, the pixel data after them is
 * decompressed into the port's bitmap, the port's palette selected if it changed, its window's
 * DIB flipped (wing_dib_flip) when MSGrafOrient reports 0, and the resource freed. The module's resource wrappers and
 * CTable_Fill are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `; error_code_set and GRPort_ColorSync are calls TLINK converted.
 *
 * RENAMED 2026-09-27 from load_picture_resource: Mac GRPort::LoadPicture(short, short, short, CTable&), 324 bytes, against 437. Callers: the Mac callers are among the Windows ones (KingRoom, Fame, HallOfFame, Customize Init, ColonyWin LoadColony, EuropeWin LoadEurope, MSInitImageButton, DoExtendedLaborReport, OnColonyExtraClick...). Seq: MSGetResource, error, MSLockResource, MSReleaseResource, CTable::Fill, MSPicDecompress, ColorSync, MSGrafOrient, MSInvertGraf (wing_dib_flip), MSReleaseResource. That is the Windows set, with free_resource_1068_9549 x2. Layout: Mac GRPort::<dtor>, LoadPicture, ResetClip, SetClip, GetClip; Windows load_gif_resource, 4c05, GRPort_ResetClip, SetClip, GetClip. Arity: this+4 = (port, id, first, count, pal). mac_bind: strong (align 19.5/0, votes 32/22).
 */

#include "colonization.h"

#include "w16types.h"
#include "grerror.h"
#include "records.h"
/* PORT: this file's own GRPort, CTable removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

struct logpal;

extern void *MSGetResource(char *type, int id);
extern void *MSLockResource(void *h);
extern void MSReleaseResource(void *h);
extern void error_code_set(int code);
extern void MSSetCTEntry(struct logpal *pal, int i, unsigned char r, unsigned char g, unsigned char b);
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern void CTable_UpdatePal(CTable *pal, int start, int count);

extern void GRPort_ColorSync(GRPort *p, CTable *pal);
extern int MSGrafOrient(GrafRec *graf);

extern "C" void *MSGetResource_1068_94c7(void *type, int id);

extern "C" void report_error_1068_94ea(void *ctx, char *msg, long v);

extern "C" void *MSLockResource_1068_94f8(void *h);

extern "C" void MSReleaseResource_1068_9549(void *res);

extern "C" void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

int GRPort_LoadPicture(GRPort *port, int id, int first, int count, struct logpal *pal)
{
    void *type;
    void *res;
    PicHeader *d;
    RECT rc;
    int w;
    int h;
    int n;
    unsigned char *colors;
    unsigned char *pix;

    type = g_picture_resource_type;
    res = MSGetResource_1068_94c7(&type, id);
    if (res == 0) {
        report_error_1068_94ea(&g_error_log, "Error: Picture resource not found - ", (long)id);
        error_code_set(GRERR_NOT_FOUND);
        return 0;
    }
    d = (PicHeader *)MSLockResource_1068_94f8(res);
    w = byte_swap_1068_97f1(d->w);
    h = byte_swap_1068_97f1(d->h);
    SetRect(&rc, 0, 0, w, h);
    if (port->ResizePort(&rc) == 0) {
        MSReleaseResource_1068_9549(res);
        return 0;
    }
    n = d->colors + 1;
    colors = (unsigned char *)d + 6;
    if (pal != 0)
        CTable_Fill((CTable *)pal, first, count, colors);
    pix = (unsigned char *)d + n * 3 + 6;
    MSPicDecompress(pix, first, d->kind, port->stride, w, h, port->bits);
    GRPort_ColorSync(port, (CTable *)pal);
    if (MSGrafOrient((GrafRec *)port->win) == 0)
        wing_dib_flip((Wing *)port->win);
    MSReleaseResource_1068_9549(res);
    return 0;   /* PORT: the success path had no return value (AX after a void call); no caller reads it (W4) */
}

