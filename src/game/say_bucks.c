/* say_bucks: candidate for 1040:a486 (49 bytes).  Verdict: RELOCATABLE.
 * symbol: say_bucks
 *
 * Appends an amount of money: the number and the currency sign (DGROUP:0x1fbc, "\xa4").
 * say_long is defined here because the shipped call to it is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

void say_bucks(char *s, long amount)
{
    say_long(s, amount);
    STRCAT(s, "\xa4");
}
