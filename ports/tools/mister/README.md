# MiSTer

The game runs as ARM software on the DE10-Nano and draws through the user's Pinedra GPU core and its OpenGL
driver; we ship neither. Sound goes to `/dev/MrAudio`. MiSTer_MVC2, a Main_MiSTer build, starts the
game in the Pinedra core's place and keeps the OSD and the pads.

- Pads, and the first start's unpack shown on the OSD: `/dev/shm/mvc2-joy` (`agpl/wrapper/mvc2_joy_shm.h`). The game runs as
  `/media/fat/games/mvc2/mvc2-pinedra`, logs in `games/mvc2/logs/`.
- `build-hps.sh` builds MiSTer_MVC2 in Main_MiSTer's own toolchain container (Docker), into `build/mister`
  or `$MISTER_BUILD`.
- `package.sh VERSION OUTDIR` makes the user's package: `mvc2-mister-VERSION.tar.gz` (files below `/media/fat`,
  with a SHA-256 manifest) and `mvc2_setup.sh`, which installs it from the Scripts menu (MISTER.md);
  `MVC2_FAT=DIR` installs into a scratch root for a test.
- `build.sh VERSION OUTDIR` is the release build (CI's): the game in `Dockerfile`'s toolchain, then the two above.
- The front end and the drawing (`FRONT_SOURCES`) can be built from a profile of a run (CMake's `MVC2_PGO`: `use`
  while `port/mister/pgo/` is there, which the tree doesn't keep): about 6 % less of the drawing's time in the
  character select at 800 MHz. To make one, build with `-DMVC2_PGO=generate`, run the game on the MiSTer from the
  title through a character select and a fight (with `MVC2_FRAMES` it ends by itself and writes the profile to
  `/tmp/mvc2pgo`, `MVC2_PGO_DIR`), and copy those files into `port/mister/pgo/` of a build directory of the same
  name. The game's own modules are built without a profile.
