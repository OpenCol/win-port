#!/bin/sh
# The release kit's launcher for Linux and macOS (roadmap W10): ./colonization [GAME_FOLDER]
#
# Runs the port (bin/) under Wine in the folder of your copy of Colonization for Windows: the
# port reads the game's own files there and saves there, as the original did. No game file is
# in the kit. The folder is: the argument; the current folder; this folder or the one above it, if
# the kit was unzipped into the game folder (or Steam's or GOG's Colonization folder); the one used
# last time (~/.config/colonization-port/gamedir); $COLONIZATION_DIR; else one the player picks
# (the system's folder dialog: AppleScript on macOS, zenity or kdialog on Linux; else a question).
#
# In each folder: the installed Windows edition (COLONIZE.EXE and its data DLLs); else a disc image
# with the Windows edition -- Steam's COLONIZE.ISO or GOG's COLONIZE.GOG, the DOS release's disc --
# in it or one folder down, unpacked once into a game folder of the kit's own --
# ~/Library/Application Support/colonization-port/game on macOS,
# ~/.local/share/colonization-port/game on Linux -- by bin/colonization-import.exe (the Windows
# launcher's --import, src/launcher/isoimport.c) under Wine, and that folder is used.
# COLONIZE.EXE must be the 1995 release, patched or the original (its SHA-256 is checked); the
# original lacks the patch's COLDATA9.DLL (the Jukebox's Control Panel, the Cheat menu's dialogs)
# and COCKGUN.WAV (GUNCOCK.WAV renamed, which the unpacking writes).
#
# Wine: $PORT_WINE, else `wine` on PATH, else Gcenx's Wine app in (~)/Applications, else wine64.
# $PORT_DESKTOP sets the game's desktop window size. On macOS use a current Wine (e.g. Gcenx's
# wine-devel 11 from github.com/Gcenx/macOS_Wine_builds): Apple's Game Porting Toolkit Wine 7.7
# stops the game the first time it plays a sound.
set -e
PATCHED=59a0e173452924603112cf410483021c02ef0cd3512e310d8966b21e0c176291
PREPATCH=ae7d9149f056766a534fe8f0006c9512aac1e1004e9776ab3cca818e4833b650
here=$(cd "$(dirname "$0")" && pwd)
conf="${XDG_CONFIG_HOME:-$HOME/.config}/colonization-port"
if [ "$(uname)" = Darwin ]; then
    unpacked="$HOME/Library/Application Support/colonization-port/game"
else
    unpacked="${XDG_DATA_HOME:-$HOME/.local/share}/colonization-port/game"
fi

winpath() { printf 'Z:%s' "$(printf %s "$1" | tr / '\\')"; }   # Wine's name for a Unix path (Z: is /)

# Wine: $PORT_WINE; else `wine` on PATH; else a Gcenx Wine app in /Applications or ~/Applications
# (Homebrew's wine@devel / wine-stable casks put them there); else wine64 -- on a Mac often Apple's
# Game Porting Toolkit, the last resort
wine=""
for w in "$PORT_WINE" wine \
         "/Applications/Wine Devel.app/Contents/Resources/wine/bin/wine" \
         "/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine" \
         "/Applications/Wine Staging.app/Contents/Resources/wine/bin/wine" \
         "$HOME/Applications/Wine Devel.app/Contents/Resources/wine/bin/wine" \
         "$HOME/Applications/Wine Stable.app/Contents/Resources/wine/bin/wine" wine64; do
    if [ -n "$w" ] && command -v "$w" >/dev/null 2>&1; then wine=$w; break; fi
done
if [ -z "$wine" ]; then
    echo "Colonization: Wine is needed (https://www.winehq.org; on macOS Gcenx's wine-devel builds)." >&2
    exit 5
fi
case "$("$wine" --version 2>/dev/null)" in
    *"Game Porting Toolkit"*|wine-7.7*)
        echo "Colonization: warning: $("$wine" --version) stops the game when a sound plays; use a" >&2
        echo "current Wine (PORT_WINE=/path/to/wine to choose one)." >&2 ;;
esac

find_exe() {    # the game's COLONIZE.EXE in $1, whatever its case
    for n in COLONIZE.EXE colonize.exe Colonize.exe; do
        [ -f "$1/$n" ] && { echo "$1/$n"; return 0; }
    done
    return 1
}

has_game() {    # the Windows edition's files the port reads: COLONIZE.EXE, COLTEXT0, COLDATA0..8
    find_exe "$1" >/dev/null || return 1
    for n in COLTEXT0 COLDATA0 COLDATA1 COLDATA2 COLDATA3 COLDATA4 COLDATA5 COLDATA6 COLDATA7 COLDATA8; do
        [ -f "$1/$n.DLL" ] || [ -f "$1/$(printf %s "$n" | tr A-Z a-z).dll" ] || return 1
    done
}

