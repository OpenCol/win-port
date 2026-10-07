/* PORT: this file's own declarations of GRPort_GetWin_1068_968c, get_ctable_1068_966f, get_win_1068_96e8 removed: the header declares them from the definition (W1 step 3). */
/* Flic_ShowFrame: candidate for 1068:2453 (315 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Flic_ShowFrame
 * name-source: sibling-build
 *
 * One tick of a running animation: install the palette if the last decoded frame changed one,
 * blit the whole frame to the port, advance the counter, and either decode the next frame and
 * say "still going" or stop and say "done". Both timer callbacks in this module call it and
 * use the returned byte to decide whether to keep the timer.
 *
 * `a->frames` at +0x2c is the SAME WORD `Flic_Loop` hands to `start_timer`
 * as its third argument. Read together the two uses say what that argument is: a repeat COUNT,
 * not an id -- the timer is asked for exactly as many ticks as the animation has frames, and
 * `EnableLowMemoryCheck` passes -1 there for a timer that should never stop.
 *
 * The frame comparison is UNSIGNED (`JNC`), so both counters are `unsigned short`.
 *
 * NO `else` ON THE LAST BRANCH. `if (...) { ...; return 1; } else { ...; return 0; }` compiles
 * to one extra `JMP SHORT` over the else, even though the branch above it returns; dropping
 * the `else` removes it. The shipped has no such jump, so the source falls through.
 *
 * The blit passes the rectangle's corner twice, once as the source origin and once as the
 * destination -- the frame is copied to the same coordinates it occupies.
 *
 * `GRPort_ColorSync`, `Flic_Reset`, `Flic_NextFrame` and `wing_blit`
 * are reached by the five-byte NOP form or a call and are `extern`; the five accessors have
 * the four-byte form and are stubs (rule 47).
 *
 * RENAMED 2026-09-26 from anim_tick: the Mac build's Flic::ShowFrame(void): this only, blitting the current frame to the port (installing its palette if it changed) and moving on to the next. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char pad[1]; } logpal;
/* PORT: this file's own Screen, GRPort, DiskFile, CTable removed: include/gen/classes.h define them once (W1 step 2). */
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
   /* 0x41e bytes: the palette, its handle and the cycling state */
extern "C" void CTable_Fill(CTable *pal, int start, int count, void *rgb);
extern void GRPort_ColorSync(GRPort *port, CTable *table);
extern void Flic_Reset(Flic *a);
extern void Flic_NextFrame(Flic *flic);
extern void wing_blit(GrafRec *src, int sx, int sy, int w, int h, MSWin *dst, int dx, int dy);

/* same object: 4-byte `push cs; call ` */

char Flic_ShowFrame(Flic *anim)
{
    RECT r;
    void *pal;

    if (anim->dirty != 0) {
        pal = get_ctable_1068_966f((Screen *)((char *)anim->port + 0x26));
        CTable_Fill((CTable *)pal, anim->pal_first, anim->pal_count, anim->rgb);
        anim->dirty = 0;
        GRPort_ColorSync(anim, (CTable *)pal);
    }
    SetRect(&r, 0, 0, anim->w, anim->h);
    wing_blit((GrafRec *)GRPort_GetWin_1068_968c(anim), r.left, r.top,
                        rect_width_1068_96a9(&r), rect_height_1068_96c8(&r),
                        (MSWin *)get_win_1068_96e8((Screen *)((char *)anim->port + 0x26)), r.left, r.top);
    anim->frame_no++;
    if (anim->frame_no < anim->frames) {
        Flic_NextFrame(anim);
        return 1;
    }
    Flic_Reset(anim);
    return 0;
}
