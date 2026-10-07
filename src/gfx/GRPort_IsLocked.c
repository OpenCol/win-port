/* GRPort_IsLocked: candidate for 1068:9729 (37 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_IsLocked
 *
 * Whether the port is locked -- whether `MSLockGraf` has left a surface at
 * +0x18. `GRPort_dtor` is the clearest caller: it asks this and, only if the answer is
 * yes, calls `MSUnlockGraf`. `GRPort_Init`, `GRPort_FillColor`, `ChangeColor`,
 * `CopyToScreen` and `Sprite_ExtractSprite` use it the same way, and all seven callers pass
 * their own `this`, which is what settles the record as a GRPort.
 *
 * The Macintosh build has `GRPort::Lock(void)` and `GRPort::Unlock(void)` but no
 * `IsLocked` on the port -- only `Sprite::IsLocked(void)`, which is the other file at
 * `1068:7aea`. So this name is the description its callers agree on and not a symbol
 * taken from the other build.
 *
 * Both arms load `AL` before a single epilogue, which by rule 16 is a conditional
 * expression, with the `== 0` test that way round rather than a `!= 0`.
 *
 * THE FIELD IS A HUGE POINTER, and that is a correction. This file used to declare it
 * `long` and carry `flags: -ml -WS -Od -Ot`, concluding that its module was built for the
 * 8086 while its neighbours were built for the 386 -- because a `long` tested against
 * zero is `MOV AX,[+0x18] / OR AX,[+0x1a]` only without `-3`, and one `CMP DWORD` with
 * it. A **** pointer is tested in halves under every flag set, because it may be
 * unnormalised (rule 38). Declared `` this is EXACT at the default flags, the
 * per-module 8086 claim goes away, and the type agrees with the lock half, which cannot
 * be read any other way.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

char GRPort_IsLocked(GRPort *p)
{
    return p->bits == 0 ? 0 : 1;
}
