/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 92 of the imported file. */
/* PORT: this file's own declarations of MSOpenFile removed: the header declares it from the definition (W1 step 3). */
/* Flic_Init: candidate for 1068:1f42 (458 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_Init
 *
 * Opens a FLIC animation: the file (DiskFile at +0xa6) by name -- a miss is reported twice
 * ("Error: FLIC file not found. " and the name) -- the name kept at +0x132, the 128-byte
 * header read to +0x26, the port sized to the header's width and height through the
 * virtual resize, FLI (0xaf11: frame offset 128, speed converted from 1/70 s to ms) or FLC
 * (0xaf12) positioning, a 310000-byte frame buffer allocated and locked, the frame
 * bookkeeping reset and the first frame prepared (Flic_NextFrame). The module's file
 * wrappers are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
/* PORT: this file's own Flic removed: include/gen/classes.h define it once (W1 step 2). */

extern int MSReadFile(MSFile *file, void *buf, long n);
extern int MSSeekFile(MSFile *file, long pos);

extern void Flic_NextFrame(Flic *flic);

extern "C" int file_MSOpenFile_1068_95d5(DiskFile *file, char *name);

extern "C" void report_error_1068_963d(void *ctx, char *msg);

void Flic_Init(Flic *flic, char *name)
{
    RECT rc;

    if (file_MSOpenFile_1068_95d5(&flic->file, name) == 0) {
        report_error_1068_963d(&g_error_log, "Error: FLIC file not found. ");
        report_error_1068_963d(&g_error_log, name);
        return;
    }
    STRCPY(flic->name, name);
    save_read_field_1068_95fb(&flic->file, flic->header, 128L);
    SetRect(&rc, 0, 0, flic->width, flic->height);
    flic->ResizePort(&rc);
    if (flic->magic == 0xaf11) {
        flic->frame_offset = 128;
        MSSeekFile_1068_964b(&flic->file, flic->frame_offset - 128);
        flic->speed = flic->speed * 1000 / 70;
    } else if (flic->magic == 0xaf12)
        MSSeekFile_1068_964b(&flic->file, flic->frame_offset - 128);
    flic->hbuf = MSAllocateHandle(310000L);
    flic->buf = (char *)MSHLock(flic->hbuf);
    flic->pos = (unsigned char*)(flic->buf);
    flic->frame_no = flic->rect_index = 0;
    flic->timer = -1;
    Flic_NextFrame(flic);
    flic->frame2 = flic->pos;
}

