/* Font::~Font() -- shipped at 1068:dd73.
 *
 * Ported by tools/port.py from matched/gfx/Font_dtor_1068_dd73.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

extern void MSReleaseFont(col_i16 hfont);
class Font {
public:
    col_i16 hfont;
    ~Font();
};

Font::~Font()
{
    if (hfont != 0)
        MSReleaseFont(hfont);
}
