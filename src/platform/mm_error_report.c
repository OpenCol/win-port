/* mm_error_report: candidate for 1078:3359 (14 bytes).  Verdict: EXACT.
 *
 * The multimedia module's error reporter (context, message, code): empty in the shipped
 * build; MSPlayCDTrack and the audio procedure call it.
 */

#include "colonization.h"

void mm_error_report(void *ctx, char *msg, long code)
{
}
