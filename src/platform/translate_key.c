/* translate_key -- shipped at 1070:6831.
 *
 * Ported by tools/port.py from matched/platform/translate_key.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_keymap[];                               DGROUP:0x33d2
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ctrl_key_down(void);
col_i16 shift_key_down(void);

col_i16 translate_key(col_i16 key)
{
    register col_i16 code;

    code = g_keymap[key & 0xff];
    if (code != 0) {
        if (ctrl_key_down() != 0)
            code |= 0x200;
        if (shift_key_down() != 0)
            code |= 0x100;
    }
    return code;
}
