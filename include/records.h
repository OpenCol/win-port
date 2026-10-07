/* records.h -- the game's records, one definition each, for every reconstruction.
 *
 * Written by tools/shared_records.py from the corpus: each record here had ONE body in
 * every matched/ file that declared it (tools/unify_typedef.py), and is never a C++ class.
 * The window and dialog classes are not here: their C files use C views and their C++
 * files classes, and the two compile differently where a cast crosses a base class.
 * The files no longer declare these records, so this is now their only definition: a
 * member is renamed HERE and at its uses (the bytes do not see a member name), and
 * `shared_records.py --check` keeps any file from declaring one again.
 */
#ifndef RECORDS_H
#define RECORDS_H

#include "w16types.h"

typedef struct { short time; short count; short kind; } AnimEvent;
/* Audio
 * as audio_open_element names it
 */
typedef struct { char pad[0x8c]; void far *mci; void (far *on_mci_done)(void); } Audio;
typedef struct { char pad[0xc4]; int shown_half; } BackdropWin;
typedef struct { unsigned w, h; int x, y; unsigned char nplanes; signed char masking; unsigned char compression; char pad; int transparent; char xasp, yasp; int pagew, pageh; } Bmhd;
/* BuildNeed
 * 4 bytes at SEG20:0xdca8
 */
typedef struct { unsigned char building, expert_job, goods, f3; } BuildNeed;
/* BuildingRec
 * 12 bytes
 * 12 bytes at SEG20:0xd422
 */
typedef struct { int name; signed char prereq2; signed char prev_level; signed char next_level; signed char kind; signed char family; signed char tools; signed char min_pop; signed char upkeep; int hammers; } BuildingRec;
typedef struct { int cmd; int x; } ButtonSlot;
/* Colony
 * 0xca bytes
 * 0xca
 * 0xca bytes at SEG20:0x7c28
 * SEG20:0x7c28
 * Colony+0xc6 -- the accumulated bells the SoL percentage divides into: `colony_rebel` computes
 * `bells * 100 / this`, and `new_turn_colony` decays it by a 64th each turn and adds twice the
 * population. Five files had called it bells_cap or bells_max after that denominator role
 * Colony+0xc2 is the bells ACCUMULATED and +0xc6 the figure they are measured against:
 * `colony_rebel` holds both and computes `bells * 100 / bells_cap`, and `new_turn_colony`
 * decays +0xc6 by a 64th a turn and adds twice the population. Twenty files had called +0xc6
 * `bells`, which is the OTHER field
 * 0xca at SEG20:0x7c28
 * Colony+0x98 (`bonus_hammers`) is hammers the colony got without building them: the rush
 * purchase in `ColonyWin_colony_hammer_buy` adds what it bought, and `perform_orders_road`
 * adds 10 when a road is finished near the colony. Nothing reads it back. The community save
 * format (pavelbel/smcol_saves_utility) calls it hammers_purchased, "possibly unused".
 */
typedef struct { unsigned char x, y; char name[24]; unsigned char nation; unsigned char ai_flags; unsigned char status; unsigned char ai_plan_flags; unsigned char want_defenders_threat; signed char pop; unsigned char jobs[32]; unsigned char spec[32]; unsigned char teach_progress[16]; signed char slots[20]; signed char flags[6]; signed char flags2[2]; signed char ai_idle_turns; signed char wanted_goods; signed char want_defenders; signed char turns_since_ship; int changed; int hammers; signed char item; unsigned char warehouses; unsigned char capitols; unsigned char mine; int bonus_hammers; int stock[16]; unsigned char hist_pop[4]; unsigned char hist_bld[4]; long bells; long bells_cap; } Colony;
typedef struct { int start, end; } CreditCue;
/* CtrlBlock
 * 0x1e bytes
 */
typedef struct { unsigned handle; void far *ctl; short shared_bitmaps; unsigned bmp0, bmp1, bmp2; short hot, pressed, f12; void far *data; short f18, f1a, kind; } CtrlBlock;
/* FameEntry
 * 0x2a bytes
 */
