/* colflag.h -- the colony's notification latches, Colony+0x1c.
 *
 * Every named bit here answers "has this already been reported?". `colony_end_of_turn` shows a
 * message when a condition becomes true and sets the bit, then shows the opposite message when it
 * becomes false again and clears it, so the player is told once per transition rather than every
 * turn. The bits are the memory that makes that work.
 *
 * WHERE THE NAMES COME FROM: the TEXT resource each transition shows, which is as direct as
 * evidence gets -- the code names its own bits by what it says to the player.
 *
 * TWO THRESHOLDS WITH HYSTERESIS, worth reading off because they are not symmetric: the unanimous
 * bit is set at Sons of Liberty 100 (`sol >= 0x64`) and cleared below 95 (`sol < 0x5f`), while the
 * majority bit is set at 50 and cleared below 50. So unanimity has a five-point dead band and
 * majority has none.
 *
 * THE TWO REBEL BITS ARE NOT ONLY LATCHES. `building_output` and `cell_production` each add one
 * to the production morale for 0x04 and one more for 0x02, against the Tory penalty they compute
 * just above -- the +1 at half Sons of Liberty and +2 at unanimity that the manual describes. So
 * these two carry live state the economy reads every turn, and telling the player is the second
 * thing they do. That is also why the unanimous bit's five-point dead band matters: production
 * keeps the bonus between 95 and 100, not just the message.
 *
 * NOT NAMED: 0x20, written and cleared by `ai_nation_turn`, and 0x40, read by `ai_unit_turn` and
 * by `apply_tax_change`, which considers only colonies that have it when it picks whose goods the
 * colonists will dump. Both are AI state on a byte the rest of the game uses for notifications,
 * and neither has a message or a guard that says what it means. The AI's own working byte is the one before this,
 * at +0x1b -- see aicol.h.
 */
#ifndef COLFLAG_H
#define COLFLAG_H

/* Sons of Liberty reached 100: "REBELUNANIMOUS" on the way up, "TORYMINORITY" below 95. */
#define COLONY_REBEL_UNANIMOUS      0x02

/* Sons of Liberty reached 50: "REBELMAJORITY" on the way up, "TORYMAJORITY" on the way down. */
#define COLONY_REBEL_MAJORITY       0x04

/* Tories at or above `10 - difficulty`: "INEFFICIENT", and "EFFICIENT" when it recovers. Both
 * messages are suppressed by COLOPT_NO_BAD_GOVERNMENT, the "Report inefficient government"
 * checkbox -- the option and the latch describing the same feature from two directions. */
#define COLONY_INEFFICIENT          0x08

/* Set by `ai_manage_colony` when the population is under 10 and cleared once it has acted. */
#define COLONY_AI_SMALL             0x10

/* A building finished here: set in three places by `colony_build_complete` and cleared by
 * `clear_colony_flag_1c`, whose `&= 0x7f` clears exactly this bit and nothing else. */
#define COLONY_BUILD_DONE           0x80

#endif
