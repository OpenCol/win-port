/* The NE resource reader (src/host/ne_res.c). */
#ifndef PORT_NE_RES_H
#define PORT_NE_RES_H
#include <stddef.h>
#include <stdint.h>

typedef struct {
    char path[260];
    unsigned char *data;
    size_t size;
    unsigned ne, rsrc;          /* offsets of the NE header and its resource table */
} NeModule;

typedef struct { uint32_t offset, length; const unsigned char *data; } NeResource;

NeModule *ne_open(const char *path);
void ne_close(NeModule *m);
/* type and name: an integer id (MAKEINTRESOURCE) or a name; returns 0 when absent */
int ne_find(const NeModule *m, const char *type, const char *name, NeResource *out);

#endif
