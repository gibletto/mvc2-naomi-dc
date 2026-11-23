#!/bin/bash
# mvc2_setup.sh - Marvel vs. Capcom 2 on the Pinedra GPU, from MiSTer's Scripts menu (MISTER.md).
#
#     mvc2_setup.sh [PACKAGE]      install or update from PACKAGE, else the newest mvc2-mister-*.tar.gz
#                                  beside this script, in /media/fat or on a USB drive
#     mvc2_setup.sh --uninstall    our files and MiSTer.ini's [MVC2 Pinedra] removed; the game's data,
#                                  saves and settings in games/mvc2 stay
#
# The package holds files below /media/fat, games/mvc2/MANIFEST (each one's SHA-256 and path) among them. Only
# those files are written, and only the files a version before listed are removed. MiSTer.ini gets [MVC2 Pinedra]
# appended, once, after a dated copy of it is kept beside it. The ROMs, the disc, mvc2.cfg, env.txt and the saves
# are never touched. Log: games/mvc2/logs/install.log.
#
# For tests: MVC2_FAT=DIR installs into DIR instead of /media/fat.
FAT=${MVC2_FAT:-/media/fat}
GAME=$FAT/games/mvc2
INI=$FAT/MiSTer.ini
SECTION='MVC2 Pinedra'
STAGE=$FAT/.mvc2_stage
LOG=$GAME/logs/install.log
TESTED=20261115

say() {
	echo "$*"
	[ -d "$GAME/logs" ] && echo "$(date '+%F %T') $*" >> "$LOG"
}
refuse() {
	say "$*"
	say "Nothing was changed."
	rm -rf "$STAGE"
	exit 1
}
fail() {
	say "ERROR: $*"
	say "The installation did not finish. Run it again; the log is $LOG."
	rm -rf "$STAGE"
	exit 1
}

# 0 when one of the paths is there
lists() {
	timeout 10 ls "$1" 2> /dev/null | grep -iqE "$2"
}
has_section() { grep -q "^\[$SECTION\]" "$INI" 2> /dev/null; }

ini_backup() {
	[ -f "$INI" ] || return 0
	b="$INI.mvc2-$(date +%Y%m%d-%H%M%S)"
	cp -p "$INI" "$b" || fail "MiSTer.ini could not be copied"
	say "MiSTer.ini kept as $(basename "$b")."
}

# a manifest's paths: each relative, below FAT, none climbing out
paths() {
	cut -c67- "$1" | grep -v -e '^/' -e '\.\.' -e '^$'
}

pinedra() {
	# its core: config/Pinedra Collection.rbf, before release 20261122 config/Pinedra Config.rbf
	[ -f "$FAT/pinedra/lib/libGL.so.1" ] &&
		{ [ -f "$FAT/pinedra/config/Pinedra Collection.rbf" ] || [ -f "$FAT/pinedra/config/Pinedra Config.rbf" ]; } ||
		refuse "Pinedra GPU is not installed (no $FAT/pinedra/lib/libGL.so.1). Install it first, then run this again."
	# VERSIONS: "release 20261115", "core 0.12.0", "driver 0.4.1" and the rest, a line each; VERSION: the release
	release=$(sed -n 's/^release //p' "$FAT/pinedra/VERSIONS" 2> /dev/null)
	[ -n "$release" ] || release=$(head -n 1 "$FAT/pinedra/VERSION" 2> /dev/null)
	core=$(sed -n 's/^core //p' "$FAT/pinedra/VERSIONS" 2> /dev/null)
	driver=$(sed -n 's/^driver //p' "$FAT/pinedra/VERSIONS" 2> /dev/null)
	say "Pinedra GPU ${release:-of an unknown release} (core ${core:-?}, driver ${driver:-?})."
	if [ -z "$release" ] || [ "$release" -lt "$TESTED" ] 2> /dev/null; then
		say "This version was tested with Pinedra GPU $TESTED; update Pinedra if the game does not start."
	fi
}

