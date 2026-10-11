/* Steam's COLONIZE.ISO into a game folder (isoimport.c) */
#ifndef ISOIMPORT_H
#define ISOIMPORT_H
#include <stddef.h>

/* 0 when dest holds the game's files from the disc image at iso_path; msg says what happened */
int iso_import(const char *iso_path, const char *dest, char *msg, size_t msglen);

#endif
