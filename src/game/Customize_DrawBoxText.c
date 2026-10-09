/* PORT: the Customize view as the class lays it out: i16 fields, objects sized by class (W9; also the i386 build's `family`). */
/* Customize_DrawBoxText: candidate for 1030:181d (405 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * One button of the Customize New World screen: button `i` of twelve, in four families of three.
 * The family caption goes on the first line and the choice 20 pixels below it, both starting at
 * the middle of the button's rectangle a third of the way down, in the small font -- red (0xc)
 * for the family being edited at +0x1a2, otherwise 0xfa.
 *
 * The labels are two INITIALIZED AUTO ARRAYS: Borland keeps their templates in DGROUP (0x1310,
 * twelve pointers, and 0x1340, four) and copies them onto the stack with `REP MOVSW` on
 * every call. The template bytes in the file are placeholder `FFFF:0000` pairs -- every one is
 * a relocation -- so the strings were read through the NE relocation table: Land Mass (Small,
 * Moderate, Large), Land Form (Archipelago, Normal, Continents), Temperature (Cool, Temperate,
 * Warm), Climate (Arid, Normal, Wet). The comparator checks none of them (§119).
 *
 * `TextColor` is declared with an `int` parameter as in `Customize_DrawIt`, this
 * function's one caller in the module (rule 55). The four module helpers come through the
 * 4-byte `PUSH CS` form and are stubs here (rule 47).
 *
 * RENAMED 2026-09-26 from draw_world_option: the Mac build's Customize::DrawBoxText(short): this plus one box index, drawing one of the twelve option boxes with its text; Customize_DrawIt calls it for each chosen box.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
typedef struct {                      /* PORT: classes.h's Customize, member for member: i16, and the
                                         objects sized by their classes -- the old view's 4-byte ints put
                                         `family` at +0x1ee, not the game's +0x1a2 (W9) */
    char gamewin[PORT_SIZEOF_GAMEWIN];
    RECT boxes[12];                       /* +0xc4  */
    i16 hit[4];                           /* +0x124 */
    i16 choice[4];                        /* +0x12c */
    char ok[PORT_SIZEOF_SBUTTON];         /* +0x134 */
    char cancel[PORT_SIZEOF_SBUTTON];     /* +0x156 */
    char port[PORT_SIZEOF_GRPORT];        /* +0x178 */
    i16 done, cancelled, family;          /* +0x19e, +0x1a0, +0x1a2 */
} Customize;

extern void GRPort_String(GRPort *port, unsigned *font, const char *s,
                                     int x, int y, int align);

/* same object: 4-byte `push cs; call ` */

void Customize_DrawBoxText(Customize *s, int i)
{
    char *choice[12] = { "Small", "Moderate", "Large",
                             "Archipelago", "Normal", "Continents",
                             "Cool", "Temperate", "Warm",
                             "Arid", "Normal", "Wet" };
    char *family[4] = { "Land Mass:", "Land Form:", "Temperature:", "Climate:" };

    if (s->family == i / 3)
        TextColor(0xc);
    else
        TextColor(0xfa);
    message_buf_set_1030_e978(family[i / 3]);
    GRPort_String((GRPort *)s, (unsigned *)&g_font_small, message_buf_1030_e99a(),
                         rect_width_1030_e4e5(&s->boxes[i]) / 2 + s->boxes[i].left,
                         rect_height_1030_e9b2(&s->boxes[i]) / 3 + s->boxes[i].top, 0);
    message_buf_set_1030_e978(choice[i]);
    GRPort_String((GRPort *)s, (unsigned *)&g_font_small, message_buf_1030_e99a(),
                         rect_width_1030_e4e5(&s->boxes[i]) / 2 + s->boxes[i].left,
                         rect_height_1030_e9b2(&s->boxes[i]) / 3 + s->boxes[i].top + 0x14, 0);
}
