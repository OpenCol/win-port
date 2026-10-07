/* say_string: candidate for 1040:a33d (40 bytes).  Verdict: RELOCATABLE.
 * symbol: say_string
 *
 * Appends TEXT string `word` to the message being built.  strings is
 * -called and defined in-file as a stub.
 */

#include "colonization.h"

extern char g_stub_s[];

extern char *strings(int n);

void say_string(char *buf, int word)
{
    STRCAT(buf, strings(word));
}
