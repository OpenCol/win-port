/* dialog_mnemonic -- shipped at 1060:9c4c.
 *
 * Ported by tools/port.py from matched/game/dialog_mnemonic.c.
 *
 * The file's own RadioItem, Ctl, Win dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: state -> accel
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
RadioItem * get_p_2e_1060_ac7c(Ctl *c);
col_i16 ctl_kind_1060_ac19(Ctl *c);
col_i16 get_w_10_1060_ac4a(Ctl *c);
col_i16 get_w_1a_1060_ac31(Ctl *c);
col_i16 get_w_20_1060_ac63(Ctl *c);
col_i16 get_w_26_1060_ac99(Ctl *c);

extern char * STRLWR(char *s);
extern void button_click(col_u16 hwnd);
extern void checkbox_click(col_u16 hwnd);
extern void radio_click(col_u16 hwnd);

/* same object: 4-byte `push cs; call ` (rule 47) */







col_i16 dialog_mnemonic(Win *w, char key)
{
    Ctl *c;
    char buf[2];
    char found;
    Ctl *grp;
    RadioItem *items;
    col_i16 n;
    register col_i16 i;
    register col_i16 r;

    found = 0;
    r = 0;
    buf[0] = key;
    buf[1] = 0;
    STRLWR(buf);
    key = buf[0];
    for (c = (*((Ctl * *)((unsigned char *)(w) + 0x8c))); c != 0 && found == 0; c = (*((void * *)((unsigned char *)(c) + 0x12)))) {
        if (c->accel >= 0) {
            if ((*((char *)((unsigned char *)(c) + 0x17))) == key) {
                if (ctl_kind_1060_ac19(c) == 6 && get_w_1a_1060_ac31(c) != 0) {
                    button_click(get_w_10_1060_ac4a(c));
                    r = 1;
                    break;
                } else if (ctl_kind_1060_ac19(c) == 2 && get_w_20_1060_ac63(c) != 0) {
                    checkbox_click(get_w_10_1060_ac4a(c));
                    r = 1;
                    break;
                }
            }
        } else if (ctl_kind_1060_ac19(c) == 3) {
            grp = c;
            items = get_p_2e_1060_ac7c(grp);
            n = get_w_26_1060_ac99(grp);
            for (i = 0; i < n; i++) {
                if (items[i].state >= 0 && (*((char *)((unsigned char *)&(items[i]) + 0x17))) == key) {
                    radio_click(items[i].hwnd);
                    r = 1;
                    found = 1;
                    break;
                }
            }
        }
    }
    return r;
}
