/* init_data: candidate for 1008:81d4 (879 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The building table, written out one row at a time: 42 calls to `setup_building`
 * and six to `setup_purchase`, and nothing else. No loop, no data segment -- the whole
 * table is 879 bytes of CODE.
 *
 * TWO COUNTERS, BOTH ADVANCED IN THE ARGUMENT LIST. `i` is the group number and `j` the index
 * of the six-entry record; both start at -1 and are pre-incremented where they are used, which
 * is what `INC SI / MOV AX,SI / PUSH AX` says against a bare `PUSH SI`. Groups run 1 to 5 rows
 * each and the row numbers are NOT in order -- group 3 is rows 9, 10, 11, 30, 31, so two rows
 * from the end of the table belong to the same group as three from the end.
 *
 * The second and fourth arguments are the previous and next row of the group, -1 at each end,
 * so the table is a set of singly-linked chains laid out by hand.
 *
 * `j` costs a `PUSH DI` in the prologue for a register that is not touched until byte 0x2e2.
 *
 * TRANSCRIBED BY SCRIPT. A function this shape -- one straight line of calls with constant
 * arguments -- is not worth reading by hand; the arguments were lifted out of the disassembly
 * by a parser that tracks the pending pushes, which is the same thing `tools/re_calls.py` now
 * does for any function.
 *
 * RENAMED 2026-09-27 from building_table_init: CONST PROPOSAL. Mac init_data(void), CODE 26 0x45ba, last in the segment. Call graph: its whole seq is setup_building x42 then setup_purchase x6; 81d4 calls building_table_set_fields x42 and recruit_set x6, in that order. Caller: Mac letsGoSurfinNow (bsr) against Windows game_init, the startup routine on both sides. Arity: (void) both. Layout: Mac CODE 26 runs setup_building 0x44c4, setup_purchase 0x4544, init_data 0x45ba; Windows 1008 runs 80f4, 8169, 81d4. Constants: 10 shared, overlap 1.0, rare 0xbb8/0x1388. Size: Mac is last in its segment (no next start); Windows 879. No collision.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */
extern int setup_building(int a, int prev_level, int prereq2, int next_level, int family);
extern int setup_purchase(int a, int type, int price, int amount, int e, int i);

void init_data(void)
{
    register int i;
    register int j;

    i = -1;
    g_seam_handle = 0;
    setup_building(0, -1, -1, 1, ++i);
    setup_building(1, 0, -1, 2, i);
    setup_building(2, 1, -1, -1, i);
    setup_building(3, -1, -1, 4, ++i);
    setup_building(4, 3, -1, 5, i);
    setup_building(5, 4, -1, -1, i);
    setup_building(6, -1, -1, 7, ++i);
    setup_building(7, 6, -1, 8, i);
    setup_building(8, 7, -1, -1, i);
    setup_building(9, -1, -1, -1, ++i);
    setup_building(10, 9, -1, 11, i);
    setup_building(11, 10, -1, -1, i);
    setup_building(30, 11, -1, 31, i);
    setup_building(31, 30, -1, -1, i);
    setup_building(12, -1, -1, 13, ++i);
    setup_building(13, 12, -1, 14, i);
    setup_building(14, 13, -1, -1, i);
    setup_building(15, -1, -1, 16, ++i);
    setup_building(16, 15, -1, -1, i);
    setup_building(17, -1, -1, -1, i);
    setup_building(18, -1, -1, -1, ++i);
    setup_building(19, -1, -1, 20, ++i);
    setup_building(20, 19, -1, -1, i);
    setup_building(21, -1, -1, 22, ++i);
    setup_building(22, 21, -1, 23, i);
    setup_building(23, 22, -1, -1, i);
    setup_building(24, -1, -1, 25, ++i);
    setup_building(25, 24, -1, 26, i);
    setup_building(26, 25, -1, -1, i);
    setup_building(27, -1, -1, 28, ++i);
    setup_building(28, 27, -1, 29, i);
    setup_building(29, 28, -1, -1, i);
    setup_building(32, -1, -1, 33, ++i);
    setup_building(33, 32, -1, 34, i);
    setup_building(34, 33, -1, -1, i);
    setup_building(35, -1, -1, 36, ++i);
    setup_building(36, 35, -1, -1, i);
    setup_building(37, -1, -1, 38, ++i);
    setup_building(38, 37, -1, -1, i);
    setup_building(39, -1, -1, 40, ++i);
    setup_building(40, 39, -1, 41, i);
    setup_building(41, 40, -1, -1, i);
    j = -1;
    setup_purchase(0, 11, 500, 2, -1, ++j);
    setup_purchase(0, 13, 1000, 2, -1, ++j);
    setup_purchase(0, 14, 2000, 4, -1, ++j);
    setup_purchase(0, 15, 3000, 6, -1, ++j);
    setup_purchase(0, 16, 2000, 3, -1, ++j);
    setup_purchase(0, 17, 5000, 8, -1, ++j);
}
