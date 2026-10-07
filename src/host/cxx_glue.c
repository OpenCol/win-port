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

int GRPort_ResizePort(GRPort *port, RECT *r) { return port->GRPort::ResizePort(r); }
void GameWin_ResizePort(GameWin *w, void *rect) { w->GameWin::ResizePort((RECT *)rect); }

void VList_dtor(VList *p, int flag)
{
    p->VList::~VList();
    if (flag & 1)
        ::operator delete(p);
}

}  /* extern "C" */
