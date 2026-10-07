/* MainWin_OnMove: candidate for 1048:cc60 (230 bytes).  Verdict: RELOCATABLE.
 *
 * Like MainWin_OnSize with the tests reordered: a minimised (iconic) window records
 * code 1; otherwise a window smaller than the desktop in both directions records its
 * rectangle with code 0; anything else leaves the record alone.
 *
 * RENAMED 2026-09-26 from its slot number. MainWin_Init installs it on handler slot +68, which
 * the window procedure dispatches on WM_MOVE; the Mac class has OnMove and OnSize, and finding 60
 * could name the pair but not order it. MainWin_OnSize is the WM_SIZE half.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
extern void *GetEventWindow(void);

extern PlacementRec *GamePrefs_GetWindow(void *root, int which);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern void MSGetWinSize_of_1048_e1d6(Screen *w, RECT *r);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern int MSIsWindowMinimized_of_1048_d8ad(Screen *w);

void MainWin_OnMove(void)
{
    Screen *w;
    PlacementRec *rec;
    RECT desk;
    RECT wr;

    w = (Screen *)GetEventWindow();
    rec = GamePrefs_GetWindow(&g_prefs_root, 1);
    if (rec != 0) {
        MSGetWinSize_of_1048_e1d6(w, &wr);
        MSGetDesktopSize(&desk);
        if (MSIsWindowMinimized_of_1048_d8ad(w)) {
            rec->state = 1;
        } else if (rect_width_1048_d69b(&desk) > rect_width_1048_d69b(&wr) && rect_height_1048_d6ba(&desk) > rect_height_1048_d6ba(&wr)) {
            rec->rect.left = wr.left;
            rec->rect.top = wr.top;
            rec->rect.right = wr.right;
            rec->rect.bottom = wr.bottom;
            rec->state = 0;
        }
    }
}
