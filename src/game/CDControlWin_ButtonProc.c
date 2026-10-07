/* CDControlWin_ButtonProc: candidate for 1040:9ffa (394 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The CDControlWin's six buttons, all wired to this one handler: `DoControlPanel`
 * builds the window, and five OFFSET16 relocations in `CDControlWin_ctor` install this
 * function on the image buttons at +0x7ee and its neighbours. 0x64 plays, 0x65 stops, 0x66
 * toggles pause, 0x67 and 0x68 step the track up and down, and 0x69 works the tray.
 *
 * The four words the panel keeps are the eight bytes `CDControlWin_ctor` already declares as
 * `char tail[8]` at the end of a 0xa5e-byte object: +0xa56 paused, +0xa58 the current track,
 * +0xa5a stopped, +0xa5c the tray. `CDControlWin_DrawTrack` paints the track number -- the
 * "number box" its own header describes reads the same +0xa58.
 *
 * `MSGetCDTracks` is called FIRST and unconditionally, before the window is even fetched,
 * although only the 0x67 arm uses it. That is the shipped order, not a reading of it.
 *
 * The window comes from `GetEventWindow()` and is then walked BACK 0x26 -- the port-to-object
 * step, spelled as the corpus spells it elsewhere: a conditional, because the null case
 * must not be adjusted.
 *
 * Stepping a track only restarts the CD when it was not stopped, so the up/down buttons
 * change the displayed number while stopped without waking the drive.
 *
 * RENAMED 2026-09-26 from cd_control_command: the Mac build's CDControlWin::ButtonProc(short): CDControlWin_Init installs it as the action of the five transport buttons, and it takes the button's code.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);

/* same object: 4-byte `push cs; call ` */
extern void CDControlWin_DrawTrack(CDControlWin *w);

void CDControlWin_ButtonProc(int cmd)
{
    int ntracks;
    CDControlWin *w;
    CDControlWin *cur;

    ntracks = MSGetCDTracks();
    cur = (CDControlWin *)GetEventWindow();
    w = (cur != 0) ? (CDControlWin *)((char *)cur - 0x26) : cur;
    switch (cmd) {
    case 0x64:
        MSPlayCDTrack(w->track);
        CDControlWin_DrawTrack(w);
        w->stopped = 0;
        break;
    case 0x65:
        MSCloseCD();
        w->stopped = 1;
        break;
    case 0x66:
        if (w->paused != 0) {
            MSUnPauseCD();
            w->paused = 0;
        } else {
            MSPauseCD();
            w->paused = 1;
        }
        break;
    case 0x67:
        if (w->track != ntracks) {
            w->track++;
            CDControlWin_DrawTrack(w);
            if (w->stopped == 0)
                MSPlayCDTrack(w->track);
        }
        break;
    case 0x68:
        if (w->track != 1) {
            w->track--;
            CDControlWin_DrawTrack(w);
            if (w->stopped == 0)
                MSPlayCDTrack(w->track);
        }
        break;
    case 0x69:
        if (w->tray_open != 0) {
            MSCloseCD();
            MSOpenCDTray();
            w->stopped = 1;
            w->tray_open = 0;
        } else {
            MSCloseCDTray();
            w->tray_open = 1;
        }
        break;
    }
}
