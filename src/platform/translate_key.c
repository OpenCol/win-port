/* translate_key: candidate for 1070:6831 (64 bytes).  Verdict: RELOCATABLE.
 * symbol: translate_key
 *
 * Maps a virtual key through the 256-entry table at DGROUP:0x33d2 and,
 * for a mapped key, ORs 0x200 for Control and 0x100 for Shift.  The two
 * key tests are -called and defined in-file as stubs.
 */

#include "colonization.h"

extern int g_stub_a;

int translate_key(int key)
{
    register int code;

    code = g_keymap[key & 0xff];
    if (code != 0) {
        if (ctrl_key_down() != 0)
            code |= 0x200;
        if (shift_key_down() != 0)
            code |= 0x100;
    }
    return code;
}
