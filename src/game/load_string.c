/* load_string: candidate for 1040:7872 (70 bytes).  Verdict: RELOCATABLE.
 * symbol: load_string
 *
 * Loads string b + 1 of resource a into the buffer at SEG29:0x10ea, turns
 * its length prefix into a terminator and returns the buffer.
 * pascal_to_c is -called and defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

char *load_string(int id, int index)
{
    g_strbuf[0] = 0;
    load_string_resource(g_strbuf, id, index + 1);
    pascal_to_c(g_strbuf);
    return g_strbuf;
}
