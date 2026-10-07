/* anim_start_ex: candidate for 1068:23a9 (55 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: anim_start_ex
 * name-source: descriptive
 *
 * Stores two words into the animation object at +0x24e and +0x250 and then arms the timer
 * through `anim_start` (1068:234a). The only caller-visible difference from `anim_start`
 * is those two words.
 *
 * Nothing in the shipped image ever reads them. The four instructions that touch either
 * offset are all stores: the two here, and the two in the object's constructor at
 * 1068:1d14 that zero them. They are vestigial fields, kept in the record and still
 * written by this entry point -- which is the reason to name the function for the two
 * arguments it forwards rather than for a meaning the bytes do not support.
 *
 * `anim_start` is reached by the four-byte `PUSH CS / CALL NEAR` form, so it is in this
 * translation unit and appears here as a stub (rule 47).
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

/* same object: 4-byte `push cs; call ` */
extern "C" void anim_start(Flic *anim, void *port);

void anim_start_ex(Flic *anim, void *port, int a4, int a5)
{
    anim->f24e = a4;
    anim->f250 = a5;
    anim_start(anim, port);
}
