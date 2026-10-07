/* MSAlertBox: candidate for 1070:65e2 (237 bytes).  Verdict: RELOCATABLE.
 * size-source: 237, the full gap to `alert_box_1070_66cf` at 0x66cf, not the census 151.
 *   The 86 bytes past the census are this function's switch ARMS and both of its jump
 *   tables. Grading at 151 left all of that uncompared, which is how the case order came
 *   to be wrong without any baseline noticing; at 237 the tables are compared and the
 *   corrected order is what makes it match.
 *
 * MessageBox with caption "Potentially fatal error": `icon` 0..3 picks MB_ICONINFORMATION /
 * MB_ICONQUESTION / MB_ICONEXCLAMATION / MB_ICONSTOP, `buttons` a button set (1 OK, 2
 * OK/Cancel, 4 Yes/No, 8 Yes/No/Cancel, 0x10 Abort/Retry/Ignore, 0x20 Retry/Cancel), always
 * task-modal; the answer id 1..7 (OK, Cancel, Abort, Retry, Ignore, Yes, No) is folded through
 * a 7-entry jump table to 0, 1, 4, 5, 6, 2, 3.
 */

#include "colonization.h"

int MSAlertBox(const char *text, int icon, int buttons)
{
    int style = 0;
    int code;
    int r;

    switch (icon) {
    case 0: style = 0x40; break;
    case 1: style = 0x20; break;
    case 2: style = 0x30; break;
    case 3: style = 0x10; break;
    }
    switch (buttons) {
    case 4: style |= 4; break;
    case 8: style |= 3; break;
    case 0x10: style |= 2; break;
    case 1: break;
    case 2: style |= 1; break;
    case 0x20: style |= 5; break;
    }
    r = MessageBox(0, (char *)text, "Potentially fatal error", style | 0x2000);
    switch (r) {
    case 1: code = 0; break;
    case 2: code = 1; break;
    case 6: code = 2; break;
    case 7: code = 3; break;
    case 4: code = 5; break;
    case 3: code = 4; break;
    case 5: code = 6; break;
    }
    return code;
}
