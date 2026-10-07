/* MainWin_OnSize: candidate for 1048:cd46 (241 bytes).  Verdict: RELOCATABLE.
 *
 * Records the main window's placement in the preferences record fetched by
 * GamePrefs_GetWindow(&SEG20:0x10, 1): code 2 when the window is at least as large as the desktop in
 * both directions, 1 when MSIsWindowMinimized_of_1048_d8ad says it is minimised, else its rectangle with code 0.
 *
 * RENAMED 2026-09-26 from its slot number. MainWin_Init installs it on handler slot +64, which
 * the window procedure dispatches on WM_SIZE; the Mac class has OnMove and OnSize, and finding 60
 * could name the pair but not order it. MainWin_OnMove is the WM_MOVE half.
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

void MainWin_OnSize(void)
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
        if (rect_width_1048_d69b(&desk) <= rect_width_1048_d69b(&wr) && rect_height_1048_d6ba(&desk) <= rect_height_1048_d6ba(&wr)) {
            rec->state = 2;
        } else if (MSIsWindowMinimized_of_1048_d8ad(w)) {
            rec->state = 1;
        } else {
            rec->rect.left = wr.left;
            rec->rect.top = wr.top;
            rec->rect.right = wr.right;
            rec->rect.bottom = wr.bottom;
            rec->state = 0;
        }
    }
}
