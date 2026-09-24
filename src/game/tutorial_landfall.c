/* tutorial_landfall -- shipped at 1008:cc71.
 *
 * Ported by tools/port.py from matched/game/tutorial_landfall.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec far g_nation_recs[4];               SEG20:0x7828
 *   extern int far g_turn_nation;                        SEG20:0x7796
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *   extern int far g_text_value;                         DGROUP:0x251a
 *   extern char g_text_ctx[];                            DGROUP:0x009e
 *   extern char far g_seg31_a4[];                        SEG31:0x00a4
 *
 * The file's own NationRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

#include "woodcut.h"
#include "gameflag.h"
extern void woodcut_show(col_i16 a);
extern void prompt_text_name(void *ctx, char *name, col_i16 n, char *out);
extern void text_message_show(char *msg, col_i16 n);

void tutorial_landfall(void)
{
    woodcut_show(WOODCUT_DISCOVERY_OF_THE_NEW_WORLD);
    g_text_value = 0;
    prompt_text_name(g_text_ctx, "LANDHO", 0x17, ((char *)((unsigned char *)&(g_nation_recs[g_turn_nation]) + 0x0)));
    strcpy(((char *)((unsigned char *)&(g_nation_recs[g_turn_nation]) + 0x0)), ((char *)g_seg31_a4));
    if (g_game_flags & GAMEFLAG_TUTORIAL)
        text_message_show("TUTORIAL2", 0);
}
