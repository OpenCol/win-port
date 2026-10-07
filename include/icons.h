/* icons.h -- where the named runs start inside the two sprite sheets of segment 20.
 *
 * g_icon_sprites (SEG20:0x6246, 0x84 sprites of 0x20 bytes) and g_terrain_sprites
 * (SEG20:0x4506, 0xbe sprites) are each ONE array in the shipped code: every reference to a run
 * inside them carries the sheet's own symbol in its segment fixup (tools/data_owners.py), so
 * the source indexed the sheet. These are the runs the corpus had named as separate globals,
 * as indices: `&g_icon_sprites[ICON_GOODS + goods]` is what `&g_goods_sprites[goods]` was.
 * (A constant ADDED to an index folds into the relocation in C and C++ alike; a subtracted one
 * does not fold in every context.)
 */
#ifndef ICONS_H
#define ICONS_H

/* g_icon_sprites */
#define ICON_COLONY         1       /* the colony pictures, by size level */
#define ICON_SETTLEMENT     11      /* the native settlement pictures, by level */
#define ICON_CURSOR         19      /* the map cursor */
#define ICON_GOODS          23      /* one per goods, GOODS_FOOD .. GOODS_MUSKETS */
#define ICON_BOYCOTT        56      /* the boycott mark */
#define ICON_BELL           63      /* the liberty bell */
#define ICON_CELL_FRAME     65      /* the colony map's cell frame */
#define ICON_EXPERT         82      /* one per job: the expert portraits */
#define ICON_CLAIM          109     /* the colony map's land-claim mark */
#define ICON_MOOD           114     /* the natives' moods */
#define ICON_EMPTY_HOLD     123     /* an empty cargo hold */
#define ICON_SOL_FLAG       124     /* the rebel flag (Sons of Liberty) */
#define ICON_TORY_FLAG      125     /* the royal flag */

/* g_terrain_sprites */
#define TSPR_MOUNTAINS      33      /* the mountain tiles, by neighbour pattern */
#define TSPR_HILLS          49      /* the hill tiles */
#define TSPR_FOREST         65      /* the forest tiles */
#define TSPR_BONUS          90      /* the bonus resources */
#define TSPR_FOG            149     /* the unexplored-square fog */

#endif
