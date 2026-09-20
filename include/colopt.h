/* colopt.h -- the ten colony-report options, as bits of SEG20:0x7786.
 *
 * Source: the COLONYOPTIONS dialog (TEXT 143 in COLTEXT0.DLL), whose ten `@options` lines are the
 * checkboxes in slot order, and `game_colony_options` (1048:b374), which maps slot to bit. The
 * mapping is NOT the identity and no arithmetic produces it -- the code assigns slot 0 to 0x2,
 * slot 1 to 0x1, then 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x100, 0x200 -- so both halves are needed
 * and tools/check_headers.py re-reads both.
 *
 * EVERY BIT IS A SUPPRESSION, which is why each name begins COLOPT_NO_. The dialog stores each
 * one inverted in both directions -- the box is ticked from `!(flags & mask)` and the bit is set
 * again when the box comes back CLEAR -- so a set bit means the player turned that report OFF.
 * Naming them after the checkbox alone would invert the sense of every test in the corpus.
 *
 * Three reconstructions written without knowledge of this dialog agree: bit 0 "suppresses the
 * square description", which is the terrain label; bit 1 suppresses the building labels beside
 * it; and bit 9 "decides whether this nation is told at all", which is the rebel-majority report.
 */
#ifndef COLOPT_H
#define COLOPT_H

#define COLOPT_NO_BUILDING_LABELS      0x0002   /* "Labels on buildings" */
#define COLOPT_NO_TERRAIN_LABELS       0x0001   /* "Labels on cargo and terrain" */
#define COLOPT_NO_TRAINED              0x0080   /* "Report when colonists trained" */
#define COLOPT_NO_FOOD_SHORTAGE        0x0040   /* "Report food shortages" */
#define COLOPT_NO_MATERIALS_SHORTAGE   0x0020   /* "Report raw materials shortages" */
#define COLOPT_NO_TOOLS_NEEDED         0x0010   /* "Report tools needed for production" */
#define COLOPT_NO_BAD_GOVERNMENT       0x0008   /* "Report inefficient government" */
#define COLOPT_NO_NEW_CARGO            0x0004   /* "Report new cargos available" */
#define COLOPT_NO_SONS_OF_LIBERTY      0x0100   /* "Report Sons of Liberty membership" */
#define COLOPT_NO_REBEL_MAJORITY       0x0200   /* "Report rebel majorities" */

#endif
