/* ask_choice_noparent: candidate for 1068:ccd3 (37 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `ask_choice(msg, options, NULL)`: the choice dialog with no parent window. It sits
 * directly before ask_choice itself.
 *
 * The call is a FAR one although ask_choice is in this same segment -- `9A` with a
 * relocation, not `push cs; call ` -- so it is extern.
 */

#include "colonization.h"

int ask_choice_noparent(char *msg, char *options)
{
    return ask_choice(msg, options, 0);
}
