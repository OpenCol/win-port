/* output_of_building: candidate for 1018:5324 (295 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -a
 * name-source: sibling-build
 *
 * What a colony building produces: returns the amount and, through the optional
 * out-pointers, the goods kind and the picture id.  The argument is a BUILDING (the
 * Mac name says so, and the callers pass building indices). The printing press and
 * newspaper give the liberty-bell bonus at DGROUP:0x4ba5 with the bell picture 0x3f,
 * the stables the horses produced; any other building is mapped through
 * job_of_building to the job worked in it (must be >= 0, as must guys_at_building),
 * and a switch on that job gives the goods kind and picture, the amount read from
 * the SEG21:0x122 production table. The statesman's bells lose the press/newspaper
 * bonus, which the press and newspaper report on their own.
 *
 * CORRECTED 2026-09-27: the building was named `job` and the job `goods_kind`, and the
 * three building tests were spelled JOB_COLONIST, JOB_PIONEER and JOB_STATESMAN -- the
 * same numbers (19, 20, 17) as BUILDING_PRINTING_PRESS, BUILDING_NEWSPAPER and
 * BUILDING_STABLES, which is what they test: the stables branch returns the horses.

 * SIZE: the census says 277 and the function is 295. The extra bytes are the CS-relative
 * jump table for this function's `switch`, which Borland puts in the CODE segment and
 * Ghidra's reachable-code size excludes. The reconstruction grades over the whole
 * extent unchanged; only the number moved.
 *
 * RENAMED 2026-09-26 from production_info: the Mac build's output_of_building(short, short*, short*, short*), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between job_count and compute_colony_yield, which are already bound; the arity agrees and the Mac body is 0.91x this one.
 */

#include "colonization.h"

#include "goods.h"
#include "jobs.h"
#include "building.h"

extern int job_of_building(int building);
extern int guys_at_building(int building);

int output_of_building(int building, int *kind_out, int *pic_out, int *extra_out)
{
    int amount = 0;
    register int kind = -1;
    register int pic = 0;
    int job;
    int u;
    int extra = 0;

    if (building == BUILDING_PRINTING_PRESS || building == BUILDING_NEWSPAPER) {
        amount = g_bells_extra;
        pic = 0x3f;
    } else if (building == BUILDING_STABLES) {
        amount = g_prod[GOODS_HORSES];
        pic = 0x1f;
    } else {
        job = job_of_building(building);
        if (job < 0)
            goto done;
        u = guys_at_building(building);
        if (u < 0)
            goto done;
        switch (job) {
        case JOB_DISTILLER:
        case JOB_TOBACCONIST:
        case JOB_WEAVER:
        case JOB_FUR_TRADER:
        case JOB_BLACKSMITH:
        case JOB_GUNSMITH:
            kind = job;
            pic = job + 0x17;
            break;
        case JOB_CARPENTER:
            kind = 0x10;
            pic = 0x37;
            break;
        case JOB_PREACHER:
            kind = 0x11;
            pic = 0x39;
            break;
        case JOB_STATESMAN:
            kind = 0x12;
            pic = 0x3f;
            break;
        }
        if (kind > 0)
            amount = g_prod[kind];
        if (job == JOB_STATESMAN)
            amount -= g_bells_extra;
    }
done:
    if (kind_out != 0)
        *kind_out = kind;
    if (pic_out != 0)
        *pic_out = pic;
    if (extra_out != 0)
        *extra_out = extra;
    return amount;
}
