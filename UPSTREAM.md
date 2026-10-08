# UPSTREAM

Requests for col-win-re, which this repository imports from and never edits (roadmap rule 4).
Each entry: what is wrong in the corpus, the evidence (a command and its output, an address,
a lockstep divergence), and what this port does meanwhile.

## 1. Three records whose size the model and the corpus disagree on (W1, 2026-10-07)

`analysis/model.json` (and the `analysis/port-header.h` made from it) gives a size that the
corpus's own definition in `include/records.h` does not have. Measured by compiling both under
MinGW i686 with Borland's widths (`int` 16-bit): of 105 sizes the header asserts, these differ:

| Record | Model | Corpus | Note |
| --- | ---: | ---: | --- |
| `MemBlock` | 2 | 14 | the model seems to size it from its first member |
| `Settlement` | 32 | 18 | the 18-byte record is the settlement the code indexes (`g_settlements`); the community save-file spec agrees |
| `WinGRec` | 8 | 10 | five words in the corpus |

Here: the port takes the corpus's records (they compiled to the shipped bytes), and
`tools/gen_headers.py` leaves those three size checks out (`SIZE_FROM_CORPUS`).

## 2. analysis/port-header.h is older than the model (W1, 2026-10-07)

At 508d4d0d0 the header still names the nation-text record `Nation34` and lacks fields renamed
since (`NationText.control`, `Colony.bonus_hammers`, `Unit.flags2`, ...): 141 files failed on
"has no member" against it. Regenerating it at that commit (`check_header_compiles.py --keep`)
gives a header that compiles fewer files (2,444 against 2,723), because it declares fewer
Windows API functions and globals. Here: `gen_headers.py` takes the corpus's records from
records.h in place of the header's, and maps `Nation34` to `NationText`.

## 3. port_transform.py deletes the statement after a column-0 goto label (W1, 2026-10-07)

`DEFN` (`tools/port_transform.py` at 508d4d0d0) matches across lines, so a label at the start
of a line followed by `if (...) {` reads as the definition of a function named `if`. The
transform then treats it as a rule-47 stub: it writes `extern label: if (...);` and deletes the
statement's body. In `begin_game.c` that removed the palette fill and the window reset after
`out:`. 11 files were damaged: begin_game, end_of_game_check, europe_recruit, foreign_unit,
king_invasion, lost_city_exploration, orders_activate, plan_move, preassign_colonist_to_field,
special_kill and tribe_village. Each failed to compile, so none was damaged silently.

Here: `import/import.py` patches its temporary copy of the transform (`TRANSFORM_FIXES`: a
statement keyword is never a function name), and stops if the patched text is ever not found.

## 4. Model records that name or size members other than the code uses (W1, 2026-10-07)

