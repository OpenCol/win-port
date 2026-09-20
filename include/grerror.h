/* grerror.h -- the code the graphics library leaves behind when a load fails.
 *
 * `error_code_set_1068_949e(n)` writes DGROUP:0x2b1a and `error_code_get_1068_94b2()` reads it
 * back; the pair sit next to `MSGetNamedResource` in the library's resource code. Every call is
 * on a failure path, immediately after the matching `"Error: ..."` string and the cleanup.
 *
 * WHERE THE VALUES COME FROM: the call sites and nothing else. Like tribes.h and gameflag.h, no
 * resource can confirm these, so the names are what the failure paths establish. What
 * check_headers.py can check, and does, is that every name here is a code the corpus actually
 * passes to the setter -- which is a real check because only four of the value space are used,
 * so a fifth name fails.
 *
 * The evidence per code, all from byte-verified reconstructions:
 *   1  MSAllocateHandle, when the handle does not come back
 *   2  GRPort_Init, when `p->surface` is zero and there is nothing to draw on
 *   3  "GIF resource not found", "PCX resource not found" and three more -- five loaders agree
 *   4  "LBM Resource must have FORM" and "LBM Resource is not a ILBM/PBM" -- a resource that
 *      was found and then did not parse
 */
#ifndef GRERROR_H
#define GRERROR_H

#define GRERR_ALLOC_FAILED      1
#define GRERR_NO_SURFACE        2
#define GRERR_NOT_FOUND         3
#define GRERR_BAD_FORMAT        4

#endif
