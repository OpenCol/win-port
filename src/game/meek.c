/* meek: candidate for 1030:4c15 (114 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Reads line 1 or 2 of a fixed text resource into name slot `slot`: "request" when flag
 * is set, "demand" otherwise. The failure exit is a `goto` past the loop.
 *
 * RENAMED 2026-09-26 from request_or_demand: the Mac build's meek(short, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the second of the two functions between negotiate_tribe and is_enemy, which are already bound, paired in order; the arity agrees and the Mac body is 1.04x this one.
 */

#include "colonization.h"

extern int text_open(void *ctx, char *name);
extern char *text_get(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void text_close(void);

void meek(int slot, int flag)
{
    int i;
    int n;
    char *line;
    char buf[0x50];

    line = buf;
    if (text_open(g_game_txt, "MEEKNESS"))
        goto done;
    n = flag ? 1 : 2;
    for (i = 0; i < n; i++)
        line = text_get();
done:
    set_name_slot(slot, line);
    text_close();
}
