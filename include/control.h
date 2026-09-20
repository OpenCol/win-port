/* control.h -- who is playing a European nation: the byte at g_nation52[nation].control.
 *
 * WARRANT: call sites only. No resource names these. Two independent lines of evidence agree,
 * which is more than this tier usually gets.
 *
 * THE WRITERS (8 assignments in 5 functions):
 *   0  set_human_player sets all four European nations to 1, then the chosen one to 0 on the
 *      line before `g_player_nation = n`. new_game_player_setup does the same.
 *   1  the value the other three keep. declaration_of_independence sets it on the MOTHER
 *      COUNTRY when you declare -- the King's nation stops being anybody's to play.
 *   2  nation_defeated sets it after freeing every one of that nation's units, and
 *      spanish_succession_event sets it on the nation that is absorbed.
 *
 * THE READERS, which confirm all three without reference to the writers. begin_nation_turn is
 * the one function that tells 1 and 2 apart, and it does both in four lines:
 *
 *      if (g_8b == 0 && g_nation52[n].control != CONTROL_DEFEATED)
 *          nation_turn();                  // a defeated nation gets no turn
 *      if (g_nation52[n].control == CONTROL_AI) {    // ...and the AI branch is 1
 *
 * For 0, mission_burned tests `nation < NATION_EUROPEAN_COUNT && control == CONTROL_HUMAN`
 * before showing a dialog, and a message is shown to a person rather than to an opponent.
 *
 * ONE ARRAY, WHICH USED TO BE THREE. The 176 read sites were once split across `g_nation52`
 * (0x7840, field `control` at +1), `g_rec52_7841` (0x7841, field `f0` at +0) and a `g_rec52`
 * that a third file uses for an unrelated tribe record at 0x7828. The first two are the same
 * byte seen from bases one apart, and re/structures/rec52_seg20_7840.json settles which is the
 * record: it is the only 52-stride array in segment 20, its +1 field has 209 reads across 91
 * functions, and its +0 field has 4. Everything is now spelled `g_nation52[n].control`.
 *
 * WHAT IS STILL NOT CHECKABLE: that 1 means specifically "a computer player" rather than, say,
 * "not being played by the person at this machine". Nothing distinguishes those readings, and
 * on a one-human game they never differ.
 */
#ifndef CONTROL_H
#define CONTROL_H

#define CONTROL_HUMAN       0
#define CONTROL_AI          1
#define CONTROL_DEFEATED    2

#endif
