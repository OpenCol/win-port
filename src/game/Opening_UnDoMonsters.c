/* Opening_UnDoMonsters: candidate for 1010:245e (454 bytes, 1 caller).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_UnDoMonsters
 * name-source: sibling-build
 *
 * Six animation slots in the window, each a flag byte, a counter word and a sprite record:
 * when a slot's flag is 1 its sprite is drawn into the port at +0xc4 at a fixed position,
 * and the flag is cleared once the counter has reached 0. Six separate scratch buffers.
 *
 * RENAMED 2026-09-26 from draw_active_animations: the Mac build's Opening::UnDoMonsters(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between Opening_OpeningMouse and Opening_DoCredits, which are already bound, paired in order; the arity agrees and the Mac body is 0.70x this one.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void Opening_UnDoMonsters(Opening *w)
{
    char out1[8], out2[8], out3[8], out4[8], out5[8], out6[8];

    if ((int)w->monster_slots[0].active == 1) {
        Sprite_Draw((RECT *)out1, (Sprite *)w->monster1, &w->port1, 0x2b4, 0xe6);
        if (w->monster_slots[0].count == 0) w->monster_slots[0].active = 0;
    }
    if ((int)w->monster_slots[1].active == 1) {
        Sprite_Draw((RECT *)out2, (Sprite *)w->monster2, &w->port1, 0x499, 0x62);
        if (w->monster_slots[1].count == 0) w->monster_slots[1].active = 0;
    }
    if ((int)w->monster_slots[2].active == 1) {
        Sprite_Draw((RECT *)out3, (Sprite *)w->monster3, &w->port1, 0x499, 0x62);
        if (w->monster_slots[2].count == 0) w->monster_slots[2].active = 0;
    }
    if ((int)w->monster_slots[3].active == 1) {
        Sprite_Draw((RECT *)out4, (Sprite *)w->monster4, &w->port1, 0x2a1, 0x6d);
        if (w->monster_slots[3].count == 0) w->monster_slots[3].active = 0;
    }
    if ((int)w->monster_slots[5].active == 1) {
        Sprite_Draw((RECT *)out5, (Sprite *)w->monster5, &w->port1, 0x63d, 0x61);
        if (w->monster_slots[5].count == 0) w->monster_slots[5].active = 0;
    }
    if ((int)w->monster_slots[6].active == 1) {
        Sprite_Draw((RECT *)out6, (Sprite *)((char *)w + 0x1a0c), &w->port1, 0x4ab, 0x118);
        if (w->monster_slots[6].count == 0) w->monster_slots[6].active = 0;
    }
}
