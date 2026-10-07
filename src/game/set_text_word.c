/* set_text_word: candidate for 1050:5f55 (39 bytes).  Verdict: RELOCATABLE.
 * symbol: set_text_word
 *
 * Puts entry i of the word table at SEG20:0xdcc8 into TEXT argument slot
 * `slot`.  popup_say is reached by the 4-byte `push cs / call ` form, so
 * it is defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

extern void popup_say(int slot, int word);

void set_text_word(int slot, int i)
{
    popup_say(slot, g_misc_words[i]);
}
