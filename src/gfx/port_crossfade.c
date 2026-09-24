/* port_crossfade -- shipped at 1068:b351.
 *
 * Ported by tools/port.py from matched/gfx/port_crossfade.c.
 *
 * The file's own CTable, Port, Screen dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
CTable * port_bits(Port *p);
void MSDoTask_1068_d906(void);
void MSWaitTicks_1068_d93a(col_i16 n);
void port_copy_full(Screen *s);

extern void CTable_set_key_and_reinstall(CTable *t, col_i16 first, col_i16 count,
                                             col_i16 steps, char r, char g, char b);
extern void palette_fade(CTable *t, col_i16 step);
extern void ctable_install_and_free(CTable *t);

/* same object: 4-byte `push cs; call ` */





void port_crossfade(Screen *s, col_i16 first, col_i16 count,
                                  register col_i16 steps)
{
    CTable *tbl;
    register col_i16 i;

    i = steps;
    tbl = port_bits(&(*((Port *)((unsigned char *)(s) + 0x26))));
    CTable_set_key_and_reinstall(tbl, first, count, steps, 0, 0, 0);
    while (i > 0) {
        palette_fade(tbl, --i);
        MSDoTask_1068_d906();
        MSWaitTicks_1068_d93a(1);
    }
    port_copy_full(s);
    while (i < steps) {
        palette_fade(tbl, ++i);
        MSWaitTicks_1068_d93a(1);
    }
    ctable_install_and_free(tbl);
}
