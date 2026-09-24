/* pedia_prompt -- shipped at 1060:203f.
 *
 * Ported by tools/port.py from matched/game/pedia_prompt.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_font_small[];                      DGROUP:0x4b1e
 *
 * The file's own PediaWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: oy -> top
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char * message_buf_1060_686e(void);
col_i16 get_w_2_1060_6886(void *p);
void message_buf_cat_1060_684c(char *s);
void message_buf_set_1060_682a(char *s);

extern unsigned char TextColor(unsigned char v);
extern void draw_string_shadowed(PediaWin *p, void *hf, const char *s,
                                     col_i16 x, col_i16 y, col_i16 align);

/* same object: 4-byte `push cs; call ` */





void pedia_prompt(PediaWin *o)
{
    message_buf_set_1060_682a("Choose a '");
    switch (o->kind) {
    case 0: message_buf_cat_1060_684c("Cargo' "); break;
    case 1: message_buf_cat_1060_684c("Unit' "); break;
    case 2: message_buf_cat_1060_684c("Terrain' "); break;
    case 3: message_buf_cat_1060_684c("Job skill' "); break;
    case 4: message_buf_cat_1060_684c("Building' "); break;
    case 5: message_buf_cat_1060_684c("Founding Father' "); break;
    }
    message_buf_cat_1060_684c("Entry:");
    TextColor(0xc);
    draw_string_shadowed(o, g_font_small, message_buf_1060_686e(),
                         get_w_2_1060_6886(o) / 2, o->top + 0x14, 0);
}