find_image() {  # a disc image with the Windows edition, in $1 or one folder down: Steam's
                # COLONIZE.ISO first, then GOG's COLONIZE.GOG (GOG's COLDIG.BIN is a DOS file)
    for n in COLONIZE.ISO colonize.iso COLONIZE.GOG colonize.gog; do
        for f in "$1/$n" "$1"/*/"$n"; do
            [ -f "$f" ] && { echo "$f"; return 0; }
        done
    done
    return 1
}

resolve() {     # $1 holds the game: the installed Windows edition, or a disc image unpacked once
    [ -n "$1" ] && [ -d "$1" ] || return 1
    if has_game "$1"; then game=$(cd "$1" && pwd); return 0; fi
    img=$(find_image "$1") || return 1
    if has_game "$unpacked"; then
        echo "Colonization: using $unpacked, unpacked from $img before" >&2
        # a folder an older kit unpacked: the patch's COCKGUN.WAV is the disc's GUNCOCK.WAV
        [ -f "$unpacked/COCKGUN.WAV" ] || cp "$unpacked/GUNCOCK.WAV" "$unpacked/COCKGUN.WAV" 2>/dev/null || true
    else
        echo "Colonization: $img: unpacking the Windows edition into $unpacked" >&2
        mkdir -p "$unpacked"
        "$wine" "$here/bin/colonization-import.exe" --import "$(winpath "$img")" "$(winpath "$unpacked")" ||
            { echo "Colonization: $img could not be unpacked (see above)." >&2; exit 6; }
    fi
    game=$unpacked
}

pick_folder() { # the system's folder dialog; a question in the terminal where there is none
    prompt="Where is Colonization? Choose the folder of Colonization for Windows (with COLONIZE.EXE), or of Steam's or GOG's Colonization (with COLONIZE.ISO or COLONIZE.GOG)."
    if [ "$(uname)" = Darwin ]; then
        osascript -e "POSIX path of (choose folder with prompt \"$prompt\")" 2>/dev/null
    elif command -v zenity >/dev/null 2>&1; then
        zenity --file-selection --directory --title="$prompt" 2>/dev/null
    elif command -v kdialog >/dev/null 2>&1; then
        kdialog --getexistingdirectory "$HOME" --title "$prompt" 2>/dev/null
    else
        printf '%s\nFolder (empty to stop): ' "$prompt" >&2
        read -r answer && printf '%s\n' "$answer"
    fi
}

# where the game is, in this order -- in each folder the installed Windows edition first, then
# Steam's COLONIZE.ISO, then GOG's COLONIZE.GOG (resolve): the folder given; the current folder;
# this script's folder and the one above it (the kit unzipped into the game's folder: the zip has
# a top folder of its own); the folder used last time; $COLONIZATION_DIR; then a folder the player
# picks, asked until one holds the game or the player cancels
game=""
for d in "$1" "$PWD" "$here" "$(dirname "$here")" "$(cat "$conf/gamedir" 2>/dev/null)" "$COLONIZATION_DIR"; do
    resolve "$d" && break
done
while [ -z "$game" ]; do
    picked=$(pick_folder) || picked=""
    picked=${picked%/}
    if [ -z "$picked" ]; then
        echo "Colonization: no folder chosen. Run: $0 /path/to/your/Colonization" >&2
        exit 1
    fi
    resolve "$picked" && break
    echo "Colonization: no COLONIZE.EXE with its COLDATA files, COLONIZE.ISO or COLONIZE.GOG in $picked." >&2
done
exe=$(find_exe "$game")
if command -v sha256sum >/dev/null 2>&1; then sum=$(sha256sum "$exe" | cut -d' ' -f1)
else sum=$(shasum -a 256 "$exe" | cut -d' ' -f1); fi
if [ "$sum" = "$PREPATCH" ]; then
    echo "Colonization: $exe is the original 1995 release: without the patch's COLDATA9.DLL, the" >&2
    echo "Jukebox's Control Panel and the Cheat menu's dialogs are missing." >&2
    [ -f "$game/COCKGUN.WAV" ] || [ -f "$game/cockgun.wav" ] ||
        echo "Colonization: COCKGUN.WAV is missing too: it is GUNCOCK.WAV renamed; a copy restores it." >&2
elif [ "$sum" != "$PATCHED" ]; then
    echo "Colonization: $exe is not a build this port knows (SHA-256 $sum)." >&2
    echo "It needs the 1995 release of Colonization for Windows (SHA-256 $PATCHED, or the original's)." >&2
    exit 4
fi
mkdir -p "$conf" && echo "$game" > "$conf/gamedir"

prog=$(ls "$here"/bin/colonize.exe.so "$here"/bin/colonize.exe 2>/dev/null | head -1)
# the port's log (and a crash's report and minidump) beside this script, as Wine names it
PORT_LOG=$(winpath "$here/colonization-port.log")
export PORT_LOG
echo "Colonization: the log is $here/colonization-port.log" >&2
cd "$game"
# through Wine's explorer, in a desktop window of its own ($PORT_DESKTOP, default 1024x768): a
# Winelib program given to `wine` directly goes to start.exe, which cannot open a .so
exec "$wine" explorer "/desktop=Colonization,${PORT_DESKTOP:-1024x768}" "$prog"
