/* gif_to_pic: candidate for 1068:bd72 (1143 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: gif_to_pic
 *
 * CONVERTS A GIF FILE INTO THE GAME'S OWN PICTURE FILE. It parses the GIF exactly as
 * `load_gif_file` does -- the whole file is read into a handle, the signature and the global
 * colour map bit checked, the colour map taken as `1 << ((byte 10 & 7) + 1)` entries of three bytes,
 * the blocks skipped to the first 0x2c or 0x21, and a local colour table warned about -- but instead of
 * decompressing it WRITES:
 *   - a six-byte header: the image width and height byte-swapped, the LZW code size, and the colour
 *     count less one;
 *   - the colour map, three bytes an entry, out of a 768-byte local copy;
 *   - the rest of the file from the byte after the code size to the end, as one block.
 * The length of that last write is a `` pointer subtraction, which is the `SHR 3` pair.
 *
 * The five error messages are this module's own copies of the same strings. The "resource not found"
 * arm tests the handle a second time and can never run, as in the loader.
 *
 * The header's colour count is a SIGNED `char`: that is what makes the narrowing `DEC AL` rather than
 * `ADD AL,-1` (rule 76, added for this function). The header struct is eight bytes with two written
 * only by the frame, and the 6 in the write call is the size the shipped code passes.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int file_MSOpenFile_1068_dbc0(DiskFile *file, char *name);

extern "C" void report_error_1068_dc30(void *ctx, char *msg, char *name);
extern "C" void report_error_1068_dc3e(void *ctx, char *msg);

extern "C" void MSWriteFile_1068_dc9a(DiskFile *out, void *buf, long n);

void gif_to_pic(char *inname, char *outname)
{
    DiskFile in;
    DiskFile out;
    unsigned char *data;
    ImgDesc *img;
    unsigned char code;
    int w_raw;
    int h_raw;
    int ncol;
    unsigned char *q;
    unsigned char *r;
    unsigned char *tmp;
    unsigned char *end;
    PicHead head;
    unsigned char buf[0x300];
    register short h;

    file_MSOpenFile_1068_dbc0(&in, inname);
    h = MSAllocateHandle(MSFileSize_1068_dbe6(&in));
    tmp = (unsigned char *)MSHLock(h);
    save_read_field_1068_dc06(&in, tmp, MSFileSize_1068_dbe6(&in));
    end = (unsigned char *)tmp + MSFileSize_1068_dbe6(&in);
    tmp = (unsigned char *)MSHUnlock(h);
    if (h == 0) {
        report_error_1068_dc30(&g_error_log, "Error: GIF resource not found - ", inname);
        return;
    }
    data = (unsigned char *)MSHLock(h);
    if (STRNCMP((char *)data, "GIF", 3) != 0) {
        report_error_1068_dc30(&g_error_log, "Error: Resource is not a GIF - ", inname);
        MSHUnlock(h);
        MSDisposeHandle_1068_9407(h);
        return;
    }
    if (!(data[0xa] & 0x80)) {
        report_error_1068_dc30(&g_error_log, "Error: GIF contains no global color map - ", inname);
        MSHUnlock(h);
        MSDisposeHandle_1068_9407(h);
        return;
    }
    ncol = 1 << ((data[0xa] & 7) + 1);
    q = data + 0xd;
    memcpy_rtl(buf, q, ncol * 3);
    q = data + ncol * 3 + 0xd;
    while (*q == 0)
        q++;
    if (*q != 0x2c && *q != 0x21) {
        report_error_1068_dc30(&g_error_log, "Error: GIF Image Block not found - ", inname);
        MSHUnlock(h);
        MSDisposeHandle_1068_9407(h);
        return;
    }
    img = (ImgDesc *)(q + 1);
    w_raw = img->w;
    h_raw = img->h;
    if (img->flags & 0x80)
        report_error_1068_dc3e(&g_error_log, "Warning: Skipping local color table");
    r = (unsigned char *)((unsigned char *)img + 9);
    code = *r;
    r++;
    head.w = byte_swap_1068_dc4c(w_raw);
    head.h = byte_swap_1068_dc4c(h_raw);
    head.code = code;
    head.ncol = ncol - 1;
    MSCreateFile_1068_dc74(&out, outname);
    MSWriteFile_1068_dc9a(&out, &head, 6L);
    MSWriteFile_1068_dc9a(&out, buf, (unsigned)(ncol * 3));
    MSWriteFile_1068_dc9a(&out, r, end - r);
    h = MSDisposeHandle(h);
    MSCloseFile_1068_dcc4(&in);
}

