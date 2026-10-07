/* popup_ask_number: candidate for 1050:6350 (102 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: popup_ask_number
 *
 * Runs TEXT dialog `name` with the number `n` (LTOA1 base 10) as its default
 * answer, then parses the answer buffer (SEG31:0xa4) back into the first TEXT
 * argument slot (SEG31:0x2d4).  Returns the dialog's result.  The two
 * -called helpers are defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from prompt_text_number: the Mac build's popup_ask_number(char*, char*, int), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller MainMenuProc; caller attempt_cargo_to_colony; caller attempt_cargo_to_ship), the runner-up gets 2, and the arity agrees (3). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

extern int popup_ask(void *ctx, char *name, int max, char *buf);
extern int ATOL_1050_8de2(char *digits);

int popup_ask_number(char *ctx, char *name, int n)
{
    int result;
    char buf[20];

    LTOA1(n, buf, 10);
    result = popup_ask(ctx, name, 5, buf);
    g_text_args[0] = ATOL_1050_8de2(g_name_slots[0]);
    return result;
}
