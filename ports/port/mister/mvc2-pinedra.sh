#!/bin/sh
# /media/fat/games/mvc2/mvc2-pinedra [--arcade | --dc]: the game on the Pinedra GPU, as the MVC2 wrapper starts it
# on the core loaded as "MVC2 Pinedra"; MVC2_* settings from env.txt beside it, NAME=value a line
cd "$(dirname "$0")" || exit 1
# Pinedra Collection's "no start animation", as its own games take it
grep -qx 'splash=0' /media/fat/pinedra/settings/all.cfg 2>/dev/null && export PND_SPLASH=0
# no multisampling, as the board drew (env.txt can set PND_MSAA=1)
export PND_MSAA=0
# the driver one frame ahead, not its default: a frame shown a vblank or two sooner, as fast
export PND_FRAMES_AHEAD=1
# the sound's thread (SDLAudio) kept on CPU1, off the game's CPU0: a fifth to a tenth of the frames over 18 ms
export PND_AUDIO_CORE=1
# the ARM at 1000 MHz while the game runs, which the Pinedra driver sets and puts back when it ends: a board that
# doesn't take it, PND_CPU_MHZ=800 in env.txt
export PND_CPU_MHZ=1000
# Pinedra Collection's "Enable 240p mode": drawn in 240 lines, which a 15 kHz CRT shows without interlace
grep -qx 'crt_240p=1' /media/fat/pinedra/settings/all.cfg 2>/dev/null && export PND_240P=1
if [ -f env.txt ]; then
    set -a
    . ./env.txt
    set +a
fi
# the ROMs and the disc from a USB drive's or a network share's games/mvc2 before the SD card's, as MiSTer's cores look
# for their games, unless env.txt (MVC2_ROM, MVC2_DC_DISC) or mvc2.cfg (arcade=, dc=) says where they are
has() { timeout 10 ls "$1" 2> /dev/null | grep -iqE "$2"; }
in_cfg() { grep -Eq "^[[:space:]]*$1[[:space:]]*=" mvc2.cfg 2> /dev/null; }
for m in /media/usb0 /media/usb1 /media/usb2 /media/usb3 /media/usb4 /media/usb5 /media/network /media/fat/cifs; do
    d=$m/games/mvc2
    if [ -z "$MVC2_ROM" ] && ! in_cfg arcade &&
        { has "$d/roms/mvsc2" '^(mvsc2\.(bin|zip)|epr-23085a\.ic11)$' || has "$d/roms" '^mvsc2\.zip$'; }; then
        export MVC2_ROM="$d/roms/mvsc2"
    fi
    if [ -z "$MVC2_DC_DISC" ] && ! in_cfg dc && has "$d/disc" '^1ST_READ\.BIN$|\.(chd|gdi|cue|iso)$'; then
        export MVC2_DC_DISC="$d/disc"
    fi
done
[ "$PND_240P" = 1 ] || unset PND_240P
exec bin/mvc2-pinedra "$@"
