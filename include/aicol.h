/* aicol.h -- the AI's per-colony working flags, Colony+0x1b, which the corpus declares
 * `unsigned char ai_flags` since 2026-09-24. Eighteen files carried it as `f1b` or as a
 * single filler byte `pad1b`; the member name is this header's own title and asserts
 * nothing about any bit.
 *
 * WHERE THE VALUES COME FROM: `colony_plan`'s own guards and nothing else. No resource
 * mentions these, no dialog edits them, and every read is in AI code -- `plan_move` and
 * `foreign_turn` deciding where to send a colonist. So this header sits in the lower tier with
 * tribes.h's neighbours: the NAMES say what the guard tests, and no more than that.
 *
 * NAMED AFTER THE TEST, NOT THE INTENT. `if (soldiers > 0) f1b |= 0x40` warrants "this colony has
 * soldiers"; it does not warrant a name about garrisons or defence plans, which is what the AI may
 * be using it for. Where the guard is arithmetic the name follows the arithmetic.
 *
 * HALF THE BYTE IS REBUILT EVERY SURVEY. `colony_plan` opens with `f1b &= 7`, which keeps
 * bits 0, 1 and 2 and clears 3..7, so 0x8 and above are recomputed from scratch on each pass while
 * 0x1, 0x2 and 0x4 carry over.
 *
 * 0x4 IS IN THE KEPT HALF AND STILL MANAGED: the survey both sets it (`|= 4`) and clears it
 * (`&= ~4`) explicitly, so it survives a pass unless the survey decides otherwise -- sticky rather
 * than recomputed.
 *
 * 0x1 AND 0x2 ARE WRITTEN, and this header said until 2026-09-24 that they were not. They are
 * written by `collect_stats`, which is not AI code and so was not where anyone looked:
 * it clears both (`&= 0xfc`) and rebuilds them from a scan of every square within five of the
 * colony for an enemy warship that can reach it. `colony_plan`'s `&= 7` keeps them because
 * the survey is not what owns them -- the recount is, and it runs first.
 *
 * WHAT MAKES THE TWO NAMES SAFE is that the recount ALSO counts them into four globals the
 * corpus had already named from the other side: `g_colonies_blockaded` and `g_pop_blockaded`
 * off 0x2, `g_colonies_besieged` and `g_pop_besieged` off 0x1. Two independent namings agreeing
 * is the evidence; the guard alone would only have warranted "a frigate is near".
 */
#ifndef AICOL_H
#define AICOL_H

/* An enemy warship that is NOT a frigate is within five steps of the colony. Counted into
 * `g_colonies_besieged` and `g_pop_besieged`. */
#define COLONY_AI_BESIEGED          0x01

/* An enemy FRIGATE is within five steps. The recount tests the type first, so a frigate sets
 * this INSTEAD of 0x1 rather than as well -- the two bits are exclusive per ship, though a
 * colony reached by both kinds carries both. Counted into `g_colonies_blockaded` and
 * `g_pop_blockaded`. */
#define COLONY_AI_BLOCKADED         0x02

/* `if (limit > 1 ? 1 : 0) + limit < mycount` -- this nation has more colonies in the region than
 * the computed allowance. */
#define COLONY_AI_OVER_LIMIT        0x04

/* `if (mycount < limit)` -- fewer than the allowance. The two are not complements: at exactly the
 * limit neither is set. */
#define COLONY_AI_UNDER_LIMIT       0x08

/* There is room for another colonist: population under 0x20 and the ring has more workable tiles
 * than people. CONFIRMED from the other side twice -- `plan_move` skips a colony without it
 * when placing a non-convert, and scores a destination `+0x19` with it and `-0x19` without. */
#define COLONY_AI_HAS_ROOM          0x10

/* `if (unimproved != 0 || hills != 0)` -- land around the colony is unimproved or hilly. Set
 * together with 0x20 (as 0xa0) in the two forest cases; what 0x20 adds is not established, so it
 * has no name. */
#define COLONY_AI_LAND_UNIMPROVED   0x80

/* `if (soldiers > 0)` -- the colony has soldiers in it. */
#define COLONY_AI_HAS_SOLDIERS      0x40

#endif
