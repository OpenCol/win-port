/* PORT: this file's own declarations of build_window_with_port, get_win_1068_d72a, subclass_control_1070_bb63 removed: the header declares them from the definition (W1 step 3). */
/* build_window_sized: candidate for 1068:a76a (144 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: build_window_sized
 * name-source: descriptive
 *
 * Builds a window with a port at w x h and then tells the port at +0xc4 about the rectangle
 * it was given: `SetRect(&r, 0, 0, w, h)`, `build_window_with_port` with all nine of the
 * caller's other words, `subclass_control_1070_bb63` on the window at +0x26, and finally a
 * VIRTUAL call on the port -- `MOV BX,[es:bx+0xc4] / CALL FAR [BX]`, the first slot of its
 * vtable, with the rectangle.
 *
 * That last call is the only reason this file is C++. The vtable pointer is a NEAR word at
 * the start of the sub-object and the entry it reads is a function; `-P` and a
 * `virtual` declaration produce both, where a hand-written function-pointer table under
 * `-ml` gives a pointer and the wrong load.
 *
 * `SetRect` is ``, so its arguments go left to right and the width and height are the
 * LAST pair pushed, not the first.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void build_window_sized(MagicWin *o, char *name, int style,
                                                 int x, int y, register int w,
                                                 register int h, void *pal)
{
    RECT r;

    SetRect(&r, 0, 0, w, h);
    build_window_with_port((GameWin *)o, name, style, x, y, w, h, pal);
    subclass_control_1070_bb63(o, (MSWin *)(get_win_1068_d72a((Screen *)(&(DialogWin &)*o))));
    o->port.ResizePort(&r);
}
