/* win_release_capture: candidate for 1078:2d3e (19 bytes).  Verdict: RELOCATABLE.
 *
 * Releases the mouse capture.
 *
 * The unused argument is a pointer, as every caller passes one (2026-10-02; it was `long`).
 */

#include "colonization.h"

void win_release_capture(void *unused1) { ReleaseCapture(); }
