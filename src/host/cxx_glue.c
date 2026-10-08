/* C entry points to C++ members that C reconstructions call by a C name.
 * flags: -P
 *
 * GameWin_ResizePort and GRPort_ResizePort are the two classes' ResizePort -- the one
 * virtual (classes.h) -- which C files call directly, as Borland's corpus could by naming
 * the member's out-of-line body. VList_dtor is VList's destructor in Borland's convention
 * (a flag word: bit 0 frees the object). The port's (src/host, W2), not imported.
 */
#include "colonization.h"
#include "classes.h"

extern "C" {

int GRPort_ResizePort(GRPort *port, RECT *r) { return port->ResizePort_own(r); }
void GameWin_ResizePort(GameWin *w, void *rect) { w->ResizePort_own((RECT *)rect); }

void VList_dtor(VList *p, int flag)
{
    p->VList::~VList();
    if (flag & 1)
        ::operator delete(p);
}

}  /* extern "C" */

/* ResizePort, the game's one virtual function, dispatched on the class tag in GRPort's first
   two bytes, where Borland kept the vtable pointer (gen_headers DEVIRTUALIZE): 1 is GameWin
   and everything derived from it, anything else GRPort. */
int GRPort::ResizePort(RECT *r)
{
    if (vptr == 1)
        return static_cast<GameWin *>(this)->ResizePort_own(r);
    return ResizePort_own(r);
}
