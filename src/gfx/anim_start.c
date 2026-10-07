/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 81 of the imported file. */
/* PORT: this file's own declarations of MagicWin_LoadFlic removed: the header declares it from the definition (W1 step 3). */
/* anim_start: candidate for 1068:234a (95 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: anim_start
 * name-source: descriptive
 *
 * Arms the animation timer. The destination port goes into the animation object at +0x570
 * AND into DGROUP:0x5690; the object itself goes into DGROUP:0x568c; `MagicWin_LoadFlic` binds
 * the two; and `start_timer` installs `timer_tick` with the object's own
 * interval at +0x36 and frame count at +0x2c (the timer's repeat count), the returned slot parked at +0x56e.
 *
 * The two globals are not incidental. `timer_tick` and its looping sibling
 * `timer_tick_chain` take no arguments and read exactly those two addresses, so
 * this function is where a callback with no parameters gets its subject.
 *
 * `g_anim_port = a->port = port;` is a CHAINED assignment: the shipped loads the pointer into
 * EAX once and stores it twice. Written as two statements Borland reloads `[bp+0xa]` in
 * between, which is four bytes more and the only difference in the function.
 *
 * Both -pointer stores are single 32-bit `MOV`s, which is what `-3` gives.
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

extern void *g_anim_port;   /* DGROUP:0x5690 */

/* same object: 5-byte NOP form, so extern (rule 47) */

void anim_start(Flic *anim, void *port)
{
    g_anim_port = anim->port = port;
    MagicWin_LoadFlic((MagicWin *)port, anim);
    g_anim = anim;
    anim->timer = start_timer((void*)(timer_tick), anim->speed, anim->frames);
}
