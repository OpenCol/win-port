/* btoi: candidate for 1040:5d3a (99 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * xtoi's twin for binary digits: '0' and '1' shift the value up, anything else skips
 * to the end of the string.
 *
 * RENAMED 2026-09-27 from parse_binary: BEYOND THE PROPOSALS, the twin of xtoi. Mac btoi(char*), CODE 11 0x4abe, 88 bytes, called by stoi, between xtoi and stoi. 5d3a (char *p), 99 bytes, called only by stoi, between parse_hex and stoi. Arity char* on both.
 */

#include "colonization.h"

int btoi(char *p)
{
    int value = 0;
    int c;

    while (*p != 0) {
        c = *p;
        p++;
        c = toupper_rtl(c);
        if (c == '0' || c == '1') {
            value = value * 2;
            value += c - '0';
        } else {
            while (*p != 0)
                p++;
        }
    }
    return value;
}
