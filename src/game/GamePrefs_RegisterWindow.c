/* GamePrefs_RegisterWindow: candidate for 1040:73b1 (111 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends an entry -- a window id, an 8-byte rectangle copied by value, and the window's
 * state word -- to the preferences' list of up to 32 twelve-byte placement entries.
 *
 * RENAMED 2026-09-27 from rect_list_add: Mac GamePrefs::RegisterWindow(short, Rect*, short), 116 bytes, against 111. Callers: ColChild::InitCWin (the 11-argument body) and MainWin::Init on both. Layout: Mac LoadPrefs, Default, GetWindow, RegisterWindow, WindowArea, WindowState; Windows GamePrefs_LoadPrefs, Default, GetWindow, 73b1, WindowArea, WindowState, one for one. Arity: this+3 = (root, id, rc, w).
 */

#include "colonization.h"

#include "w16types.h"
#include "records.h"
#include "classes.h"

void GamePrefs_RegisterWindow(GamePrefs *root, int id, RECT *rc, int w)
{
    if (root->count < 0x20) {
        root->windows[root->count].id = id;
        root->windows[root->count].rect = *rc;
        root->windows[root->count].state = w;
        root->count++;
    }
}
