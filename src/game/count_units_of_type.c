/* count_units_of_type: candidate for 1038:1b3f (78 bytes).  Verdict: RELOCATABLE.
 * symbol: count_units_of_type
 *
 * How many units of u's chain have the given type.  Both chain helpers are
 * -called and defined in-file as stubs.
 */

#include "colonization.h"

#include "records.h"

extern int g_stub_a;
extern int top_unit(int u);
extern int next_unit(int u);

int count_units_of_type(int u, unsigned char type)
{
    int n = 0;

    for (u = top_unit(u); u >= 0; u = next_unit(u)) {
        if (g_units[u].type == type)
            n++;
    }
    return n;
}
