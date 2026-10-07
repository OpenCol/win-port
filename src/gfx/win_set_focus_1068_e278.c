/* win_set_focus_1068_e278: candidate for 1068:e278 (33 bytes).  Verdict: RELOCATABLE.
 *
 * Gives the window's handle (pointer at +6) the input focus -- module 1068's copy of
 * `win_set_focus` (1060:67c3). Its one caller is `audio_play_element`, which takes the
 * focus back after handing the MCI device a window to notify.
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void MSSetFocus(void *hwnd);

void win_set_focus_1068_e278(DialogWin *w)
{
    MSSetFocus((MSWin *)w->win);
}