`analysis/port-header.h` at 508d4d0d0 gives these records members the corpus does not use.
Each failed to compile against the header ("has no member", "subscripted value is neither
array nor pointer"):

| Record | Model | Corpus | Evidence |
| --- | --- | --- | --- |
| `ClassRec` | `w0`, `w2` | `name`, `transport_cost` | `load_data.c` fills them from the CLASS table |
| `FontRes` | `uint16_t obj` | handles at +0x106, indexed | `remove_font_resource.c`: `p->obj[i]` |
| `FontTable` | `uint16_t fonts` | handles at +0x106, appended | `enum_font_family_proc.c`: `tbl->fonts[tbl->count++]` |
| `Table` | `int16_t entries` | entries at +0x106, indexed | `table_entry_handle.c`: `t->entries[n]` |
| `Pattern` | `_tail[9]` | `unsigned char b[9]` | `PediaWin_DoTerrainArticle.c` (local typedef) |

The three handle tables run past the 0x108 bytes the model sizes them at: the model records a
trailing array's first element as a scalar. Here: `tools/gen_headers.py` `MODEL_FIXES` renames
the members and makes the three trailing members `[1]` arrays; the sizes and their checks stay
the model's.

## 5. Classes the corpus defines in every file and never in classes.h (W1, 2026-10-07)

`Popup` (42 of 43 C++ copies identical, 61 of 63 C views), `Flic` (20 of 25 C++ copies; the
rest pad the same offsets) and a C view of `MagicWin` are not in `include/classes.h`; each file
that uses one carries its own copy. `MainWin` and `PediaWin` are worse: C++ files view each
three ways (the class, a byte view, an opaque pad), so they cannot be merged by substitution.
Here: `include/port/corpus_classes.h` holds `Popup`, `Flic` and the `MagicWin` view once, and
the local copies are removed; `MainWin` and `PediaWin` stay per file for now.

## 6. Globals typed as Borland `int *` that point at 16-bit cells (W1, 2026-10-07)

`g_scale_map` is declared `int *` throughout the corpus, but it is set to a `ScaleSlot`'s
`map`, an array of words. On a 32-bit `int` that reads two cells at a time. Here:
`gen_headers.py` `GLOBAL_TYPES` declares it `i16 *`. The same holds for any `int *` the port
widens to: `max_ptr_1050_97d1` now takes `i16 *`, and so do the locals its callers pass.

## 7. In-file stubs and `asm db` that stand for code (W1, 2026-10-07)

`PopupBoxDraw.c` and `SuperWin_Init.c` define empty bodies for six `set_on_*` functions to get
the byte-matched call shape; a link would have two definitions of each. `colony_select_profession.c`
spells two loop exits as `asm db 0x0b,0xc0,0x74,0x58` and `asm db 0x83,0x7e,0xd0,0x00,0x7c,0x48`
(both jump to 1010:d57b, after the `for (;;)`): the C around them does not say they exit.
Here: the stubs are removed and the exits are `break`s, each with a `PORT:` note.

## 8. tools/e2e_turns.py is not committed (W3, 2026-10-08)

The autoplay recorder the W3 oracle needs exists in col-win-re's working tree (last changed
2026-10-03, sha256 9f759c52...) but in no commit, so nothing pinned can use it. Here:
`tests/oracle/e2e_turns.py` is a copy, unchanged but for a header saying so, run from the
col-win-re tree unpacked at SOURCE. Once col-win-re commits it, the copy goes and SOURCE moves.

## 9. Resource-type tags declared as far pointers (W3, 2026-10-08)

`g_color_resource_type`, `g_flic_resource_type`, `g_picture_resource_type` and
`g_restype_text_popup` are declared `void far *`, but they hold four characters ("CTAB",
"FLIC", "PICT"/"cPic", "TEXT": the MS framework's Mac-style resource types), which the code
copies into a local and passes by address (`CTable_Init`). A pointer type is wrong for them
the moment a pointer is not 4 bytes (W9). Here: tests/state.py converts them as 4 bytes
(`NOT_POINTERS`).

## 10. No C++ class for MainWin (W3, 2026-10-08)

`include/classes.h` has a C view of MainWin but no class; `create_game_globals.c` defines the
real one for itself (with MapWin, MiniWin, StatWin, ReportWin, ColonyWin, EuropeWin and
PediaWin members), and C++ files view it opaquely. So the port cannot say how big
`g_main_win` is outside that file, and tests/state.py knows it only by address.

## 11. Class-typed globals declared through a 1-byte view (W3, 2026-10-08)

The header declares `extern int8_t g_colony_window;` for C (and `ColonyWin` for C++), and
likewise `g_europe_win`; storage generated from the C declaration was 1 byte for a
2,606-byte object, which the state round trip caught. Here: tools/gen_globals.py defines
every class-typed global in C++, from the class.

## 12. The emulator's MoveWindow sends no messages (W4, 2026-10-08)

`tools/re_agent/winsim.py` has no `api_MOVEWINDOW`: the call reaches the generic stub, returns 0
and sends nothing. Windows sends WM_WINDOWPOSCHANGING/CHANGED, and from them WM_MOVE and WM_SIZE.
The game reacts: `GameWin::ResizePort` -> `win_set_client_size` -> `MoveWindow`, and its move
handler (`object_message_handler` -> `call_on_move` -> `port_wndproc` -> `GameWin_Resize`)
re-initialises the port's surface. So the oracle's framework trace (tests/trace.py) and the
port's part at call 132, the opening's first picture: the port, under Windows' rules, makes
the extra resize; the oracle cannot. Measured: 132 of the first 10,000 framework calls match,
and the next one is that resize. W4's trace gate needs the oracle to send these messages.
