/* MainWin_dtor: candidate for 1008:d2c2 (222 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MainWin::~MainWin()
 *
 * The destructor of MainWin: the eight members destroyed in reverse order, then the EventWin
 * base; nothing else.
 * PediaWin::~PediaWin, EuropeWin::~EuropeWin, ColonyWin::~ColonyWin, ReportWin::~ReportWin, StatWin::~StatWin, MiniWin::~MiniWin, MapWin::~MapWin and EventWin::~EventWin are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "classes.h"
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
