/* PORT: a member reached by the game's byte offset is reached by name or by its class sizes (PORT_EVENT_TABLE, PORT_SIZEOF_*): the offsets move on 64-bit Windows (W9). */
/* Opening_DoMonsters: candidate for 1010:1ed5 (1417 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_DoMonsters
 * name-source: sibling-build
 *
 * One frame of the six-slot intro animation. Every event in the window's monster_events list (time,
 * count, kind; terminated by time -1) whose time the scroll counter has reached starts its slot -- flag set,
 * frame 1, a countdown of count times a per-slot factor. The timed_monster frame is chosen by
 * scroll-counter thresholds; then each active slot draws its current 32-byte sprite record at its
 * fixed position, steps its frame modulo the slot's length, and counts down. The event
 * index and the timed_monster frame share one variable.

 * SIZE: the census says 857 and the function is 1417. a CS-relative jump table sits past the code, and
 * Ghidra sizes a function by its reachable CODE, so the table is excluded and the
 * reconstruction stops before it. `tools/re_agent/check_sizes.py` reports the table's
 * address; the reconstruction already grades over the whole extent unchanged.
 *
 * RENAMED 2026-09-26 from intro_animation_frame: the Mac build's Opening::DoMonsters(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between Opening_OpeningMouse and Opening_DoCredits, which are already bound, paired in order; the arity agrees and the Mac body is 0.69x this one.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void Opening_DoMonsters(Opening *w)
{
    char b1[8], b2[8], b3[8], b4[8], b5[8], b6[8], b7[8];
    int i = 0;

    do {
        switch (w->monster_events[i].kind) {
        case 1:
            if (w->monster_events[i].time == w->scroll && w->monster_slots[0].active == 0) {
                w->monster_slots[0].active = 1;
                w->monster_slots[0].frame = 1;
                w->monster_slots[0].count = w->monster_events[i].count * 13;
            }
            break;
        case 3:
            if (w->monster_events[i].time == w->scroll && w->monster_slots[1].active == 0) {
                w->monster_slots[1].active = 1;
                w->monster_slots[1].frame = 1;
                w->monster_slots[1].count = w->monster_events[i].count * 16;
            }
            break;
        case 4:
            if (w->monster_events[i].time == w->scroll && w->monster_slots[2].active == 0) {
                w->monster_slots[2].active = 1;
                w->monster_slots[2].frame = 1;
                w->monster_slots[2].count = w->monster_events[i].count * 14;
            }
            break;
        case 5:
            if (w->monster_events[i].time == w->scroll && w->monster_slots[3].active == 0) {
                w->monster_slots[3].active = 1;
                w->monster_slots[3].frame = 1;
                w->monster_slots[3].count = w->monster_events[i].count * 22;
            }
            break;
        case 7:
            if (w->monster_events[i].time == w->scroll && w->monster_slots[5].active == 0) {
                w->monster_slots[5].active = 1;
                w->monster_slots[5].frame = 1;
                w->monster_slots[5].count = w->monster_events[i].count * 11;
            }
            break;
        case 8:
            if (w->monster_events[i].time == w->scroll && w->monster_slots[6].active == 0) {
                w->monster_slots[6].active = 1;
                w->monster_slots[6].frame = 1;
                w->monster_slots[6].count = w->monster_events[i].count * 13;
            }
            break;
        }
        i++;
    } while (w->monster_events[i].time != -1);
    i = 1;
    if (w->scroll >= 0x10e) i = 2;
    if (w->scroll >= 0x132) i = 3;
    if (w->scroll >= 0x15a) i = 4;
    if (w->scroll >= 0x186) i = 5;
    if (w->scroll >= 0x1b8) i = 6;
    if (w->scroll >= 0x1d8) i = 7;
    if (w->scroll >= 0x1f8) i = 6;
    Sprite_Draw((RECT *)b1, (Sprite *)&w->timed_monster[i], &w->port1, 0x561, 0x123);
    if ((int)w->monster_slots[0].active == 1) {
        Sprite_Draw((RECT *)b2, (Sprite *)&w->monster1[w->monster_slots[0].frame], &w->port1, 0x2b4, 0xe6);
        w->monster_slots[0].frame++;
        if (w->monster_slots[0].frame >= 13) w->monster_slots[0].frame = 0;
        w->monster_slots[0].count--;
    }
    if ((int)w->monster_slots[1].active == 1) {
        Sprite_Draw((RECT *)b3, (Sprite *)&w->monster2[w->monster_slots[1].frame], &w->port1, 0x499, 0x62);
        w->monster_slots[1].frame++;
        if (w->monster_slots[1].frame >= 16) w->monster_slots[1].frame = 0;
        w->monster_slots[1].count--;
    }
    if ((int)w->monster_slots[2].active == 1) {
        Sprite_Draw((RECT *)b4, (Sprite *)&w->monster3[w->monster_slots[2].frame], &w->port1, 0x499, 0x62);
        w->monster_slots[2].frame++;
        if (w->monster_slots[2].frame >= 14) w->monster_slots[2].frame = 0;
        w->monster_slots[2].count--;
    }
    if ((int)w->monster_slots[3].active == 1) {
        Sprite_Draw((RECT *)b5, (Sprite *)&w->monster4[w->monster_slots[3].frame], &w->port1, 0x2a1, 0x6d);
        w->monster_slots[3].frame++;
        if (w->monster_slots[3].frame >= 22) w->monster_slots[3].frame = 0;
        w->monster_slots[3].count--;
    }
    if ((int)w->monster_slots[5].active == 1) {
        Sprite_Draw((RECT *)b6, (Sprite *)&w->monster5[w->monster_slots[5].frame], &w->port1, 0x63d, 0x61);
        w->monster_slots[5].frame++;
        if (w->monster_slots[5].frame >= 11) w->monster_slots[5].frame = 0;
        w->monster_slots[5].count--;
    }
    if ((int)w->monster_slots[6].active == 1) {
        Sprite_Draw((RECT *)b7, (Sprite *)&w->monster6[w->monster_slots[6].frame], &w->port1, 0x4ab, 0x118);
        w->monster_slots[6].frame++;
        if (w->monster_slots[6].frame >= 13) w->monster_slots[6].frame = 0;
        w->monster_slots[6].count--;
    }
}