typedef struct { char name[0x18]; int nation, indep, granted, year, season, difficulty, raw_score, rating, rank; } FameEntry;
/* Father
 * 6 bytes at SEG20:0xd6fa
 * 6 bytes
 * 6 bytes at SEG20:0xd6fa, the same struct choose_freedom declares; see fathers.h
 */
typedef struct { int name; unsigned char cat; unsigned char weight[3]; } Father;
typedef struct { long size; short type; short chunks; char reserved[8]; } FlicFrame;
typedef struct { char pad[0x100]; int count; } FontFile;
typedef struct { unsigned hfont; short owned; } FontRec;
typedef struct { int f0; int step; int phase; int awaiting_human; int nation; int unit_held; } GameBrains;
/* GameState
 * 0x8e bytes: the save file's block at offset 0x10
 */
typedef struct { int tutorial_flags, game_flags, colony_options, hint_flags, america_map, year, season, turn_counter, view_mode, selected_unit, turn_nation, view_nation, player_nation, settlement_count, unit_count, colony_count, trade_route_count, reveal_all, view_override; unsigned char difficulty; signed char year_hi, year_lo; signed char father_owner[25]; int game_in_progress, human_turn_active, end_of_turn_pending; int war_started[4]; int rebel_sentiment, mother_country_idx, intervention_nation, merc_nation, rebel_step; int royal_forces[4]; int force[4]; int base_sales[16]; unsigned char woodcuts_seen[4]; } GameState;
typedef struct { signed char start_lo, start_hi, price_min, price_max, spread, up, down, drift; unsigned char shift; char f9; } GoodsRec;
typedef struct { unsigned kind[9]; int count[9]; int part[9]; } GoodsRow;
/* GrafRec
 * the WinG surface record MSInitGraf builds
 * 0x12 bytes
 */
typedef struct { unsigned handle; unsigned hdc; short hobj; unsigned hbm; unsigned old; int topdown; int w; int h; int stride; } GrafRec;
typedef struct { unsigned handle; } Handle;
typedef struct { int left, top, w, h; unsigned char flags; } ImgDesc;
typedef struct { unsigned char a, b, c, pad; } LeaderFace;
/* LevelRec
 * 6 bytes
 * 6 bytes at SEG20:0xcb7c, the LEVELS resource: "Semi-Nomadic, Camp, Camps"
 */
typedef struct { int name, settlement, settlements; } LevelRec;
/* ListItem
 * 0x92 bytes
 */
typedef struct { int hwnd; char rest[0x90]; } ListItem;
typedef struct { char pad[4]; long far *head; long far *cur; } ListIter;
typedef struct { char pad[0x26]; int count; char pad2[6]; ListItem far *items; } ListWin;
typedef struct { char pad0[2]; int id; char pad[0x1c - 4]; void (far *handler)(int id); } ListboxHandlers;
/* MSFile
 * OFSTRUCT at +4
 */
typedef struct { short writable; short handle; char ofs[0x88]; } MSFile;
/* MSWin
 * the HWND is at +2
 * 0x26 bytes
 */
typedef struct { unsigned handle; unsigned hwnd; unsigned hdc; int focus; unsigned hbrush; unsigned hpal; unsigned hcursor; void far *pattern; long f12; void far *icon; int drag_by_client; int size_by_edges; int drag_height; int edge_width; int destroyed; unsigned flags; } MSWin;
typedef struct { int left, top, right, bottom; } MapRect;
/* MapSize
 * the map's width and height
 */
typedef struct { int w, h; } MapSize;
typedef struct { char pad0[4]; unsigned devid; char pad6[8 - 6]; unsigned long parm; } Mci;
typedef struct { char pad[4]; unsigned id; } MciDev;
typedef struct { unsigned char kind; char owns_handle; short handle; void far *ptr; unsigned short used; unsigned short size; unsigned short avail; } MemBlock;
typedef struct { unsigned hwnd; RECT rect; int index; int style; char text[0x8e - 0xe]; char accel; char pad8f; int mark; } MenuItem;
typedef struct { char active; char pad; short frame; short count; } MonsterSlot;
/* Nation
 * 0x13c bytes
 * 316 bytes at SEG20:0xc2f4
 * SEG20:0xc2f4
 * 0x13c bytes; +0x19 the rebel support
 * 316 bytes at SEG20:0xc2f4; this file needs the SIZE and no member
 */
