/* save_frr_file: candidate for 1068:2a7a (357 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: save_frr_file
 *
 * Writes an object's rectangle cache next to its file: the name at +0x132 with its last four
 * characters replaced by ".frr" (DGROUP:0x2bf3). The file holds the four bytes at +0x3c, the
 * count at +0x568, `count` words from the pointer at +0x560 and `count` 8-byte rectangles
 * from the one at +0x55c. The answer is 0 when the file cannot be created and 1 when it was
 * written. Its caller hands "Total rects calculated:" (DGROUP:0x2bdb) to the empty debug routine
 * `report_error_1068_94ea` just before.
 *
 * The two sizes are `unsigned` products widened to `long` -- `MOVZX EAX,AX` -- and the doubling
 * is `ADD AX,AX` where the times eight is `SHL AX,3`.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Flic : public GRPort {
public:
    char header[4];                     /* +0x26 */
    unsigned magic;                     /* +0x2a */
    unsigned short frames;              /* +0x2c */
    short width, height;                /* +0x2e */
    char pad_32[4];
    long speed;                         /* +0x36 */
    char pad_3a[2];
    long created;                       /* +0x3c */
    char pad_40[0x76 - 0x40];
    long frame_offset;                  /* +0x76 */
    char pad_7a[0xa6 - 0x7a];
    DiskFile file;                      /* +0xa6 */
    char name[0x232 - 0x132];           /* +0x132 */
    void *res;                      /* +0x232 */
    short res_id;                       /* +0x236 */
    short hbuf;                         /* +0x238 */
    char *buf;                     /* +0x23a */
    unsigned char *pos;            /* +0x23e */
    void *frame2;                   /* +0x242 */
    char pad_246[4];
    long left;                          /* +0x24a */
    short f24e, f250;                   /* +0x24e */
    unsigned char rgb[0x300];           /* +0x252 */
    short pal_first;                    /* +0x552 */
    short pal_count;                    /* +0x554 */
    unsigned char dirty;                /* +0x556 */
    char f557;
    short hrects, hrect_frames;         /* +0x558 */
    RECT *rects;                    /* +0x55c */
    int *rect_frames;               /* +0x560 */
    short frame_no, rect_index;         /* +0x564 */
    unsigned count, least, most;        /* +0x568 */
    short timer;                        /* +0x56e */
    void *port;                     /* +0x570 */
    void (*loop_hook)(void);        /* +0x574 */
    Flic();
    ~Flic();
};

/* same object: 4-byte `push cs; call ` (rule 47) */

char save_frr_file(Flic *flic)
{
    DiskFile f;
    char name[12];

    name[0] = 0;
    L1_0c72(name, flic->name, STRLEN(flic->name) - 4);
    STRCAT(name, ".frr");
    if (MSCreateFile_1068_9565(&f, name) == 0)
        return 0;
    MSWriteFile_1068_958b(&f, &flic->created, 4L);
    MSWriteFile_1068_958b(&f, &flic->count, 2L);
    MSWriteFile_1068_958b(&f, flic->rect_frames, (long)(flic->count * 2));
    MSWriteFile_1068_958b(&f, flic->rects, (long)(flic->count * 8));
    MSCloseFile_1068_95b5(&f);
    return 1;
}

