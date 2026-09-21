/* RGBTree::RGBTree() -- shipped at 1068:1259.
 *
 * Ported by tools/port.py from matched/gfx/RGBTree_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

typedef struct {
    unsigned char flags[3];
    unsigned char pad;
    col_i16 v;
    col_i16 lo;
    col_i16 hi;
} Node;

class RGBTree {
public:
    col_i16 handle;
    Node *nodes;
    col_i16 nil;
    col_i16 count;
    col_i16 base;
    col_i16 stack[(0x606 - 0xc) / 2];
    col_i16 sp;
    col_i16 built;
    col_i16 visited;
    RGBTree();
    ~RGBTree();
};

RGBTree::RGBTree()
{
    built = 0;
    visited = 0;
    sp = 0;
    nodes = 0;
    handle = 0;
    count = 0;
    base = 0;
    nil = 0;
}
