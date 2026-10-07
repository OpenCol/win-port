/* destroy_MSMMWindow_window: candidate for 1078:09ac (35 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The multimedia shutdown, and the mirror of `create_MSMMWindow_window` at 1078:0426:
 * close the wave device, close the CD, stop whatever MCI device is still running, then
 * destroy the notification window whose handle sits at DGROUP:0x5726 -- the same handle
 * `MSPlayCDTrack` and the track-stepping pair hand to `mciSendCommand` as the notify target.
 *
 * Its one caller sits a few bytes after the startup sequence that creates that window,
 * opens the resource library and calls `seed_random_from_clock`.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

void destroy_MSMMWindow_window(void)
{
    MSCloseSound();
    MSCloseCD();
    stop_mci_device_1078_0940();
    DestroyWindow(g_cd_notify_hwnd);
}
