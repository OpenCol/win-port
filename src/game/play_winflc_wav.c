/* play_winflc_wav -- shipped at 1010:0312.
 *
 * Ported by tools/port.py from matched/game/play_winflc_wav.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSPlaySound_1068_a582(char *name);

void play_winflc_wav(void)
{
    MSPlaySound_1068_a582("winflc.wav");
}