find_package() {
	# shellcheck disable=SC2012
	ls -t "$(dirname "$0")"/mvc2-mister-*.tar.gz "$FAT"/mvc2-mister-*.tar.gz /media/usb*/mvc2-mister-*.tar.gz \
		2> /dev/null | head -n 1
}

uninstall() {
	[ -f "$GAME/MANIFEST" ] || refuse "Marvel vs. Capcom 2 is not installed here."
	if has_section; then
		ini_backup
		# the section is ours from its header to the next one
		awk -v s="[$SECTION]" '/^\[/ { h = $0; sub(/[ \t\r]+$/, "", h); skip = h == s } !skip' "$INI" > "$INI.mvc2-new" &&
			cat "$INI.mvc2-new" > "$INI" && rm -f "$INI.mvc2-new" || fail "MiSTer.ini could not be written"
		say "[$SECTION] is out of MiSTer.ini."
	fi
	paths "$GAME/MANIFEST" | while IFS= read -r p; do rm -f "$FAT/$p"; done
	rm -f "$GAME/MANIFEST" "$FAT/config/$SECTION".*
	rmdir "$GAME/bin" "$GAME/licenses/MiSTer_MVC2/source" "$GAME/licenses/MiSTer_MVC2" "$GAME/licenses" 2> /dev/null
	sync
	say "Removed. The game's data, saves and settings are still in $GAME: delete that folder to remove them too."
}

