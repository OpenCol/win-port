/* nation_treaty_step -- shipped at 1030:50d4.
 *
 * Ported by tools/port.py from matched/game/nation_treaty_step.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_game_flags;                         SEG20:0x7784
 *   extern int far g_turn_counter;                       SEG20:0x7790
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 nation_wants_war(col_i16 a, col_i16 b);
void wake_units_near(col_i16 a, col_i16 b);

#include "gameflag.h"


extern void set_name_slot_from_text(col_i16 slot, col_i16 v);
extern void text_message_show(char *s, col_i16 n);
extern void nation_pair_flags_or_both(col_i16 a, col_i16 b, col_i16 bits);
extern void nation_pair_flags_clear_both(col_i16 a, col_i16 b, col_i16 bits);
extern col_i16 lookup_with_two_overrides(col_i16 n);

extern col_i16 nation_pair_flags_get(col_i16 a, col_i16 b);



void nation_treaty_step(col_i16 a, col_i16 b)
{
    if (g_game_flags & GAMEFLAG_INDEPENDENCE)
        goto done;
    if ((a + b + g_turn_counter) % 3 != 0 && (nation_pair_flags_get(a, b) & 0x20))
        goto done;
    if ((nation_pair_flags_get(a, b) & 2) || (nation_pair_flags_get(b, a) & 2))
        goto done;
    set_name_slot_from_text(0, lookup_with_two_overrides(a));
    set_name_slot_from_text(1, lookup_with_two_overrides(b));
    if (nation_wants_war(a, b) == 0 && nation_wants_war(b, a) == 0) {
        if (!(nation_pair_flags_get(a, b) & 0x40) && !(nation_pair_flags_get(b, a) & 0x40)) {
            text_message_show("SIGNTREATY", 2);
            nation_pair_flags_or_both(a, b, 0x40);
            wake_units_near(a, b);
            wake_units_near(b, a);
        }
    } else {
        if ((nation_pair_flags_get(a, b) & 0x40) || !(nation_pair_flags_get(a, b) & 0x20)) {
            if (nation_pair_flags_get(a, b) & 0x40)
                text_message_show("CANCELPEACE", 2);
            else
                text_message_show("DECLAREWAR", 2);
            nation_pair_flags_clear_both(a, b, 0x40);
        }
    }
done:
    ;
}
