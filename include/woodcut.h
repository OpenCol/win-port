/* woodcut.h -- the full-screen woodcut illustrations, by the id `woodcut` takes.
 *
 * Source: the WOODCUT list resource (TEXT 716 in COLTEXT0.DLL), one title a line, whose line
 * index IS the id. tools/check_headers.py re-reads it, so the table is derived from the game
 * rather than transcribed -- the bar every table here has to meet, because a wrong value
 * compiles to the same bytes as a right one.
 *
 * The call sites agree with the titles without being told to: `tribe_village` shows
 * 7 and 7 is ENTERING INDIAN VILLAGE, `lost_city_exploration`'s first outcome shows 8 and 8 is THE
 * FOUNTAIN OF YOUTH.
 *
 * THE LIST IS LONGER THAN THE ART. Lines 14, 15 and 16 read "woodcut 14", "woodcut 15" and
 * "woodcut 16" -- placeholder titles for ids the game never fills in. They are NOT named here:
 * a macro for a placeholder would look like a fact. `woodcut`'s own switch runs 0..10,
 * which is narrower still.
 */
#ifndef WOODCUT_H
#define WOODCUT_H

#define WOODCUT_A_NEW_WORLD                  0
#define WOODCUT_DISCOVERY_OF_THE_NEW_WORLD   1
#define WOODCUT_BUILDING_A_COLONY            2
#define WOODCUT_MEETING_THE_NATIVES          3
#define WOODCUT_THE_AZTEC_EMPIRE             4
#define WOODCUT_THE_INCA_NATION              5
#define WOODCUT_DISCOVERY_OF_THE_PACIFIC_OCEAN 6
#define WOODCUT_ENTERING_INDIAN_VILLAGE      7
#define WOODCUT_THE_FOUNTAIN_OF_YOUTH        8
#define WOODCUT_CARGO_FROM_THE_NEW_WORLD     9
#define WOODCUT_MEETING_FELLOW_EUROPEANS     10
#define WOODCUT_COLONY_BURNING               11
#define WOODCUT_COLONY_DESTROYED             12
#define WOODCUT_INDIAN_RAID                  13

#endif
