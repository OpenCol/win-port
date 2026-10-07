/* remove_ms_window: candidate for 1078:1e6b (107 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Removes handle h from the MS window stack: find it, shift the rest down, drop the
 * count; a count below zero is reported as "Error: MS window stack inaccurate", a literal
 * (DGROUP:0x3ebf, in segment 16's pool after "MSWindowClass": tools/check_module_data.py).
 *
 * RENAMED 2026-09-26 from check_ms_window_stack: Finds handle h in the MS window stack at DGROUP:0x5728, shifts the later entries down and decrements the count at 0x3c80. Its caller is MSDisposeWindow, and it is the inverse of push_ms_window. The only 'check' is a report if the count goes below zero, which is a minor side effect. The old name hides that the function modifies the stack.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void remove_ms_window(int h)
{
    int i;

    for (i = 0; i < g_ms_window_count; i++)
        if (g_ms_window_stack[i] == h)
            break;
    if (g_ms_window_count - 1 > i)
        for (; g_ms_window_count - 1 > i; i++)
            g_ms_window_stack[i] = g_ms_window_stack[i + 1];
    g_ms_window_count--;
    if (g_ms_window_count < 0)
        stub_reporter(&g_error_log, "Error: MS window stack inaccurate");
}