install() {
	pkg=$1
	[ -z "$pkg" ] || [ -f "$pkg" ] || refuse "No package $pkg."
	[ -n "$pkg" ] || pkg=$(find_package)
	[ -n "$pkg" ] || refuse "No mvc2-mister-*.tar.gz found: put it beside this script (in Scripts) and run this again."
	say "Package: $pkg"
	pidof mvc2-pinedra MiSTer_MVC2 > /dev/null && refuse "The game is running: quit it first."
	pinedra

	need=$(($(du -k "$pkg" | cut -f1) * 4 + 1024))
	free=$(df -k "$FAT" | awk 'NR == 2 { print $4 }')
	[ "${free:-0}" -gt "$need" ] || refuse "The card needs $need KB free, and has ${free:-?}."

	rm -rf "$STAGE"
	mkdir -p "$STAGE" && tar -xzf "$pkg" -C "$STAGE" || refuse "The package could not be unpacked."
	[ -f "$STAGE/games/mvc2/MANIFEST" ] || refuse "The package has no games/mvc2/MANIFEST."
	(cd "$STAGE" && sha256sum -c games/mvc2/MANIFEST > /dev/null 2>&1) ||
		refuse "The package is damaged (a file's SHA-256 differs): download it again."
	version=$(cat "$STAGE/games/mvc2/VERSION" 2> /dev/null)

	mkdir -p "$GAME/logs" "$GAME/roms/mvsc2" "$GAME/disc"
	[ -f "$GAME/MANIFEST" ] && say "Updating $(cat "$GAME/VERSION" 2> /dev/null) to $version." || say "Installing $version."

	# each file put in place, the old one removed first (a rename over a file leaks space on exFAT)
	paths "$STAGE/games/mvc2/MANIFEST" > "$STAGE/new.txt"
	[ -f "$GAME/MANIFEST" ] && paths "$GAME/MANIFEST" > "$STAGE/old.txt"
	echo games/mvc2/MANIFEST >> "$STAGE/new.txt"
	while IFS= read -r p; do
		mkdir -p "$(dirname "$FAT/$p")" && rm -f "$FAT/$p" && mv "$STAGE/$p" "$FAT/$p" || fail "$p could not be written"
	done < "$STAGE/new.txt"
	# the files of the version before that this one doesn't have
	[ -f "$STAGE/old.txt" ] && grep -vxF -f "$STAGE/new.txt" "$STAGE/old.txt" | while IFS= read -r p; do
		rm -f "$FAT/$p" && say "Removed $p (not in this version)."
	done
	# the one menu entry from before the arcade's and the Dreamcast's
	old_mgl="$FAT/_Other/MVC2 (Pinedra).mgl"
	grep -qs '>MVC2 Pinedra<' "$old_mgl" && ! grep -q '<file' "$old_mgl" && rm -f "$old_mgl" &&
		say "Removed _Other/MVC2 (Pinedra).mgl (now one entry for each version)."
	chmod +x "$FAT/MiSTer_MVC2" "$GAME/mvc2-pinedra" "$GAME/bin/mvc2-pinedra" "$FAT/Scripts/mvc2_setup.sh" 2> /dev/null
	# a Pinedra from before release 20261122 has its core by its old name: the entries load that one until it is updated
	if [ ! -f "$FAT/pinedra/config/Pinedra Collection.rbf" ] && [ -f "$FAT/pinedra/config/Pinedra Config.rbf" ]; then
		for m in "$FAT/_Other/MVC2 Arcade (Pinedra).mgl" "$FAT/_Other/MVC2 Dreamcast (Pinedra).mgl"; do
			[ -f "$m" ] && sed -i 's|pinedra/config/Pinedra Collection<|pinedra/config/Pinedra Config<|' "$m"
		done
		say "The menu entries load Pinedra's core by its old name (Pinedra Config): after updating Pinedra, run this again."
	fi

	if has_section; then
		say "MiSTer.ini has [$SECTION] already."
	else
		ini_backup
		# after a blank line
		[ -f "$INI" ] && [ -n "$(tail -c 1 "$INI")" ] && echo >> "$INI"
		[ -f "$INI" ] && [ -n "$(tail -n 1 "$INI" | tr -d '\r \t')" ] && echo >> "$INI"
		printf '[%s]\nmain=MiSTer_MVC2\n' "$SECTION" >> "$INI" || fail "MiSTer.ini could not be written"
		say "[$SECTION] added to MiSTer.ini."
	fi
	rm -rf "$STAGE"
	sync

	echo
	say "Done: $version. In the menu: _Other, then \"MVC2 Arcade (Pinedra)\" or \"MVC2 Dreamcast (Pinedra)\"."
	# where the game's launcher will find them: a USB drive or a network share, then the SD card
	local m d found_rom="" found_disc=""
	for m in /media/usb0 /media/usb1 /media/usb2 /media/usb3 /media/usb4 /media/usb5 /media/network "$FAT/cifs" "$FAT"; do
		d=$m/games/mvc2
		if [ -z "$found_rom" ] && { lists "$d/roms/mvsc2" '^(mvsc2\.(bin|zip)|epr-23085a\.ic11)$' || lists "$d/roms" '^mvsc2\.zip$'; }; then
			found_rom=$d/roms/mvsc2
		fi
		if [ -z "$found_disc" ] && lists "$d/disc" '^1ST_READ\.BIN$|\.(chd|gdi|cue|iso)$'; then
			found_disc=$d/disc
		fi
	done
	if [ -n "$found_rom" ]; then
		say "Arcade: the ROM set is in $found_rom."
	else
		say "Arcade: put MAME's mvsc2.zip (or its files) in games/mvc2/roms/mvsc2, on the SD card, a USB drive or a network share."
	fi
	if [ -n "$found_disc" ]; then
		say "Dreamcast: the disc is in $found_disc."
	else
		say "Dreamcast: put the USA disc (an image, or its files) in games/mvc2/disc, on the SD card, a USB drive or a network share."
	fi
}

main() {
	echo "Marvel vs. Capcom 2 for Pinedra GPU"
	echo
	if [ "${1:-}" = --uninstall ]; then
		uninstall
	else
		install "${1:-}"
	fi
}

# read whole before it runs: an update replaces this file
main "$@"; exit $?
