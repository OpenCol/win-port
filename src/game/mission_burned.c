/* mission_burned: candidate for 1038:022f (114 bytes).  Verdict: RELOCATABLE.
 * symbol: mission_burned
 *
 * A tribe burns a European nation's mission: for a human-controlled European (byte +1
 * of the 52-byte record at SEG20:0x7840 clear) with a window given, the
 * tribe's adjective goes into text slot 0 and show_text_dialog_1038_865b reports; flag
 * 0x40 between tribe and nation is cleared and tribe_burn_missions runs.  The two
 * module helpers are -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "control.h"
#include "nations.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */
extern int g_stub_a;
extern int get_name(int nation);
extern void popup_say(int slot, int word);
extern int treaty_off(int nation, int other, int mask);

extern void tribe_burn_missions(int tribe, int nation);

void mission_burned(void *w, int tribe, int nation)
{
    if (nation < NATION_EUROPEAN_COUNT && g_nation_text[nation].control == CONTROL_HUMAN && w != 0) {
        popup_say(0, get_name(tribe + 4));
        show_text_dialog_1038_865b(w);
    }
    treaty_off(tribe + 4, nation, 0x40);
    tribe_burn_missions(tribe, nation);
}
