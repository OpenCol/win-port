/* PORT: this file's own declarations of Sprite_GetHotSpot removed: the header declares it from the definition (W1 step 3). */
/* Opening_DoShip: candidate for 1010:05ca (1828 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_DoShip
 * name-source: sibling-build
 * size-source: 1828, not the census 46. Ghidra stops at the indirect switch jump; the five
 * cases and their 10-byte jump table continue through 1010:0ced.
 *
 * Advances the opening sequence's five-state composition machine.  The first three states
 * step sprite strips over a path or fixed point, the fourth assembles and highlights the
 * finished title screen, and the fifth waits for its deadline before marking the sequence done.
 *
 * RENAMED 2026-09-26 from opening_scene_step: the Mac build's Opening::DoShip(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (callee Draw; callee GetHotSpot; callee sound_play), the runner-up gets 2, the arity agrees (1), and the Mac body is 0.62x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "w16types.h"
#include "sounds.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

/* the ship's current frame in each of its three animations */
#define FRAME16AC(o) (&(o)->ship_state0[(o)->frame])
#define FRAME058C(o) (&(o)->ship_state1[(o)->frame])
#define FRAME096C(o) (&(o)->ship_state2[(o)->frame])

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern "C" void sound_play(int n);   /* PORT: C linkage, as defined (W2) */
extern void DoFizzle(GRPort *port, GameWin *window, RECT *r,
                              int x, int y, int mode, int step);

/* Same translation unit: the shipped calls use PUSH CS / CALL NEAR. */

extern void Opening_RefreshUpdatePort(Opening *o);

void Opening_DoShip(Opening *o)
{
    int hot_x;
    int hot_y;
    RECT r0;
    RECT r1;
    RECT r2;
    RECT r3;
    RECT r4;
    RECT r5;
    RECT r6;
    register int path_index;

    switch (o->state) {
    case 0:
        path_index = o->scroll / 2;
        SetRect(&o->source_rect, 0, 0,
                rect_width_1010_2b62(FRAME16AC(o)),
                rect_height_1010_2b81(FRAME16AC(o)));
        o->dest_rect = o->source_rect;
        Sprite_GetHotSpot(FRAME16AC(o), &hot_x, &hot_y);
        OffsetRect(&o->dest_rect, o->ship_path[path_index].x + hot_x,
                                  o->ship_path[path_index].y + hot_y);
        GRPort_CopyToPort(&o->port1, &o->port2, &o->dest_rect, &o->source_rect);
        Sprite_Draw(&r2, FRAME16AC(o), &o->port1,
                    o->ship_path[path_index].x, o->ship_path[path_index].y);
        o->ship_x = o->ship_path[path_index].x;
        o->ship_y = o->ship_path[path_index].y;
        o->ship_sprite = FRAME16AC(o);
        o->frame++;
        if (o->frame >= 8)
            o->frame = 0;
        if (o->ship_path[path_index + 1].x < 0) {
            o->state = 1;
            o->frame = 0;
            o->sprite_x = o->ship_path[path_index - 1].x;
            o->sprite_y = o->ship_path[path_index - 1].y;
        }
        break;

    case 1:
        SetRect(&o->source_rect, 0, 0,
                rect_width_1010_2b62(FRAME058C(o)),
                rect_height_1010_2b81(FRAME058C(o)));
        o->dest_rect = o->source_rect;
        Sprite_GetHotSpot(FRAME058C(o), &hot_x, &hot_y);
        OffsetRect(&o->dest_rect, o->sprite_x + hot_x, o->sprite_y + hot_y);
        GRPort_CopyToPort(&o->port1, &o->port2, &o->dest_rect, &o->source_rect);
        Sprite_Draw(&r3, FRAME058C(o), &o->port1, o->sprite_x, o->sprite_y);
        o->ship_x = o->sprite_x;
        o->ship_y = o->sprite_y;
        o->ship_sprite = FRAME058C(o);
        o->frame++;
        if (o->frame >= 18) {
            o->frame = 0;
            o->state = 2;
        }
        break;

    case 2:
        if (o->sound_played == 0) {
            sound_play(SOUND_MUSIC_JINE_THE_CAVALRY);
            o->sound_played = 1;
        }
        SetRect(&o->source_rect, 0, 0,
                rect_width_1010_2b62(FRAME096C(o)),
                rect_height_1010_2b81(FRAME096C(o)));
        o->dest_rect = o->source_rect;
        Sprite_GetHotSpot(FRAME096C(o), &hot_x, &hot_y);
        OffsetRect(&o->dest_rect, o->sprite_x + hot_x, o->sprite_y + hot_y);
        GRPort_CopyToPort(&o->port1, &o->port2, &o->dest_rect, &o->source_rect);
        Sprite_Draw(&r4, FRAME096C(o), &o->port1, o->sprite_x, o->sprite_y);
        o->ship_x = o->sprite_x;
        o->ship_y = o->sprite_y;
        o->ship_sprite = FRAME096C(o);
        o->frame++;
        if (o->frame >= 54) {
            o->frame = 0;
            o->state = 3;
            Opening_RefreshUpdatePort(o);
        }
        break;

    case 3:
        SetRect(&r0, 0x500, 0x17c, 0x780, 0x1e0);
        SetRect(&r1, 0, 0x17c, 0x280, 0x1e0);
        GRPort_CopyToPort(&o->port1, &o->port1, &r0, &r1);
        Sprite_Draw(&r5, &o->ship_state2[0x35], &o->port1, 0x144, 0x111);
        Sprite_Draw(&r6, &o->logo, &o->port1, port_width_1010_2ba0(o) >> 1, 0);
        SetRect(&r0, 0, 0, 0x280, 0x1e0);
        DoFizzle(&o->port1, (GameWin *)o, &r0, 0, 0, -1, 0x1e);
        o->state = 4;
        o->deadline = MSGetTicks_1010_2bbf() + 0x1e0L;
        break;

    case 4:
        if (MSGetTicks_1010_2bbf() > o->deadline)
            o->sequence_done = 1;
        break;
    }
}
