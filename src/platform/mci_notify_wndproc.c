/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* PORT: lParam's words as Borland's 16-bit casts took them -- i16 for coordinates, sizes and positions, u16 for handles, ids and codes; (unsigned)/(int) keep 32 bits here (W4). */
/* mci_notify_wndproc: candidate for 1078:04b7 (127 bytes).  Verdict: RELOCATABLE.
 * symbol: mci_notify_wndproc
 *
 * The window procedure that owns the game's MCI devices. It handles exactly one message --
 * `MM_MCINOTIFY` (0x3b9) -- and hands everything else to `DefWindowProc`.
 *
 * On a successful notification (`wParam == MCI_NOTIFY_SUCCESSFUL`) the device that finished
 * is closed with `mciSendCommand(id, MCI_CLOSE, 0, 0)`, where the id is the LOW WORD of
 * `lParam`. Then, if that id is one of the two the game tracks at DGROUP:0x3c78 and
 * DGROUP:0x3c7a, the matching cleanup runs and the slot is zeroed -- two device slots, each
 * with its own finished-handler, and nothing happens for an id that is neither.
 *
 * That it is a window procedure is not an inference: the epilogue is `RETF 0xa`, so it is
 * `` with ten bytes of arguments, which is `(HWND, UINT, WPARAM, LPARAM)` exactly, and
 * the default arm calls `DefWindowProc` with all four unchanged.
 *
 * IT IS A `switch`, NOT AN `if`, and the bytes say so before anything else diverges. The
 * shipped code is `MOV AX,SI / CMP AX,0x3b9` -- the message copied into the accumulator and
 * compared there -- followed by Borland's `JZ +2 / JMP` pair. Written as
 * `if (msg != 0x3b9) return DefWindowProc(...)` the same logic compiles to `CMP SI,0x3b9`
 * against the register directly, one byte longer, with a single `JZ`. A `switch` loads its
 * selector into `AX` even when it has one case; an `if` compares the variable where it sits.
 * For a window procedure that is worth knowing, because the source almost always IS a
 * switch and the one-case form is easy to mistake for a guard.
 */

#include "colonization.h"

#include "w16const.h"

long mci_notify_wndproc(unsigned hwnd, unsigned msg, unsigned wp, intptr_t lp)
{
    switch (msg) {
    case MM_MCINOTIFY:
        if (wp == 1) {
            mciSendCommand((u16)lp, 0x804, 0, 0);
            if ((u16)lp == g_mci_device) {
                call_midi_done_callback();
                g_mci_device = 0;
            } else if ((u16)lp == g_cd_device) {
                call_cd_done_callback();
                g_cd_device = 0;
            }
        }
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}
