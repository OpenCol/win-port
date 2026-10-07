/* AboutBox: candidate for 1060:71fc (175 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: AboutBox
 * name-source: sibling-build
 *
 * The About box: an AboutWin and a ResourceFile on the stack, the map scale SAVED and set to
 * 1x1 for the duration, `AboutWin_DoCredits` to run the window, and the scale put back before the
 * two destructors fire.
 *
 * The save-and-restore around the scale is the shape worth naming. `GetGlobalSpriteScale`
 * reads the current pair out by reference, `SetGlobalSpriteScale(1, 1)` forces 1x1 so
 * the About art is drawn unscaled, and the same pair goes back in afterwards --
 * `UpdateStats` does the same two calls for the same reason.
 *
 * `MSShowMenuBar` between the run and the restore is the fourteen-byte empty function;
 * `EuropeWin_Close` calls it too.
 *
 * The frame is the sum of the two objects: AboutWin 0xbd6 at `bp-0xbd8` ending exactly where
 * the two-byte ResourceFile begins, which is the `show_opening` arithmetic again.
 *
 * size-source: 175, the census, and NOT the 204-byte gap. The 29 bytes between are a
 * padding byte and the next function's exception descriptor, which is data. The census is
 * right here and the gap is not, which is the case its own note warns about.
 *
 * The literal is `coldata5.dll` at DGROUP:0x29e4 -- the same file `show_opening` opens, from
 * a different copy of the string.
 *
 * RENAMED 2026-09-27 from about_colonization: Mac AboutBox(void), 122 bytes, against 175. Caller: MainWin::MainMenuProc on both (WM_COMMAND 706). Callees: AboutWin ctor, DoCredits, Get/SetGlobalSpriteScale and MSShowMenuBar on both; Windows adds its coldata5.dll ResourceFile. Layout: the Mac CODE 11 AboutBox, AboutWin::<ctor>, <dtor>, DoCredits is the Windows 1060 71fc, AboutWin_ctor, AboutWin_dtor, AboutWin_DoCredits, one for one. Arity: void.
 * ResourceFile's constructor and destructor are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
extern "C" void GetGlobalSpriteScale(int *num, int *den);
extern "C" void SetGlobalSpriteScale(int num, int den);
extern "C" void AboutWin_DoCredits(AboutWin *w);
extern "C" void MSShowMenuBar(void);

extern "C" void AboutBox(void)
{
    AboutWin w;
    ResourceFile rf("coldata5.dll");
    int x;
    int y;

    GetGlobalSpriteScale(&x, &y);
    SetGlobalSpriteScale(1, 1);
    AboutWin_DoCredits(&w);
    MSShowMenuBar();
    SetGlobalSpriteScale(x, y);
}
