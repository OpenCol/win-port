/* dbgopt.h -- the seven debug-display bits at DGROUP:0xb6.
 *
 * Source: the DEBUGOPTIONS dialog (TEXT 8) and `debug_info_flags` (1048:baf0), the Cheat-menu
 * entry that edits them. The mapping is the simple one the other option dialogs do NOT have: the
 * word is rebuilt as `(get(i) ? 1 : 0) << i`, so bit i IS checkbox i and the resource's line
 * order is the bit order.
 *
 * These are not inverted. The word is cleared to zero and a bit survives only if its box came
 * back set, so a set bit means that display is ON -- the opposite convention to colopt.h, whose
 * names carry NO_ for exactly that reason.
 *
 * `compose_unit_icon` agrees from the other side: it speaks of "debug bit 3 at DGROUP:0xb6" for a
 * European unit showing its planning mode, and bit 3 is "Foreign AI planning modes".
 *
 * Reachable only through the Cheat menu that `cheat_menu_toggle` puts up.
 */
#ifndef DBGOPT_H
#define DBGOPT_H

#define DBG_ANGER_FRICTION_LEVELS  0x01   /* "Anger & Friction Levels" */
#define DBG_INDIAN_AI_MOVEMENT     0x02   /* "Indian AI movement" */
#define DBG_SUPPLY_AND_DEMAND_INDIANS 0x04   /* "Supply and Demand (Indians)" */
#define DBG_FOREIGN_AI_PLANNING_MODES 0x08   /* "Foreign AI planning modes" */
#define DBG_CLOSE_MOVES            0x10   /* "Close Moves" */
#define DBG_FAR_MOVES              0x20   /* "Far Moves" */
#define DBG_ALL_MOVEMENT           0x40   /* "All Movement" */

#endif
