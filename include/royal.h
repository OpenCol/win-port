/* royal.h -- the four military kinds an intervention force is counted in.
 *
 * THREE ARRAYS SHARE THIS INDEX SPACE, which is why it is a header and not a comment:
 * `g_royal_forces` at SEG20:0x77dc (the King's), `g_force` at SEG20:0x77e4 (the intervening
 * power's) and `g_merc` in data segment 26 (mercenaries for hire). `intervention_force_arrives`
 * declares the second "regulars, dragoons, men-of-war, artillery" and the third "the same four",
 * and indexes all of them with one `kind` variable.
 *
 * THE MOUNTED KIND IS NOT ONE UNIT TYPE. `select_state_code(kind, nation)` turns a kind into the
 * unit type that nation fields, so kind 1 is cavalry for one side and dragoons for another -- the
 * two existing declarations disagree for exactly that reason and neither is wrong. The name here
 * follows the King's, and the resolution is the function's business.
 *
 * Originally: the King's expeditionary force, counted at SEG20:0x77dc.
 *
 * Four counts in one array, and the code keeps telling you which is which by how it adds them up:
 * `[0] + [1] + [3]` is summed as one quantity in three different functions while `[2]` is always
 * handled apart. That is the land force against the ships that carry it --
 * `royal_expedition_lands` will not sail when `total - [2] == 0` and lands nothing when
 * `[2] == total`, neither of which means anything unless [2] is the fleet.
 *
 * `king_mobilises_forces` picks between "regulars, cavalry, artillery -- whichever lags the
 * counts" when the treasury reaches 1800, which is the same three land kinds and in the same
 * order.
 *
 * WHERE THE VALUES COME FROM: the call sites, like tribes.h's neighbours. No resource indexes this
 * array; the KINGBUY and KINGMOBILIZE messages name the unit through `select_state_code` rather
 * than by position, so there is nothing to re-derive the order against.
 */
#ifndef ROYAL_H
#define ROYAL_H

#define ROYAL_REGULARS      0
#define ROYAL_CAVALRY       1
#define ROYAL_MAN_O_WAR     2       /* the fleet: summed apart from the other three */
#define ROYAL_ARTILLERY     3

#define ROYAL_KIND_COUNT    4

#endif
