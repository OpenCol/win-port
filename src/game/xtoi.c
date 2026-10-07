/* xtoi: candidate for 1040:5cbb (127 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Parses hexadecimal digits from p, upper-casing each; a digit shifts the value by four
 * and adds; any other character skips to the end of the string.
 *
 * RENAMED 2026-09-27 from parse_hex: CONST PROPOSAL. Mac xtoi(char*), CODE 11 0x4a48, 118 bytes, called by stoi. 5cbb (char *p), 127 bytes, called only by stoi (already the Mac stoi). Mac stoi's seq opens xtoi, btoi, and Windows stoi calls parse_hex and parse_binary. Layout: Mac xtoi, btoi, stoi; Windows parse_hex, parse_binary, stoi. Windows calls only toupper_rtl; the Mac xtoi makes no out-of-line call. Size 118 against 127. Constants: overlap 0.70, runner-up 0.36. Weaker constants, but the call graph, layout, arity and size all agree.
 */

#include "colonization.h"

/* Borland's character-class table, DGROUP:0x46c0 (CWL.LIB's ctype module), as ctype.h's
   macros index it: isdigit(c) is `_ctype[(c) + 1] & _IS_DIG` (2). The shipped code reaches it
   through its segment (`mov ax, seg __ctype`), so the game's declaration was -- ctype.h's
   is , and BCC refuses a redeclaration after it ("Multiple declaration") */
extern unsigned char _ctype[257];   /* DGROUP:0x46c0, declared */

int xtoi(char *p)
{
    int result = 0;
    int ch;

    while (*p != 0) {
        ch = *p;
        p++;
        ch = toupper_rtl(ch);
        if (_ctype[ch + 1] & 2) {           /* isdigit(ch) */
            result = result << 4;
            result += ch - '0';
        } else if (ch >= 'A' && ch <= 'F') {
            result = result << 4;
            result += ch - 0x37;
        } else {
            while (*p != 0)
                p++;
        }
    }
    return result;
}
