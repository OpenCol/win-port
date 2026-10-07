/* PORT: this file's own declarations of port_unbind_anim removed: the header declares it from the definition (W1 step 3). */
/* anim_unbind: candidate for 1068:3401 (35 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: anim_unbind
 * name-source: descriptive
 *
 * Hands the object's pointer at +0x570 to `port_unbind_anim`.
 *
 * `timer_tick` already describes it: "runs 1068:3401, which forwards the object's 
 * pointer at +0x570 to ..." -- and `timer_tick_chain` pairs it with `Flic_Reset`.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own MagicWin, GRPort, DiskFile removed: include/gen/classes.h define them once (W1 step 2). */
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

void anim_unbind(Flic *anim)
{
    port_unbind_anim((MagicWin *)anim->port);
}
