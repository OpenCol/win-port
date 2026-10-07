/* push_ms_window: candidate for 1078:1e4c (31 bytes).  Verdict: RELOCATABLE.
 *
 * Appends a word to the table at DGROUP:0x5728, the count at DGROUP:0x3c80.
 *
 * RENAMED 2026-09-26 from push_word_5728: appends a window handle to g_ms_window_stack and counts it;
 * remove_ms_window removes it again, and the MSWindow/MSMovie creators push each new window.
 */

#include "colonization.h"

void push_ms_window(int hwnd)
{
    g_ms_window_stack[g_ms_window_count] = hwnd;
    g_ms_window_count++;
}
