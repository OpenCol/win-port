/* set_job: candidate for 1018:4f04 (996 bytes).  Verdict: RELOCATABLE.
 * symbol: set_job
 * name-source: sibling-build
 * size-source: 996, not the census 979: the dead closing epilogue and the four-entry jump table of
 * the move switch, which ends where job_count begins.
 *
 * Moves colonist `c` of the current colony into `job` and returns where it ended up. It is the
 * mechanical half of `try_to_set_job`, which asks the questions and then calls this.
 *
 * EQUIPMENT GOES BACK TO THE WAREHOUSE AND COMES OUT AGAIN. The goods the OLD job carries
 * (`job_cargo`) are returned to the colony's stock -- the unit's own tools for tools,
 * 50 for horses, 50 for muskets or anything else, nothing for food -- and after the move the goods the
 * NEW job needs are taken out: 50 horses, 50 muskets, and tools in twenties up to 100 (all the colony
 * has, rounded down to twenty). A job 0x14 unit, JOB20 "HARDY PIONEERS", carries those tools with it.
 *
 * `job_transfer_type` then says what kind of move it is:
 *
 *   0  a worker changes job inside the colony: the job byte at Colony+0x20+c;
 *   2  a colonist walks out as a unit of the job's type (`create_unit`), keeping its
 *      specialty, and leaves the colony's roster;
 *   1  a unit outside already, changing type in place, orders cleared;
 *   3  a unit JOINS the colony, while the colony has fewer than 32 people: 100 added to the colony's
 *      long at +0xc6, a new roster slot filled by calling this function again, the specialty copied, the
 *      unit record freed. A colony reaching three people this way whose nation has
 *      FATHER_LA_SALLE gets colony flag 0, the stockade -- La Salle's documented effect.
 *
 * `flag` and `jobname` are computed and never read. Two `(int)` casts on the population byte
 * (rule 51); the move switch's case bodies are in the order 0, 2, 1, 3/default.
 *
 * RENAMED 2026-09-26 from colony_set_job: the Mac build's set_job(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller colony_select_profession; caller create_colony; caller resolve_raid; callee has_father), the runner-up gets 2, and the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"
#include "building.h"
#include "jobs.h"
#include "goods.h"
#include "fathers.h"
#include "records.h"

extern void end_unit_turn(int u);
extern void delete_unit(int u);
extern int has_father(int nation, int bit);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int get_job(int colonist);
extern int job_cargo(int *goods, int job);
extern int nth_unit_with_job(int n);

extern void set_progress(int colonist, int progress);
extern int job_transfer_type(int colonist, int job);
extern int unit_of_job(int job);
extern int get_specialty(int u);
extern void delete_colonist(int colonist);
extern void reset_colony_units(void);
extern void set_specialty(int i, int s);
extern void set_building(int bit, int on);

int set_job(int colonist, int job)
{
    int result;
    int u;
    int idx;
    int kind;
    int n;
    int goods[3];
    int oldjob;
    int jobname;
    int spec;
    int flag = 0;
    unsigned char keep[16];
    register int i;
    register int amt;

    jobname = job;
    if (jobname == 0x17)
        jobname = 0x15;
    oldjob = get_job(colonist);
    n = job_cargo(goods, oldjob);
    for (i = 0; i < n; i++) {
        u = nth_unit_with_job(colonist - g_cur_colony_rec->pop);
        switch (goods[i]) {
        case GOODS_TOOLS:
            amt = g_units[u].hold.w.tools;
            break;
        case GOODS_HORSES:
            amt = 0x32;
            break;
        case GOODS_FOOD:
            amt = 0;
            break;
        case GOODS_MUSKETS:
        default:
            amt = 0x32;
            break;
        }
        g_cur_colony_rec->stock[goods[i]] += amt;
    }
    memset_rtl(keep, 0, 0x10);
    amt = g_cur_colony_rec->stock[GOODS_TOOLS] / 20;
    keep[GOODS_TOOLS] = min_int_1018_b329(100, amt * 20);
    keep[GOODS_MUSKETS] = 0x32;
    keep[GOODS_HORSES] = 0x32;
    if (oldjob != job)
        set_progress(colonist, 0);
    kind = job_transfer_type(colonist, job);
    switch (kind) {
    case 0:
        g_cur_colony_rec->jobs[colonist] = job;
        result = colonist;
        break;
    case 2:
        u = create_unit(unit_of_job(job), g_cur_colony_rec->nation,
                                  g_cur_colony_rec->x, g_cur_colony_rec->y);
        if (u >= 0) {
            end_unit_turn(u);
            spec = get_specialty(colonist);
            g_units[u].spec = spec;
            if (job == JOB_PIONEER)
                g_units[u].hold.w.tools = keep[GOODS_TOOLS];
            delete_colonist(colonist);
            result = g_cur_colony_rec->pop;
            reset_colony_units();
        }
        break;
    case 1:
        u = nth_unit_with_job(colonist - g_cur_colony_rec->pop);
        g_units[u].type = unit_of_job(job);
        g_units[u].orders = ORDERS_NONE;
        if (job == JOB_PIONEER)
            g_units[u].hold.w.tools = keep[GOODS_TOOLS];
        end_unit_turn(u);
        result = colonist;
        reset_colony_units();
        break;
    case 3:
    default:
        if ((int)g_cur_colony_rec->pop < 0x20) {
            u = nth_unit_with_job(colonist - g_cur_colony_rec->pop);
            g_cur_colony_rec->bells_cap += 100;
            idx = g_cur_colony_rec->pop++;
            set_job(idx, job);
            set_specialty(idx, g_units[u].spec);
            delete_unit(u);
            result = idx;
            if ((int)g_cur_colony_rec->pop >= 3
                && has_father(g_cur_colony_rec->nation, FATHER_LA_SALLE) != 0)
                set_building(BUILDING_STOCKADE, 1);
            reset_colony_units();
        } else
            idx = g_cur_colony_rec->pop - 1;
        break;
    }
    n = job_cargo(goods, job);
    for (i = 0; i < n; i++) {
        amt = keep[goods[i]];
        if (goods[i] == GOODS_FOOD)
            flag = 0;
        g_cur_colony_rec->stock[goods[i]] = max_int_1018_b34f(0, g_cur_colony_rec->stock[goods[i]] - amt);
    }
    if (g_cur_colony_rec->pop == 0)
        g_abandon_flag = 1;
    return result;
}
