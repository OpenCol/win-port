/* make_fake_type: candidate for 1020:8c3d (106 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Initialises the fourteen-byte record at SEG20:0x7c0a: the leading word cleared, one byte from
 * each argument (the second stored twice), three more bytes cleared and the
 * byte at +9 set to 1.  The stores are not in offset order, so the
 * assignments are written in the order the shipped code makes them.
 *
 * IT IS g_unit_types[23]. 0x7c0a = 0x7ac8 + 14*23, and the fourteen bytes end exactly where
 * g_leader_traits begins. This header used to say nothing reads it; everything that looks up the
 * throwaway unit's type reads it, through g_unit_types[g_units[u].type]. The fields are the
 * loader's: icon, defence and attack from the two arguments, name, holds, moves and guns cleared,
 * hammers 1. (Corrected 2026-09-26.)
 *
 * It is NOT immediately before the colony array, as this header used to say. 0x7c28 - 0x7c0a
 * is 30 bytes, and the fourteen here are followed directly by `g_leader_traits`
 * (0x7c18, four nations of four bytes) before `g_colonies` begins.
 *
 * RENAMED 2026-09-26 from init_rec_7c0a: its record at SEG20:0x7c0a is g_unit_types[23] -- 0x7ac8 + 14*23 --
 * the scratch UNIT_TEMPORARY entry, refilled with the icon and strength (defence = attack) of the
 * throwaway unit make_fake_unit is about to make. It is read wherever that unit's type is
 * looked up, which is why no reader names the address.
 *
 * RENAMED 2026-09-26 from init_temporary_unit_type: the Mac build's make_fake_type(short, short), bound by call-sequence alignment (tools/mac_callgraph.py's seq): in make_fake_unit -- bound callers on both sides -- the calls on either side of this one align, and this is the one unbound callee left in the gap on each side (1.0 votes, the next candidate 0.0); the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"
#include "records.h"

void make_fake_type(int icon, int strength)
{
    g_unit_types[UNIT_TEMPORARY].name = 0;
    g_unit_types[UNIT_TEMPORARY].icon = icon;
    g_unit_types[UNIT_TEMPORARY].defence = strength;
    g_unit_types[UNIT_TEMPORARY].attack = strength;
    g_unit_types[UNIT_TEMPORARY].holds = 0;
    g_unit_types[UNIT_TEMPORARY].moves = 0;
    g_unit_types[UNIT_TEMPORARY].guns = 0;
    g_unit_types[UNIT_TEMPORARY].hammers = 1;
}
