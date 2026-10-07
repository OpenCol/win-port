/* great: candidate for 1030:4b8b (138 bytes, 12 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Loads the text resource "<prefix><name>", reads line n (0-based) of it, stores that line
 * in name slot `slot`, and releases. On a load failure the slot gets the local buffer holding the resource name "<prefix><name>".
 * The failure exit is a `goto` past the loop.
 *
 * RENAMED 2026-09-26 from load_named_text_line_to_slot: the Mac build's great(short, char*, short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the first of the two functions between negotiate_tribe and is_enemy, which are already bound, paired in order; the arity agrees and the Mac body is 1.03x this one.
 */

#include "colonization.h"

extern int text_open(void *ctx, char *name);
extern char *text_get(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void text_close(void);

void great(int slot, char *name, int n)
{
    int i;
    char *line;
    char buf[0x50];

    line = buf;
    STRCPY(buf, "GREAT");
    STRCAT(buf, name);
    if (text_open(g_game_txt, buf))
        goto done;
    for (i = 0; i <= n; i++)
        line = text_get();
done:
    set_name_slot(slot, line);
    text_close();
}
