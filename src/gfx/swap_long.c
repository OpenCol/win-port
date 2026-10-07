/* swap_long: candidate for 1068:976d (132 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * Ghidra sizes this at 128 because the last four bytes -- a second POP DS / POP BP /
 * DEC BP / RETF after the `return` -- are unreachable; the candidate reproduces all 132.
 * Only the 16 linker-patched bytes of the four calls are set aside.
 *
 * Four masked byte fields of a 32-bit value, each moved through the runtime's long
 * shift helpers (1000:3cca = LXLSH, 1000:3ceb = LXURSH -- the latter uses SHR DX,CL,
 * so the operand is UNSIGNED), OR'd pairwise left to right. A big-endian to
 * little-endian byte swap. The one caller copies 8 bytes out of a buffer with
 * MSCopyMem, swaps the first long with this, and rounds it up to even: reading a
 * Mac-origin record.
 */

#include "colonization.h"

unsigned long swap_long(unsigned long v)
{
    return ((v & 0xffL) << 24) | ((v & 0xff00L) << 8)
         | ((v & 0xff0000L) >> 8) | ((v & 0xff000000L) >> 24);
}
