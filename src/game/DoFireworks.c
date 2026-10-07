/* DoFireworks: candidate for 1010:0312 (26 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Plays "winflc.wav" (the string at DGROUP:0x9ab) through PlaySnd.
 *
 * RENAMED 2026-09-26 from play_winflc_wav: the Mac build's DoFireworks(void), bound by layout (between DoTheClosing and ClosingWin_DoClosing on both sides); the arity agrees and the Mac body is 1.38x this one. It plays the victory animation file, WIN.FLC -- the fireworks.
 */

#include "colonization.h"

extern void PlaySnd(char *name);

void DoFireworks(void)
{
    PlaySnd("winflc.wav");
}
