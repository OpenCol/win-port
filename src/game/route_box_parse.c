/* route_box_parse: candidate for 1058:87dd (139 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A click on the trade-route panel at (x, y): the row picks the route -- `(y - 105) / 75`,
 * clamped to at most 3 -- and the column picks what happens. Left of 215 is the route itself
 * and goes to `route_name`; the two cargo strips at 215 and 415 go to
 * `route_order` with the side it landed on.
 *
 * The column is built by two INDEPENDENT tests, not an else-if chain: `col = 0; if (x >= 215)
 * col = 1; if (x >= 415) col = 2;` -- the second test runs whatever the first decided, which
 * is why there is no jump over it in the shipped code.
 *
 * The row divide is signed (`CWD` then `IDIV`), so a click above the strip gives a negative
 * route and `min_int` does not clamp it. That is the shipped arithmetic, not a reading of it.
 *
 * `route_order` TAKES THREE ARGUMENTS, and this call site is what proves it:
 * `ADD SP,6`. Its body reads only the first two, which is why its own reconstruction could
 * not see the third -- §119's blind spot. Its header has been corrected.
 *
 * RENAMED 2026-09-27 from route_strip_click: the Mac build's route_box_parse, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: Trade-route panel click: row gives the route (set_route), then route_name or route_order by column; Mac route_box_parse calls set_dest, route_name, route_order; 2 args.
 */

#include "colonization.h"

extern void route_name(void);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void route_order(int side, int x, int y);
void route_box_parse(register int x, int y)
{
    register int col;
    int route;

    route = (y - 0x69) / 0x4b;
    route = min_int_1058_be68(route, 3);
    set_route(route);
    col = 0;
    if (x >= 0xd7)
        col = 1;
    if (x >= 0x19f)
        col = 2;
    switch (col) {
    case 0:
        route_name();
        break;
    case 1:
    case 2:
        route_order(col == 2, x, y);
        break;
    }
}
