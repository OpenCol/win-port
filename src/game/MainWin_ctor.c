/* MainWin_ctor: candidate for 1008:d1f4 (206 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MainWin::MainWin()
 *
 * The constructor of MainWin (0x3b0c bytes, over EventWin): the menu handle wrapper at
 * +0x8c and the seven child windows -- MapWin +0x8e, MiniWin +0x816, StatWin +0xf9c, ReportWin
 * +0x173c, ColonyWin +0x1ff4, EuropeWin +0x2a08, PediaWin +0x32b2 -- constructed in order by
 * the compiler; the body is empty. The member sizes follow from the offsets.
 * EventWin::EventWin, MapWin::MapWin, MiniWin::MiniWin, StatWin::StatWin, ReportWin::ReportWin, ColonyWin::ColonyWin, EuropeWin::EuropeWin and PediaWin::PediaWin are defined in-file first, for the 4-byte calls.
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

MainWin::MainWin()
{
}
