/* set_flag_bit: candidate for 1050:63ca (49 bytes).  Verdict: RELOCATABLE.
 *
 * Sets or clears one bit of a global flag word, chosen by the caller's bit number and its on/off argument.
 */

#include "colonization.h"

void set_flag_bit(int bit, int on)
{
    if (on)
        g_check_mask |= 1 << bit;
    else
        g_check_mask &= ~(1 << bit);
}
