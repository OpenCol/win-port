/* PORT: this file's own declarations of MrTimer_Reset removed: the header declares it from the definition (W1 step 3). */
/* ResetTimer: candidate for 1068:90ba (75 bytes).  Verdict: RELOCATABLE.
 *
 * Reports "Error: MrTimer not initialized in ResetTimer" when the timer object at
 * DGROUP:0x2ae8 is null (and returns), else resets it through MrTimer_Reset(timer, t).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void report_error_1068_963d(void *ctx, char *msg);
#include "records.h"
#include "classes.h"
extern MrTimer * g_timer;   /* DGROUP:0x2ae8, declared */

void ResetTimer(int slot)
{
    if (g_timer == 0) {
        report_error_1068_963d(&g_error_log, "Error: MrTimer not initialized in ResetTimer");
        return;
    }
    MrTimer_Reset(g_timer, slot);
}