typedef struct { unsigned char flags; signed char tax; unsigned char recruit[3]; unsigned char grants_taken; unsigned char recruit_count; unsigned char father_bits[4]; char pad_0b[1]; int bells; int bells_total; int last_tax_delta; int next_father; unsigned char fathers; char pad_15[1]; int f16; unsigned char villages_burned; unsigned char support; unsigned char shown; unsigned char f1b[2]; char pad_1d[1]; int artillery; int boycott; long royal_gold; long net_take; long gold; int crosses; int need; unsigned char start_x; unsigned char start_y; unsigned char rel[4]; char pad_38[8]; unsigned char timer[4]; unsigned char seed; unsigned char seed_add; int founded; unsigned char artillery_credit; unsigned char muskets; unsigned horses; signed char price[16]; int drift[16]; long trade_gold[16]; long trade_tons[16]; long sold[16]; } Nation;
/* NationText
 * 0x34 bytes: a European nation's leader, country, and who plays it
 */
typedef struct { char leader[24]; char country[24]; unsigned char once_flags; signed char control; short colony_names_used; } NationText;
/* Palette
 * hpal at +0x404
 */
typedef struct { short version; short count; struct { unsigned char r, g, b, reserved; } e[256]; unsigned hpal; } Palette;
/* PediaEntry
 * the list node PediaWin_AddString builds
 */
typedef struct PediaEntry { short name, category, index; } PediaEntry;
typedef struct { int colony; int value; signed char loads; unsigned char troops; } Pickup;
/* PlacementRec
 * 12 bytes
 */
typedef struct { short id; RECT rect; short state; } PlacementRec;
typedef struct { signed char x, y, kind, priority; } Plan;
typedef struct { int x, y; } Point;
/* PopupEdit
 * 0x14 bytes
 * the edit item Popup_edit builds
 */
typedef struct PopupEdit { int f0; int width; int maxlen; int index; char far *label; char far *buf; struct PopupEdit far *next; } PopupEdit;
/* PopupItem
 * 0x12 bytes
 */
typedef struct PopupItem { short flags; short id; short index; char far *text; struct PopupItem far *next; struct PopupItem far *prev; } PopupItem;
typedef struct PopupLine { int flags; char far *text; struct PopupLine far *next; } PopupLine;
typedef struct { short f0; short width; short height; short stride; char pad[0x18 - 8]; unsigned char huge *bits; } Port;
typedef struct { int x, y; } Pos;
typedef struct { unsigned char flags[3]; unsigned char pad; short v; short lo; short hi; } RGBTreeNode;
/* RadioItem
 * 0x92 bytes: key is the mnemonic, accel its position in the label (-1: none)
 */
typedef struct { unsigned hwnd; RECT rc; int index; int enabled; char label[0x80]; char key; char pad_8f; int accel; } RadioItem;
typedef struct { short colony, amount, count; } Rec6;
typedef struct { char f0; unsigned char type; char tax_cut, f3; int price; } Recruit;
typedef struct { int f0; int handle; void far *data; } Resource;
typedef struct { int colony; char rest[8]; } Stop;
/* RouteRec
 * 0x4a bytes at SEG20:0xc804
 * 74 bytes at SEG20:0xc804
 */
typedef struct { char name[32]; unsigned char sea; unsigned char count; Stop stops[4]; } RouteRec;
/* ScaleSlot
 * 0x808 bytes
 */
typedef struct { int num, den; long stamp; int map[0x400]; } ScaleSlot;
/* Settlement
 * 18 bytes at SEG20:0xcb9a
 * 0x12 bytes at SEG20:0xcb9c
 * 18 bytes
 * 0x12 bytes, the settlement table from its first byte
 * 0x12 bytes, one entry of the settlement table; DGROUP:0x4c2c points at the current one -- NOT
 * the Tribe above, which this file had been using for both pointers
 * SEG20:0xcb9a
 * 0x12
 * 0x12 bytes
 */
