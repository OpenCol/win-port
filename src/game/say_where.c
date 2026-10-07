/* say_where: candidate for 1058:923d (308 bytes).  Verdict: RELOCATABLE.
 *
 * Names a location into `buf`: off the map it is Europe -- the nation's name when the x
 * coordinate is that nation's Europe slot (nation - 20), else the generic word at
 * SEG20:0xdd40; on the map a sea-lane square (terrain 0x1a) with `flag` set is also the
 * nation; otherwise the colony there (DGROUP:0x4ba2 receives its index) by name, or the bare
 * "(x, y)".
 */

#include "colonization.h"

#include "terrain.h"
#include "misc.h"
#include "records.h"

extern void open_string(char *s);
extern void close_string(char *s);
extern void comma_string(char *buf);

void say_where(char *buf, int x, int y, int nation, int flag)
{
    if (!on_map(x, y)) {
        if (nation >= 0 && (char)(nation - 20) == (char)x)
            say_string(buf, g_nation_name_word[nation]);
        else
            say_string(buf, g_misc_words[MISC_HIGH_SEAS]);
    } else if (terrain_at(x, y) == TERRAIN_SEA_LANE && flag != 0) {
        say_string(buf, g_nation_name_word[nation]);
    } else {
        g_cur_colony_idx = find_colony(x, y);
        if (g_cur_colony_idx >= 0)
            STRCPY(buf, g_colonies[g_cur_colony_idx].name);
        else {
            open_string(buf);
            say_number(buf, x);
            comma_string(buf);
            say_number(buf, y);
            close_string(buf);
        }
    }
}
