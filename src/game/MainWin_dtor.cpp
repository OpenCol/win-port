/* MainWin::~MainWin() -- shipped at 1008:d2c2.
 *
 * Ported by tools/port.py from matched/game/MainWin_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class EventWin { public: char body[0x8c]; EventWin(); ~EventWin(); };
class MenuClass { public: char body[2]; MenuClass(); ~MenuClass(); };
class MapWin { public: char body[0x816 - 0x8e]; MapWin(); ~MapWin(); };
class MiniWin { public: char body[0xf9c - 0x816]; MiniWin(); ~MiniWin(); };
class StatWin { public: char body[0x173c - 0xf9c]; StatWin(); ~StatWin(); };
class ReportWin { public: char body[0x1ff4 - 0x173c]; ReportWin(); ~ReportWin(); };
class ColonyWin { public: char body[0x2a08 - 0x1ff4]; ColonyWin(); ~ColonyWin(); };
class EuropeWin { public: char body[0x32b2 - 0x2a08]; EuropeWin(); ~EuropeWin(); };
class PediaWin { public: char body[0x3b0c - 0x32b2]; PediaWin(); ~PediaWin(); };
class MainWin : public EventWin {
public:
    MenuClass menu;
    MapWin map;
    MiniWin mini;
    StatWin stat;
    ReportWin report;
    ColonyWin colony;
    EuropeWin europe;
    PediaWin pedia;
    MainWin();
    ~MainWin();
};

MainWin::~MainWin()
{
}
