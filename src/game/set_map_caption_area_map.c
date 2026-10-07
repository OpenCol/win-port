/* set_map_caption_area_map: candidate for 1038:91a4 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Sets the map window's caption to "Area Map", the string at DGROUP:0x1cac, through
 * `game_win_set_text_1038_dffc` on the map window object at SEG20:0x0658. That is the caption
 * `message_timer_expired` writes back when the map's message timer runs out, so it is the
 * map window's resting title.
 *
 * The call is the four-byte `push cs; call ` with no NOP, so the callee is defined here
 * as a stub (rule 47).
 */

#include "colonization.h"

#include "classes.h"

extern char g_area_map_str[];   /* DGROUP:0x1cac, "Area Map" */

/* same object: 4-byte `push cs; call ` (rule 47) */

void set_map_caption_area_map(void)
{
    game_win_set_text_1038_dffc(&g_map_window, g_area_map_str);
}
