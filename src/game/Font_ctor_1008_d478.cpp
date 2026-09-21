/* Font::Font() -- shipped at 1008:d478.
 *
 * Ported by tools/port.py from matched/game/Font_ctor_1008_d478.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class Font {
public:
    col_i16 hfont;
    col_i16 f2;
    Font();
    ~Font();
};

Font::Font()
{
    hfont = 0;
}