typedef struct { unsigned char x; unsigned char y; unsigned char owner; unsigned char flags; unsigned char size; signed char mission; signed char growth; signed char refused; signed char sold; signed char bought; int anger[4]; } Settlement;
/* SpecRec
 * 8 bytes, see jobs.h
 * 8 bytes at SEG20:0xd61a
 * 8 bytes
 * 8 bytes at SEG20:0xd61a, see jobs.h
 * see jobs.h
 * 8 bytes to the row at SEG20:0xd61a
 */
typedef struct { int name; int expert; int school; int price; } SpecRec;
/* TerrainRec
 * 16 bytes
 * 0x10 bytes
 * 0x10
 */
typedef struct { unsigned short name; unsigned char movement; unsigned char defence; unsigned char work_turns; unsigned char value; char best_goods_value; unsigned char yield[5]; unsigned char lumber; char pad_0d[3]; } TerrainRec;
typedef void (far *TimerFn)(int slot, int count);
/* Tribe
 * 0x4e bytes at SEG20:0xd182
 * 0x4e bytes: g_tribes[nation - 4] indexes it
 * 0x4e bytes
 */
typedef struct { unsigned char x, y; unsigned char level; unsigned char flags; unsigned char f4; unsigned char count; unsigned char f6; signed char muskets; signed char horse_herds; char f9; int horses; int silver_base; int goods_drift[16]; int mood[4]; signed char calm[4]; unsigned char b3a[12]; int anger[4]; } Tribe;
/* TribeName
 * 6 bytes
 * 6 bytes at SEG20:0xd3da, four records before the tribes' own start at 0xd3f2, so indexing it
 * by the nation number lands on tribe n's record. THIS READS +0, THE PLURAL NOUN. The TRIBES
 * resource is "Incas, Inca, Jewelled Relics" and `load_data` stores the columns in order, so +0
 * is "Incas" and +2 "Inca" -- which is what `get_true_adjective` returns. Both functions fall
 * back to the European adjective table for nations under 4, so the asymmetry is the game's: a
 * tribe contributes its plural noun where a European contributes an adjective.
 * 6 bytes at SEG20:0xd3f2; the TRIBES resource is "Incas, Inca, Jewelled Relics", so +2 is the
 * ADJECTIVE -- which is why the European branch falls back to the adjective table
 * 6 bytes at SEG20:0xd3f2, the TRIBES resource: "Incas, Inca, Jewelled Relics"
 */
typedef struct { int name, adjective, treasure; } TribeName;
/* Unit
 * 0x1c bytes, DGROUP-far array at SEG20:0xa208
 * 0x1c bytes at SEG20:0xa208
 * SEG20:0xa208
 * 28 bytes at SEG20:0xa208
 * 0x1c bytes
 * 0x1c
 * 0x1c at SEG20:0xa208
 */
typedef struct { unsigned char x, y, type, flags, flags2, moves_used; signed char home; signed char mission; unsigned char orders, dest_x, dest_y; signed char last_dir; unsigned char cargo_count; unsigned char cargo[3]; union { unsigned char amount[6]; struct { unsigned char unused, work_turns; signed char work_dir; unsigned char unused13, trade_loaded, tools; } w; struct { unsigned char unused10[2]; int turn_stamp; } n; } hold; unsigned char counter; char spec; short link, link2; } Unit;
/* UnitType
 * 14 bytes at SEG20:0x7ac8
 * 14 bytes
 * 14 bytes; load_data
 */
typedef struct { int name; unsigned char icon, b3, moves, defence, attack, holds, size, hammers, tools, guns, hull, bits; } UnitType;
typedef struct { unsigned handle; unsigned hdc; unsigned hpal; unsigned hbitmap; unsigned oldbm; } WinGRec;
/* Wing
 * +0xe, +0x10
 */
typedef struct { char pad0[0xe]; short height; short rowbytes; } Wing;

#endif
