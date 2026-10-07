/* PediaWin_KeyClick: candidate for 1060:1fce (113 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The Colonizopedia window's key handler: key 0xd2 runs PediaWin_Done on the current
 * window's owner (a call TLINK converted); any other key goes to the game's keyboard
 * accelerators unless the window is in its modal form (+0x857).
 *
 * RENAMED 2026-09-26 from pedia_key: the Mac build's PediaWin::KeyClick(short): PediaWin_Show installs it as the window's keydown handler, and it takes the key.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);
extern void PediaWin_Done(PediaWin *o);
extern void KeyEvents(int key);

void PediaWin_KeyClick(int key)
{
    PediaWin *o;
    char *w = (char *)GetEventWindow();

    o = (PediaWin *)(w != 0 ? w - 0x26 : w);
    switch (key) {
    case 0xd2:
        PediaWin_Done(o);
        break;
    default:
        if (o->modal == 0)
            KeyEvents(key);
        break;
    }
}
