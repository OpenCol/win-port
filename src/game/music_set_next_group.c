/* music_set_next_group: candidate for 1040:894a (20 bytes).  Verdict: RELOCATABLE.
 *
 * Queues the next music group in DGROUP:0x1d14. `check_music` is the consumer and spends
 * it in one place -- `g_mood = g_next_mood; g_next_mood = 0;` -- so the word is a one-shot
 * request for the group the next track should come from.
 *
 * THE THREE WORDS ARE ONE FAMILY: 0x1d14 is the queued group, 0x1d16 the current one
 * (`g_mood`, which `set_mood` writes from `force_mood`'s mood id), and
 * 0x1d18 the group that was playing (`g_prev_group`). The corpus says "mood" for the argument
 * and "group" for the words; they are the same 1..7 id seen from the caller's end and the
 * jukebox's.
 *
 * NO CALLER IN THE CENSUS. It is reconstructed because the consumer is unambiguous and because
 * the three-word family is otherwise complete.
 */

#include "colonization.h"

extern int g_next_mood;   /* DGROUP:0x1d14 */

void music_set_next_group(int group)
{
    g_next_mood = group;
}
