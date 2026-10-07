/* region.h -- the sixteen per-region flag bytes at `g_region_flags`, which the AI reads when it
 * decides where to put the next colony and where to send the next soldier.
 *
 * WHERE THE VALUES COME FROM: one function, `collect_stats`, and nothing else. It zeroes
 * all sixteen and rebuilds them in one pass over every unit, every colony and every native
 * settlement on the map, so each bit below is a guard copied out of that pass rather than a
 * meaning inferred from a reader. That is the strongest form the evidence takes here -- the
 * filler pins every bit at once, and no reader had to be guessed at.
 *
 * THE RECOUNT IS PER-NATION, and this matters for reading the names. It runs for one nation at a
 * time and the sixteen bytes are not per-nation, so "ours" below means the nation the recount is
 * running for, and the flags say what the LAST recount saw. `foreign_turn` recounts before it
 * uses them, which is what makes that safe.
 *
 * 0x10 AND ABOVE ARE NOT SET by the recount and are not read anywhere, so they have no names.
 */
#ifndef REGION_H
#define REGION_H

/* A native settlement stands in the region. Set from the settlement loop, for every settlement,
 * regardless of tribe. */
#define REGION_NATIVE_SETTLEMENT    0x01

/* A unit belonging to a EUROPEAN power other than ours is in the region -- the recount's `else`
 * arm, guarded by `NATION(unit) < 4`, so the native nations do not set it. */
#define REGION_RIVAL_UNITS          0x02

/* A colony belonging to someone other than ours is in the region. Unlike 0x2 this one has no
 * nation test, so a native settlement that the colony loop walks would set it too. */
#define REGION_RIVAL_COLONY         0x04

/* We have a FORTIFIED defender in the region, standing outside any European-owned square:
 * defence > 1, orders FORTIFY or FORTIFIED, and `is_colony(...) < 0`. Counted per
 * region into `g_garrisoned_regions`, which `plan_move` then subtracts from the desirability
 * of another colony site -- the more regions we already hold down, the less we want more. */
#define REGION_GARRISONED           0x08

#endif
