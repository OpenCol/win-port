/* say_terrain: candidate for 1040:a628 (122 bytes, 2 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends the name of kind k from a 16-byte-record table (a default word when k is
 * negative), then for kinds 8..23 a space and a second word.
 *
 * RENAMED 2026-09-26 from say_word_kind: the Mac build's say_terrain(char*, short), bound by call-graph agreement: 3 of its Mac call edges already bound land on this function (caller InfoTerrain; callee say_string; callee space_string), the runner-up gets 2; the arity agrees (2), and the body does what the Mac name says.
 */

#include "colonization.h"

#include "labels.h"
#include "misc.h"
#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void space_string(char *buf);
void say_terrain(char *s, int kind)
{
    int n = kind;

    if (n >= 0)
        say_string(s, g_terrain_rows[n].name);
    else
        say_string(s, g_misc_words[MISC_LAKE]);
    if (n >= 8 && n < 0x18) {
        space_string(s);
        say_string(s, g_feature_words[FEATURE_FOREST]);
    }
}
