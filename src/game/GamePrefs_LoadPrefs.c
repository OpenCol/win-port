/* PORT: this file's own declarations of file_MSOpenFile_1040_a922, read_file_1040_a84d removed: the header declares them from the definition (W1 step 3). */
/* GamePrefs_LoadPrefs: candidate for 1040:719a (326 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GamePrefs_LoadPrefs
 *
 * Reads COLWIN.PRF into the 0x19c-byte preferences record. A file that will not open, or
 * one that is not exactly 0x19c bytes long, is discarded without reading -- the length is
 * the whole integrity check.
 *
 * Even a good file is only trusted if the desktop has not changed size: the saved width and
 * height at +4 and +6 are compared against what `MSGetDesktopSize` reports now, and a
 * mismatch throws the layout away and re-derives it with `GamePrefs_Default`. The long at
 * +0 is saved across that call and put back, so whatever it holds survives a resolution
 * change when nothing else does.
 *
 * `GamePrefs_Default` is then called AGAIN on that path, because the mismatch branch falls
 * into the same tail the two failure paths reach. Twice is what the shipped code does and
 * the second call is not conditional on the first having been skipped.
 *
 * The size test is `if (a != w || b != h) { ... } else { ...; return; }` and not the
 * positive form with the early return first. Borland gives the `||` a `JNZ` into the body
 * and a `JZ` into the else; the `&&` form gives two `JNZ`s past it, and the shipped `JZ` at
 * +0xb5 is the only thing that tells them apart.
 *
 * A local DiskFile, so the function carries the exception block; the class tower is
 * DiskFile_ctor.c's.
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
/* PORT: this file's own DiskFile, GamePrefs removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"


extern void GamePrefs_Default(GamePrefs *prefs);



void GamePrefs_LoadPrefs(GamePrefs *p)
{
    DiskFile f;
    RECT r;
    long save;

    if (file_MSOpenFile_1040_a922((MSFile *)&f, "COLWIN.PRF") != 0 && MSFileSize_1040_aa60((MSFile *)&f) == 0x19cL) {
        read_file_1040_a84d((MSFile *)&f, p, 0x19cL);
        MSGetDesktopSize(&r);
        if (rect_width_1040_a903(&r) != p->w || rect_height_1040_aa80(&r) != p->h) {
            save = p->f0;
            GamePrefs_Default(p);
            p->f0 = save;
        } else {
            MSCloseFile_1040_a956((MSFile *)&f);
            return;
        }
    }
    MSCloseFile_1040_a956((MSFile *)&f);
    GamePrefs_Default(p);
}

