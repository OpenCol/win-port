/* MSShowControl_1060_68a5: candidate for 1060:68a5 (54 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Shows and invalidates a control's window (the handle at +0x10) when it
 * has one.
 *
 * RENAMED 2026-09-27 from control_show_1060_68a5: Mac MSShowControl. Caller: PediaWin_DrawListArea, which is Mac MSShowControl's caller PediaWin::DrawListArea. FAMILY: control_show_* are 4 byte-identical per-module copies (1010:e574, 1050:8459, 1058:c26f, 1060:68a5; show_hwnd + invalidate_hwnd on control+0x10). The copy takes the control record, as the Mac MSShowControl(ControlRecord**) does, so it takes the Mac name, suffixed as the corpus spells per-module copies (MSFlushInput_1010_e5aa, MSEnableItem_1048_e2dd). SEQUENCE PROOF: the Mac pairs MSShowControl with MSDrawControl, and Windows has one call in that place. colony_hammer_change (Mac ...MSHideControl, MSShowControl, MSDrawControl...; Windows ...control_hide, control_show...), MiscButtonClick (4 hides, then Show+Draw twice; Windows 4 hides, 2 shows), TimerProc, colony_hammer_buy, Popup::draw (4 Show+Draw pairs; Windows 4 control_show) and DrawSpriteSquare (3 pairs; Windows 3). On Windows MSDrawControl is folded away and the invalidate does the draw. So mac_bind's proposal control_show_1050_8459 -> MSDrawControl is WRONG: the copy is MSShowControl, and so are its 3 byte-identical siblings. Arity 1.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void MSShowControl_1060_68a5(DlgControl *control)
{
    if (control->hwnd != 0) {
        show_hwnd(control->hwnd);
        invalidate_hwnd(control->hwnd);
    }
}
