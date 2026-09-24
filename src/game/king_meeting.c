/* king_meeting -- shipped at 1048:6bc1.
 *
 * Ported by tools/port.py from matched/game/king_meeting.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern char g_text_king_again[];                     DGROUP:0x2153
 *   extern char g_text_king_prompt[];                    DGROUP:0x215e
 *   extern char g_text_king_reply1[];                    DGROUP:0x2167
 *   extern char g_text_king_reply5[];                    DGROUP:0x2171
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ask_text_resource(char *name);
void king_adjust_tax(void);
void king_tax_demand(void);
void king_welcome(void);
void recruit_at_university(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */





void king_meeting(void)
{
    register col_i16 r;

    if (g_cur_nation_rec->flags & 0x20) {
        ask_text_resource(g_text_king_again);
        goto out;
    }
    g_cur_nation_rec->flags |= 0x20;
    king_welcome();
    r = ask_text_resource(g_text_king_prompt);
    if (r == 0)
        goto out;
    switch (r) {
    case 1:
        ask_text_resource(g_text_king_reply1);
        break;
    case 2:
        recruit_at_university();
        break;
    case 3:
        king_tax_demand();
        break;
    case 4:
        king_adjust_tax();
        break;
    case 5:
        ask_text_resource(g_text_king_reply5);
        break;
    }
out:
    return;
}
