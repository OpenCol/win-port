/* settle.h -- the flag bits of a native settlement record.
 *
 * WHERE THE VALUES COME FROM: the call sites and the reconstruction headers that read them, not a
 * resource. This header sits in the same tier as tribes.h, gameflag.h and grerror.h -- the NAMES
 * are what byte-verified reconstructions establish, the VALUES have no witness outside the code,
 * and check_headers.py can only check that no two names claim one bit and that each is a single
 * bit. It cannot tell you a bit means what it says.
 *
 * Bits of the flags byte at Settlement+3 that nothing establishes are NOT named here: 0x01, 0x20,
 * 0x40 and 0x80 are all tested somewhere and none of them has a meaning anyone has written down.
 */
#ifndef SETTLE_H
#define SETTLE_H

/* ---- the flags byte, Settlement+3 ------------------------------------------------------------ */

/* The settlement has already taught a colonist. `ALREADY` is shown when this is set and the
 * capital bit is clear, which is the "we have already taught your people" refusal. */
#define SETTLEMENT_TAUGHT       0x02

/* The tribe's capital. It draws the overlay at SEG20:0x6486, doubles what living among the
 * natives is worth, takes 500 off an extortion demand, and multiplies an alarm swing by nine
 * (`alarm += |alarm| * 8`). "50 per level with the capital doubling it" is the same bit. */
#define SETTLEMENT_CAPITAL      0x04

/* The chief has given this settlement's speech; it is answered once per settlement. */
#define SETTLEMENT_CHIEF_SPOKE  0x08

/* The settlement has already been extorted. `EXTORTPOOR` is the refusal when this is set. */
#define SETTLEMENT_EXTORTED     0x10

/* ---- the mission byte, Settlement+5 ----------------------------------------------------------- */

/* The low nibble is the nation whose missionary is here. */
#define MISSION_NATION_MASK     0x0f

/* The mission is an expert one: set when the missionary's specialty is JOB_MISSIONARY, and given
 * to every mission of a nation that has Jean de Brebeuf. It doubles the mission's effect. */
#define MISSION_EXPERT          0x10

#endif
