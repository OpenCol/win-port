/* file_exists -- shipped at 1040:5f55.
 *
 * Ported by tools/port.py from matched/game/file_exists.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * open_data_file(char *name, char *mode);

extern col_i16 FCLOSE(void *f);



col_i16 file_exists(char *name)
{
    register col_i16 r = 0;
    void *f = 0;

    f = open_data_file(name, "rb");
    if (f != 0) {
        FCLOSE(f);
        r = 1;
    }
    return r;
}
