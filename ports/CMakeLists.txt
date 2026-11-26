# Marvel vs. Capcom 2 (NAOMI and Dreamcast) on PC, 3DS and PSP, from the decompilation (MVC2_DECOMP: by default
# the folder above).
#   cmake -G Ninja -B build -DCMAKE_C_COMPILER=gcc && ninja -C build    (MSYS2 mingw32; -DMVC2_SDL3=OFF: SDL2)
#   sh tools/linux/build.sh VERSION OUTDIR                              (32-bit x86 Linux, in Docker)
#   psp-cmake -G Ninja -B build-psp && ninja -C build-psp               (pspdev: the Dreamcast's EBOOT.PBP)
cmake_minimum_required(VERSION 3.21)
project(mvc2_ports C)
if(WIN32)
    enable_language(RC)
    set(MVC2_VERSION "dev" CACHE STRING "the version in the executables' Windows version information")
endif()

set(PORT ${CMAKE_SOURCE_DIR}/port)
get_filename_component(DEFAULT_DECOMP ${CMAKE_SOURCE_DIR}/.. ABSOLUTE)
set(MVC2_DECOMP ${DEFAULT_DECOMP} CACHE PATH "the decompilation")
set(GAME ${MVC2_DECOMP})
if(EXISTS ${GAME}/pointers.tsv)
    set(POINTERS ${GAME})
else()
    set(POINTERS ${GAME}/build)
endif()
option(MVC2_SDL3 "the PC front end on SDL3, with the GPU renderer (OFF: SDL2)" ON)
# the MiSTer (DE10-Nano, 32-bit ARM Linux): no SDL, its own front end (port/mister), drawn on the Pinedra GPU core by
# its OpenGL (rd_gl1), its libGL.so.1 loaded at run time, so linked dynamically
option(MVC2_MISTER "the MiSTer front end" OFF)
if(MVC2_MISTER)
    set(MVC2_SDL3 OFF)
    add_compile_definitions(MVC2_MISTER)
    # a fatal signal's stack written by backtrace(), which needs the unwind tables on ARM
    add_compile_options(-funwind-tables)
endif()
# Debian bullseye's gcc 10.2 made a loop into a memmove to the wrong place under -fwrapv: no loops made calls
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 11)
    add_compile_options(-fno-tree-loop-distribute-patterns)
endif()

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "the game stores addresses in 32 bits: build for a 32-bit target (MSYS2 mingw32, or 32-bit x86 or ARM Linux)")
endif()
set(LINUX_X86 OFF)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i.86|AMD64")
    set(LINUX_X86 ON)
endif()
find_package(Python3 COMPONENTS Interpreter REQUIRED)
# where the host maps the game's 32 MB of main RAM: the SH-4's address, unless a host without an MMU needs another
# (the PSP: its RAM ends at 0x0C000000, and only the Dreamcast's 16 MB fit)
if(PSP)
    set(RAM_DEFAULTS 0x0A000000 0x01000000)
else()
    set(RAM_DEFAULTS 0x0C000000 0x02000000)
endif()
list(GET RAM_DEFAULTS 0 RAM_BASE_DEFAULT)
list(GET RAM_DEFAULTS 1 RAM_SIZE_DEFAULT)
set(MVC2_RAM_BASE ${RAM_BASE_DEFAULT} CACHE STRING "the host address of the game's main RAM (a multiple of 64 KB)")
# 32 MB, the board's; 16 MB (0x01000000), the Dreamcast's, is enough for a build that runs only its program
set(MVC2_RAM_SIZE ${RAM_SIZE_DEFAULT} CACHE STRING "the main RAM mapped: 0x02000000, or 0x01000000 for the Dreamcast alone")
if(NOT MVC2_RAM_SIZE MATCHES "^0x0[12]000000$")
    message(FATAL_ERROR "MVC2_RAM_SIZE ${MVC2_RAM_SIZE}: 0x02000000 or 0x01000000")
endif()
math(EXPR RAM_BASE "${MVC2_RAM_BASE}")
math(EXPR RAM_BASE_LOW "${RAM_BASE} % 65536")
math(EXPR RAM_END "${RAM_BASE} + ${MVC2_RAM_SIZE}")
# moved, it may not overlap the SH-4's 0x0C000000 to 0x0E000000: the game tells a pointer not yet moved by its value
if(NOT RAM_BASE_LOW EQUAL 0 OR RAM_BASE LESS 65536 OR RAM_BASE GREATER 2113929216 OR
   (NOT RAM_BASE EQUAL 201326592 AND RAM_END GREATER 201326592 AND RAM_BASE LESS 234881024))
    message(FATAL_ERROR "MVC2_RAM_BASE ${MVC2_RAM_BASE}: a multiple of 64 KB, the RAM below 0x80000000, and "
                        "0x0C000000 or clear of 0x0C000000 to 0x0E000000")
endif()
add_compile_definitions(MVC2_RAM_BASE=${MVC2_RAM_BASE} MVC2_RAM_SIZE=${MVC2_RAM_SIZE})
# no fused multiply-add: the same results on each CPU
add_compile_options(-ffp-contract=off)
# a fixed load address, so MVC2_RAM_BASE stays free for the game's RAM
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-fno-pie)
    add_link_options(-no-pie)
endif()
# on x86 at Windows' address, its heap far below the RAM; 64-bit file offsets and inode numbers (a stat on
# btrfs, or a disc image past 2 GB)
if(LINUX_X86)
    add_link_options(-Wl,-Ttext-segment=0x400000)
    add_compile_definitions(_FILE_OFFSET_BITS=64)
endif()
# on the PSP no data through $gp (the game's sits at its link's addresses, past $gp's reach), and each division
# checked for 0 by an assembler that gives 0 as SHC's code did (port/psp/divzero_as.py), which gcc runs (-B)
if(PSP)
    execute_process(COMMAND ${CMAKE_C_COMPILER} -print-prog-name=as OUTPUT_VARIABLE PSP_AS
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    file(WRITE ${CMAKE_BINARY_DIR}/psp_as/as
         "#!/bin/sh\nexec \"${Python3_EXECUTABLE}\" \"${CMAKE_SOURCE_DIR}/port/psp/divzero_as.py\" \"${PSP_AS}\" \"$@\"\n")
    file(CHMOD ${CMAKE_BINARY_DIR}/psp_as/as PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE
         WORLD_READ WORLD_EXECUTE)
    # each routine in its own section, so the link can put the busiest together (port/psp/hot_text.ld)
    add_compile_options(-G0 -B${CMAKE_BINARY_DIR}/psp_as/ -ffunction-sections)
    add_compile_definitions(AICA_PERF)     # the sound's costs in the log (psp_audio.c)
endif()

# the game's C as SHC took it
# -O2 on the MiSTer, gated by the PC's and Fightcade's checks; the others' stays until gated on their hardware
if(MVC2_MISTER)
    set(MVC2_GAME_OPT -O2 CACHE STRING "the game's C's optimization")
else()
    set(MVC2_GAME_OPT -O1 CACHE STRING "the game's C's optimization")
endif()
set(GAME_OPTIONS
    -std=gnu89 -fwrapv -fno-strict-aliasing -fsigned-char -w ${MVC2_GAME_OPT} -fno-delete-null-pointer-checks
    -include ${PORT}/include/port_prefix.h
    -fno-toplevel-reorder -fno-zero-initialized-in-bss)
# floats in single precision, as the SH-4's: x87's wider ones round differently
set(HOST_FLOAT)
if(WIN32 OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i686|AMD64|i386")
    set(HOST_FLOAT -mfpmath=sse -msse2)
    list(APPEND GAME_OPTIONS -malign-data=abi ${HOST_FLOAT})
endif()
# checks where the host can differ from the SH-4, logged to check.log (port/naomi/host_check.c)
option(MVC2_CHECK "the game's C with gcc's float, shift, bounds and division checks" OFF)
if(MVC2_CHECK)
    list(APPEND GAME_OPTIONS -fsanitize=float-cast-overflow,shift-exponent,bounds,integer-divide-by-zero
         -fsanitize-recover=all)
endif()
option(MVC2_FMAC "the sums SHC fused into one FMAC rounded once, as on the board (OFF: a multiply and an add, as Flycast Dojo plays Fightcade's replays)" ON)
option(MVC2_PSP_ORDER "PSP: the busiest routines together at the start of the program's code" ON)
set(MVC2_PSP_TITLE "Marvel vs. Capcom 2" CACHE STRING "PSP: the title the XMB shows (a test build's own, to tell it apart)")
option(MVC2_PSP_LINES "PSP: debug lines in the program, for a sampled run's source lines (the main CPU's code unchanged)" OFF)
if(PSP AND MVC2_PSP_LINES)
    add_compile_options(-g)
endif()
option(MVC2_PSP_PREFETCH "PSP: lines read ahead in the drawing's hot loops (host_prefetch; off for a trial)" ON)
if(PSP AND NOT MVC2_PSP_PREFETCH)
    add_compile_definitions(HOST_NO_PREFETCH)
endif()
# -O2 by default on the PSP alone, where it measured faster on the console with the same game RAM
if(PSP)
    set(GAME_O2_DEFAULT ON)
else()
    set(GAME_O2_DEFAULT OFF)
endif()
option(MVC2_GAME_O2 "the game's C at -O2 (a trial on the 3DS)" ${GAME_O2_DEFAULT})
if(MVC2_GAME_O2)
    list(APPEND GAME_OPTIONS -O2)
endif()
option(MVC2_GAME_OS "3DS and PSP: the game's C at -Os (about 400 KB less code: for the Old 3DS, and the PSP's I-cache)" OFF)
if((NINTENDO_3DS OR PSP) AND MVC2_GAME_OS)
    list(APPEND GAME_OPTIONS -Os)
endif()
# SHC's enums are int-sized, devkitARM's as small as fit: the game and the layers that share its structs
# keep SHC's (the 3DS front end, with libctru, keeps devkitARM's)
set(HOST_ABI)
set(RELOC_FLAGS)
if(NINTENDO_3DS)
    set(HOST_ABI -fno-short-enums)
    set(RELOC_FLAGS -Wl,--no-enum-size-warning)
    list(APPEND GAME_OPTIONS ${HOST_ABI})
endif()

# Capcom's modules; shl23_00, at02_scene, pl_char (on x86 set05_05, on the PSP sprite) go through wrappers
# (port/game/*_host.c)
file(GLOB ARC_SOURCES ${GAME}/src/*.c)
list(REMOVE_ITEM ARC_SOURCES ${GAME}/src/shl23_00.c ${GAME}/src/at02_scene.c ${GAME}/src/pl_char.c)
if(WIN32 OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i686|AMD64|i386")
    # in its place in the list: the objects' order lays out their data
    list(TRANSFORM ARC_SOURCES REPLACE "^${GAME}/src/set05_05\.c$" "${PORT}/game/set05_05_host.c")
endif()
list(APPEND ARC_SOURCES ${PORT}/game/shl23_00_host.c ${PORT}/game/at02_scene_host.c ${PORT}/game/pl_char_host.c)
set_source_files_properties(${PORT}/game/shl23_00_host.c ${PORT}/game/at02_scene_host.c ${PORT}/game/pl_char_host.c
                            ${PORT}/game/set05_05_host.c PROPERTIES INCLUDE_DIRECTORIES ${GAME}/src)
if(PSP)
    list(TRANSFORM ARC_SOURCES REPLACE "^${GAME}/src/sprite\.c$" "${PORT}/game/sprite_host.c")
    set_source_files_properties(${PORT}/game/sprite_host.c PROPERTIES INCLUDE_DIRECTORIES ${GAME}/src)
endif()
# the host runs the game's main() as a coroutine
set_source_files_properties(${GAME}/src/main.c PROPERTIES COMPILE_DEFINITIONS main=naomi_main)

# the decomp's plain-C library modules, one copy for both programs
set(LIB_SOURCES ${GAME}/src/lib/libnaomi/nlmath.c ${GAME}/src/lib/libnaomi/nlproj.c ${PORT}/nl/nl_work.c
    ${PORT}/nl/nl_matha.c)
add_library(mvc2_lib OBJECT ${LIB_SOURCES})
target_include_directories(mvc2_lib PRIVATE ${GAME}/src/lib/libnaomi ${GAME}/include ${PORT}/include)
target_compile_options(mvc2_lib PRIVATE ${GAME_OPTIONS})

# the board and Sega's libraries
set(NAOMI_SOURCES
    ${PORT}/naomi/naomi_board.c
    ${PORT}/sys/fmac_int.c
    ${PORT}/naomi/naomi_rom.c
    ${PORT}/naomi/naomi_zip.c
    ${PORT}/naomi/naomi_m1.c
    ${PORT}/naomi/nl_am.c
    ${PORT}/nl/nl_xmtr.c
    ${PORT}/nl/nl_tex.c
    ${PORT}/nl/nl_lz.c
    ${PORT}/nl/nl_sprite.c
    ${PORT}/nl/nl_strip.c
    ${PORT}/nl/nl_fog.c
    ${PORT}/nl/nl_sys.c
    ${PORT}/nl/nl_obj.c
    ${PORT}/render/rd.c
    ${PORT}/snd/arm7.c
    ${PORT}/snd/aica.c
    ${PORT}/snd/aica_dsp_jit.c
    ${PORT}/snd/snd_drv.c
    ${PORT}/snd/snd_nl.c
    ${PORT}/snd/s3l.c)
if(PSP)
    # the sound's data, which the Media Engine writes, on cache lines of its own (psp_audio.c)
    list(FIND NAOMI_SOURCES ${PORT}/snd/arm7.c at)
    list(INSERT NAOMI_SOURCES ${at} ${PORT}/psp/snd_lines_begin.c)
    list(FIND NAOMI_SOURCES ${PORT}/snd/s3l.c at)
    list(INSERT NAOMI_SOURCES ${at} ${PORT}/psp/snd_lines_end.c)
endif()
if(MVC2_CHECK)
    list(APPEND NAOMI_SOURCES ${PORT}/naomi/host_check.c)
endif()
# the test build runs on a New 3DS at an Old 3DS's speed, with its memory
option(OLD_3DS_TEST "3DS: make the Old 3DS test build" OFF)
option(CTR_SOFT "3DS: build in the software renderer, for checks" OFF)
# into prof.log
option(MVC2_PROF "3DS: time the frame's parts" OFF)
option(MVC2_PROF_FUNCS "3DS: with MVC2_PROF, sample the game's functions" OFF)
option(MVC2_PROF_GAMEZ "3DS: with MVC2_PROF, time the game's frame by its routines" OFF)
set(PROGRAM_WRAPS)
set(PROGRAM_WRAP_SOURCES)
if(NINTENDO_3DS AND MVC2_PROF)
    add_compile_definitions(MVC2_PROF)
    if(MVC2_PROF_GAMEZ)
        add_compile_definitions(MVC2_PROF_GAMEZ)
        # the calls between the game's modules to these go through port/ctr/ctr_gamez_wrap.c, inside each program
        foreach(fn System_Main SysTrans SysTrans2 load_main sound_vsync key_repeat vms_check col_send game_main
                   all_work_move pl_work_move scr_move stage_frame pl_hit_work_set bg_move fog_set hit_ck0 vs_sys_move
                   flash_move disp_prio_clr pl_work_disp game_disp sprite_disp sprite_tex_set lz_unpack
                   work_list_move work_list_disp)
            list(APPEND PROGRAM_WRAPS -Wl,--wrap=${fn})
        endforeach()
        set(PROGRAM_WRAP_SOURCES ${PORT}/ctr/ctr_gamez_wrap.c)
    endif()
    if(MVC2_PROF_FUNCS)
        add_compile_definitions(MVC2_PROF_FUNCS)
        add_compile_options(-finstrument-functions)
        # the front end, the sound and port/sys left out: their time is in the zones
        file(GLOB UNTIMED ${PORT}/ctr/*.c ${PORT}/snd/*.c ${PORT}/sys/*.c)
        set_source_files_properties(${UNTIMED} PROPERTIES COMPILE_OPTIONS -fno-instrument-functions)
    endif()
endif()
if(NINTENDO_3DS)
    set(HOST_SOURCES)
    if(CTR_SOFT)
        set(HOST_SOURCES ${PORT}/render/rd_soft.c)
        add_compile_definitions(CTR_SOFT)
    endif()
    list(APPEND HOST_SOURCES ${PORT}/snd/arm7_jit.c ${PORT}/snd/arm7_jit2.c)    # the sound CPU translated to the 3DS's own code
    # nlmath's angle routines' last answers kept (nl_trig.c), for the game's calls and nlproj's
    list(APPEND HOST_SOURCES ${PORT}/nl/nl_trig.c)
    set(TRIG_WRAPS -Wl,--wrap=nlSin,--wrap=nlCos,--wrap=nlTan,--wrap=nlArcTan)
    set_source_files_properties(${PORT}/snd/arm7.c PROPERTIES COMPILE_DEFINITIONS ARM7_JIT)
    # the MPCore's VFP is the ARM1136's VFP11, whose latencies gcc schedules for only under that name
    set_source_files_properties(${PORT}/nl/nl_obj.c PROPERTIES COMPILE_OPTIONS -mtune=arm1136jf-s)
    set(SYS_SOURCES ${PORT}/sys/coro_arm.c ${PORT}/sys/divzero_arm.c)
    set(FRONT_SOURCES ${PORT}/ctr/main_3ds.c ${PORT}/ctr/ctr_render.c ${PORT}/ctr/ctr_rom.c ${PORT}/ctr/ctr_m1.c ${PORT}/ctr/ctr_art.c ${PORT}/ctr/ctr_options.c
        ${PORT}/ctr/ctr_log.c ${PORT}/ctr/ctr_save.c ${PORT}/ctr/ctr_alloc.c ${PORT}/ctr/ctr_memcpy.c ${PORT}/ctr/ctr_stream.c ${PORT}/sdl/start.c
        ${PORT}/sdl/test_mode.c)
elseif(PSP)
    # rd's lists drawn by the GE (psp_render.c, psp_list.c), rd_soft the fallback
    # nlmath's angle routines' last answers kept (nl_trig.c), as on the 3DS: the library's sin and cos are soft doubles
    set(HOST_SOURCES ${PORT}/render/rd_soft.c ${PORT}/nl/nl_trig.c)
    set(TRIG_WRAPS -Wl,--wrap=nlSin,--wrap=nlCos,--wrap=nlTan,--wrap=nlArcTan)
    set(SYS_SOURCES ${PORT}/sys/coro_psp.c ${PORT}/sys/divzero_mips.c)
    set(FRONT_SOURCES ${PORT}/psp/main_psp.c ${PORT}/psp/psp_mem.c ${PORT}/psp/psp_video.c ${PORT}/psp/psp_render.c
        ${PORT}/psp/psp_list.c ${PORT}/psp/psp_audio.c ${PORT}/psp/psp_log.c ${PORT}/psp/psp_crash.c ${PORT}/psp/psp_prof.c ${PORT}/psp/psp_me.c ${PORT}/psp/psp_reader.c ${PORT}/sdl/start.c
        ${PORT}/tools/rd_frame_file.c ${PORT}/psp/psp_art.c)
    # the pillar pictures (port/psp/gfx), their PNGs as arrays (psp_art.c reads them)
    set(PSP_ART_H ${CMAKE_BINARY_DIR}/gen/psp_art_png.h)
    file(WRITE ${PSP_ART_H} "")
    foreach(art left right)
        file(READ ${PORT}/psp/gfx/${art}.png ART_HEX HEX)
        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," ART_HEX "${ART_HEX}")
        file(APPEND ${PSP_ART_H} "static const unsigned char ${art}_png[] = { ${ART_HEX} };\n")
        set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PORT}/psp/gfx/${art}.png)
    endforeach()
    set_source_files_properties(${PORT}/psp/psp_art.c PROPERTIES INCLUDE_DIRECTORIES
                                "${CMAKE_BINARY_DIR}/gen;${PORT}/ext/libchdr/deps/miniz-3.1.2")
else()
    set(HOST_SOURCES ${PORT}/render/rd_soft.c)
    if(MVC2_SDL3)
        list(APPEND HOST_SOURCES ${PORT}/render/rd_gpu.c)
    endif()
    if(WIN32)
        set(SYS_SOURCES ${PORT}/sys/coro_win32.c ${PORT}/sys/divzero_win32.c)
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
        set(SYS_SOURCES ${PORT}/sys/coro_arm.c ${PORT}/sys/divzero_arm.c)
    elseif(LINUX_X86)
        set(SYS_SOURCES ${PORT}/sys/coro_i386.c ${PORT}/sys/divzero_i386.c)
    else()
        message(FATAL_ERROR "port/sys has coroutines and division by zero for Windows (x86), x86 Linux and 32-bit ARM only")
    endif()
    if(MVC2_MISTER)
        set(FRONT_SOURCES ${PORT}/mister/main_mister.c ${PORT}/mister/mister_pinedra.c ${PORT}/mister/mister_audio.c
            ${PORT}/mister/mister_save.c ${PORT}/mister/mister_prof.c ${PORT}/mister/mister_input.c ${PORT}/sdl/start.c
            ${PORT}/render/rd_gl1.c ${PORT}/render/rd_gl1_tex.c ${PORT}/mister/mister_reader.c ${PORT}/mister/mister_log.c
            ${PORT}/sdl/test_mode.c ${PORT}/sdl/menu_font.c)
        # -O3 vectorizes the translucent sort's box tests
        set_source_files_properties(${PORT}/render/rd_gl1.c PROPERTIES COMPILE_OPTIONS -O3)
        set_source_files_properties(${PORT}/nl/nl_obj.c ${PORT}/render/rd.c ${PORT}/nl/nl_matha.c PROPERTIES COMPILE_OPTIONS -O3)
        # the front end and the drawing built from a profile of a run (port/mister/pgo, tools/mister/README.md): use
        # (the default while it is there: about 6 % less of the drawing's time in the character select), generate
        # (a build that writes one to MVC2_PGO_DIR on the MiSTer), or none. A profile from older code is used where it
        # still fits and left out where it doesn't; the game's own modules are built as before
        if(EXISTS ${PORT}/mister/pgo)
            set(PGO_DEFAULT use)
        else()
            set(PGO_DEFAULT "")
        endif()
        set(MVC2_PGO ${PGO_DEFAULT} CACHE STRING "MiSTer: the front end and the drawing from a profile: use, generate or none")
        set(MVC2_PGO_DIR /tmp/mvc2pgo CACHE STRING "MiSTer: where a generate build writes its profile")
        if(MVC2_PGO STREQUAL "generate")
            set_property(SOURCE ${FRONT_SOURCES} APPEND PROPERTY COMPILE_OPTIONS
                         -fprofile-generate=${MVC2_PGO_DIR} -fprofile-update=atomic)
            set(MVC2_PGO_LINK -fprofile-generate)
        elseif(MVC2_PGO STREQUAL "use")
            set_property(SOURCE ${FRONT_SOURCES} APPEND PROPERTY COMPILE_OPTIONS
                         -fprofile-use=${PORT}/mister/pgo -fprofile-correction -Wno-missing-profile -Wno-coverage-mismatch)
        endif()
    else()
    set(FRONT_SOURCES ${PORT}/sdl/main_sdl.c ${PORT}/sdl/hunt.c ${PORT}/sdl/start.c ${PORT}/sdl/input_sdl.c ${PORT}/sdl/menu_sdl.c
        ${PORT}/sdl/sdl_audio.c ${PORT}/sdl/sdl_save.c ${PORT}/sdl/test_mode.c ${PORT}/sdl/menu_font.c)
    if(WIN32)
        list(APPEND FRONT_SOURCES ${PORT}/sdl/console_win32.c ${PORT}/sdl/display_win32.c)
    endif()
    if(MVC2_SDL3)
        # OpenGL's fixed function (the MiSTer's rd_gl1), where SDL_GPU has neither Vulkan nor Direct3D 12
        list(APPEND FRONT_SOURCES ${PORT}/sdl/gl_sdl.c ${PORT}/render/rd_gl1.c ${PORT}/render/rd_gl1_tex.c)
    endif()
    endif()
endif()
if(NINTENDO_3DS)
    list(APPEND FRONT_SOURCES ${PORT}/ctr/ctr_audio.c ${PORT}/ctr/ctr_code.c)    # the sound out and its translator's code memory
    if(MVC2_PROF)
        # the game's calls into the library timed by layer, through the linker's --wrap
        list(APPEND FRONT_SOURCES ${PORT}/ctr/ctr_prof.c ${PORT}/ctr/ctr_game_zones.c ${PORT}/ctr/ctr_regcheck.c
            ${PORT}/ctr/ctr_gpuprobe.c)
        if(MVC2_PROF_GAMEZ)
            list(APPEND FRONT_SOURCES ${PORT}/ctr/ctr_gamez.c)
        endif()
        set_source_files_properties(${PORT}/ctr/ctr_game_zones.c PROPERTIES INCLUDE_DIRECTORIES ${GAME}/include
                                    COMPILE_OPTIONS -fno-short-enums)
        set(GAME_ZONE_WRAPS
            nlArcTan2 nlCalcPoint nlCalcVector nlFloor nlFogDensity nlFogDensityF nlFogTableColor
            nlFogTableOff nlFogTableSet nlGetMatrix nlInitLibrary nlInitMatrix nlLoadMatrix nlLookAt
            nlMakeModelViewProjectMatrix nlMatrixMode nlMultiMatrix nlObjPutSetFadeColor nlPerspective nlPerspectiveX
            nlPopMatrix nlPow nlProjectScreen nlPushMatrix nlRand nlRandom nlRotateAX nlRotateAY nlRotateAZ nlScaleV
            nlScreenPoint nlSetBackColor nlSetBackColorZ nlSetScaleFactor nlShiftPerspective nlTranslate
            nlTranslateV nlUnitMatrix nlUnitVector nlaChangeCreditSequence nlaCheckCreditEnough nlaGetBackupData
            nlaGetBootFlag nlaGetCreditConf nlaGetCreditData nlaGetRTC nlaGetSystemFlag nlaInTestMode nlaInitSystem
            nlaInitUserBackup nlaInitUserEEPROM nlaJAMMACreditInit nlaJAMMAInit nlaJAMMASetPrimaryNode
            nlaJumpTestMode nlaNRSDecodeRead nlaPutLED nlaROMTypeSet nlaReadROMDMA nlaUpdateSystemBackup
            nlaUpdateUserBackup nlaUpdateUserEEPROM
            gxCmdQueueRun gspSubmitGxCommand)    # C3D_FrameEnd's hand-over to GSP (ctr_prof.c)
    endif()
endif()
add_library(mvc2_port OBJECT ${NAOMI_SOURCES} ${HOST_SOURCES})
target_include_directories(mvc2_port PRIVATE ${PORT}/include ${PORT}/sys ${PORT}/render ${PORT}/nl ${PORT}/snd)
# the library routines take the game's own types from its headers
set_source_files_properties(${PORT}/naomi/nl_am.c PROPERTIES INCLUDE_DIRECTORIES ${GAME}/include)
target_compile_options(mvc2_port PRIVATE -O2 -std=gnu17 -fwrapv -fno-strict-aliasing -fsigned-char -Wall ${HOST_ABI} ${HOST_FLOAT})
if(PSP)
    target_compile_definitions(mvc2_port PRIVATE RD_ABGR)     # the GE's 8888 texels
elseif(MVC2_SDL3 AND NOT NINTENDO_3DS)
    find_package(SDL3 REQUIRED CONFIG)
    set(SDL_LIBS SDL3::SDL3)
    target_compile_definitions(mvc2_port PRIVATE RD_SDL3)
    target_link_libraries(mvc2_port PRIVATE SDL3::SDL3)
elseif(MVC2_MISTER)
    target_link_libraries(mvc2_port PRIVATE pthread)
elseif(NOT NINTENDO_3DS)
    find_package(SDL2 REQUIRED)
    set(SDL_LIBS SDL2::SDL2main SDL2::SDL2)
    target_link_libraries(mvc2_port PRIVATE SDL2::SDL2)
endif()
# the coroutines and the division by zero, which keep their state when the rest starts again
add_library(mvc2_sys OBJECT ${SYS_SOURCES})
target_include_directories(mvc2_sys PRIVATE ${PORT}/include ${PORT}/sys)
target_compile_options(mvc2_sys PRIVATE -O2 -std=gnu17 -Wall ${HOST_FLOAT})
# the names that bracket a relocatable's .data and .bss, linked first and last in it (port/sys/restart.c)
add_library(mvc2_span_begin OBJECT ${PORT}/game/span_begin.c)
add_library(mvc2_span_end OBJECT ${PORT}/game/span_end.c)

# A program: the game's objects, what gen_host finds they use and nothing defines (the program's data,
# filled from the user's image at start-up, aliases into it, fixed buffers, and stubs for the library
# routines the port lacks), all made one relocatable (ld -r -d) in which only NAME_program stays global
# (objcopy -G). What it uses and does not define binds to the host, and two programs can share one
# executable. The names it keeps in place become NAME.name, and the executable's link gives their addresses.
set(GEN ${CMAKE_BINARY_DIR}/gen)
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(UNDERSCORE --underscore)
    set(SYM_PREFIX _)
endif()
function(mvc2_program NAME)
    cmake_parse_arguments(P "" "PROGRAM" "SOURCES;DEFINITIONS;HOST;LOCAL" ${ARGN})
    set(OUT ${GEN}/${NAME})
    # host copies of some modules (port/tools/fmac.py): the sums SHC fused into FMACs as FMAC( a, b, c ), and on
    # ARM each float made unsigned as SHC's code makes it (ARM's own conversion stops a negative at 0)
    set(SITES ${PORT}/data/fmac_${P_PROGRAM}.tsv)
    if(NOT MVC2_FMAC)
        set(SITES ${OUT}/fmac_none.tsv)
        file(WRITE ${SITES} "")
    endif()
    # and on every host, the calls whose arguments SHC's code takes in an order gcc may not (port/data/arg_order.tsv)
    # and the reads past a table that need the board's word after it (port/data/board_reads.tsv)
    set(EDITS ${PORT}/data/arg_order.tsv ${PORT}/data/board_reads.tsv)
    if(NINTENDO_3DS OR CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
        list(APPEND EDITS ${PORT}/data/arm_u32.tsv)
    endif()
    file(STRINGS ${SITES} ROWS)
    foreach(e ${EDITS})
        file(STRINGS ${e} MORE)
        list(APPEND ROWS ${MORE})
    endforeach()
    list(TRANSFORM ROWS REPLACE "	.*" "")
    if(ROWS)
        set(SOURCES)
        foreach(s ${P_SOURCES})
            get_filename_component(stem ${s} NAME_WE)
            get_filename_component(dir ${s} DIRECTORY)
            # a wrapper (port/game/*_host.c) is copied beside the module's copy, so that it includes that
            string(REGEX REPLACE "_host$" "" module ${stem})
            if(module IN_LIST ROWS)
                set(from ${s})
                if(NOT module STREQUAL stem)
                    set(from ${GAME}/src/${module}.c)
                endif()
                add_custom_command(OUTPUT ${OUT}/fmac/${module}.c
                    COMMAND Python3::Interpreter ${PORT}/tools/fmac.py rewrite ${SITES} ${from} ${OUT}/fmac/${module}.c
                            ${EDITS}
                    DEPENDS ${from} ${SITES} ${EDITS} ${PORT}/tools/fmac.py)
                if(NOT module STREQUAL stem)
                    add_custom_command(OUTPUT ${OUT}/fmac/${stem}.c
                        COMMAND ${CMAKE_COMMAND} -E copy ${s} ${OUT}/fmac/${stem}.c
                        DEPENDS ${s} ${OUT}/fmac/${module}.c)
                endif()
                set_source_files_properties(${OUT}/fmac/${stem}.c PROPERTIES INCLUDE_DIRECTORIES "${dir};${GAME}/src")
                set(s ${OUT}/fmac/${stem}.c)
            endif()
            list(APPEND SOURCES ${s})
        endforeach()
        set(P_SOURCES ${SOURCES})
    endif()
    add_library(${NAME}_game OBJECT ${P_SOURCES})
    target_include_directories(${NAME}_game PRIVATE ${GAME}/include ${PORT}/include)
    target_compile_options(${NAME}_game PRIVATE ${GAME_OPTIONS})
    target_compile_definitions(${NAME}_game PRIVATE ${P_DEFINITIONS})
    set(HOST_DIRS)
    set(HOST_OBJECTS)
    set(LOCAL_OBJECTS)
    # LOCAL: host routines of this program alone, linked into its relocatable with the game
    if(P_LOCAL)
        add_library(${NAME}_local OBJECT ${P_LOCAL})
        target_include_directories(${NAME}_local PRIVATE ${PORT}/include ${GAME}/include)
        target_compile_options(${NAME}_local PRIVATE -O2 -std=gnu17 -fwrapv -fno-strict-aliasing -fsigned-char -Wall
                               ${HOST_ABI} ${HOST_FLOAT})
        list(APPEND P_HOST ${NAME}_local)
        set(LOCAL_OBJECTS "$<JOIN:$<TARGET_OBJECTS:${NAME}_local>,\n>\n")
    endif()
    foreach(h ${P_HOST})
        list(APPEND HOST_DIRS ${CMAKE_BINARY_DIR}/CMakeFiles/${h}.dir)
        list(APPEND HOST_OBJECTS $<TARGET_OBJECTS:${h}>)
    endforeach()
    add_custom_command(OUTPUT ${OUT}/host_data.c ${OUT}/host_stubs.c ${OUT}/host_defsyms.rsp ${OUT}/host_rename.txt
        COMMAND Python3::Interpreter ${PORT}/tools/gen_host.py ${GAME} ${CMAKE_NM} ${OUT}
                ${CMAKE_BINARY_DIR}/CMakeFiles/${NAME}_game.dir ${HOST_DIRS} --program=${P_PROGRAM} --prefix=${NAME}
                --ram-base=${MVC2_RAM_BASE} ${UNDERSCORE}
        DEPENDS ${NAME}_game ${P_HOST} $<TARGET_OBJECTS:${NAME}_game> ${HOST_OBJECTS} ${PORT}/tools/gen_host.py
                ${PORT}/data/stub_values.tsv
                ${GAME}/symbols.txt ${GAME}/symbols_dc.txt ${GAME}/mvc2.sub ${GAME}/mvc2dc.sub
                ${POINTERS}/pointers.tsv ${POINTERS}/pointers_dc.tsv
        COMMENT "Finding what the ${P_PROGRAM} program uses that nothing defines")
    add_library(${NAME}_gen OBJECT ${OUT}/host_data.c ${OUT}/host_stubs.c ${PORT}/game/program.c ${PROGRAM_WRAP_SOURCES})
    target_include_directories(${NAME}_gen PRIVATE ${PORT}/include)
    target_compile_options(${NAME}_gen PRIVATE -w)
    target_compile_definitions(${NAME}_gen PRIVATE PROGRAM=${NAME}_program)
    file(GENERATE OUTPUT ${OUT}/objects.rsp
         CONTENT "$<TARGET_OBJECTS:mvc2_span_begin>\n$<JOIN:$<TARGET_OBJECTS:${NAME}_game>,\n>\n$<JOIN:$<TARGET_OBJECTS:${NAME}_gen>,\n>\n${LOCAL_OBJECTS}$<TARGET_OBJECTS:mvc2_span_end>\n")
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${NAME}.o
        COMMAND ${CMAKE_C_COMPILER} -r -nostdlib -Wl,-d ${RELOC_FLAGS} ${PROGRAM_WRAPS} @${OUT}/objects.rsp -o ${OUT}/all.o
        COMMAND ${CMAKE_OBJCOPY} --redefine-syms=${OUT}/host_rename.txt -G ${SYM_PREFIX}${NAME}_program
                ${OUT}/all.o ${CMAKE_BINARY_DIR}/${NAME}.o
        DEPENDS ${NAME}_game ${NAME}_gen $<TARGET_OBJECTS:${NAME}_game> $<TARGET_OBJECTS:${NAME}_gen>
                mvc2_span_begin mvc2_span_end $<TARGET_OBJECTS:mvc2_span_begin> $<TARGET_OBJECTS:mvc2_span_end>
                ${OUT}/host_rename.txt ${OUT}/objects.rsp
        COMMENT "Making the ${P_PROGRAM} program one relocatable")
    set_source_files_properties(${CMAKE_BINARY_DIR}/${NAME}.o PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)
endfunction()

# the PSP has the Dreamcast's alone for now: the arcade's ROM wants streaming
if(NOT PSP)
    mvc2_program(arc PROGRAM arcade SOURCES ${ARC_SOURCES} DEFINITIONS BUILD_DATE=20000225 HOST mvc2_port mvc2_lib
                 LOCAL ${PORT}/naomi/naomi_pd.c)
endif()

# the Dreamcast's (USA disc): the same modules with its switches, src/dc's in place of their namesakes and
# its own beside them, but not its start-up (strt2.c) or Sega's library modules (src/dc/lib)
file(GLOB DC_ONLY_SOURCES ${GAME}/src/dc/*.c)
list(REMOVE_ITEM DC_ONLY_SOURCES ${GAME}/src/dc/strt2.c)
set(DC_SOURCES ${ARC_SOURCES})
foreach(f ${DC_ONLY_SOURCES})
    get_filename_component(n ${f} NAME)
    list(REMOVE_ITEM DC_SOURCES ${GAME}/src/${n})
endforeach()
list(APPEND DC_SOURCES ${DC_ONLY_SOURCES})
# what the Dreamcast adds to the board and the libraries: the disc, Shinobi and the music's player
add_library(mvc2_dc OBJECT ${PORT}/dc/dc_board.c ${PORT}/dc/dc_gd.c ${PORT}/dc/dc_shinobi.c ${PORT}/dc/dc_adx.c
            ${PORT}/dc/dc_vmu.c)
target_include_directories(mvc2_dc PRIVATE ${PORT}/include ${GAME}/include ${PORT}/snd)
target_compile_options(mvc2_dc PRIVATE -O2 -std=gnu17 -fwrapv -fno-strict-aliasing -fsigned-char -Wall ${HOST_ABI} ${HOST_FLOAT})
mvc2_program(dc PROGRAM dc SOURCES ${DC_SOURCES} DEFINITIONS TARGET_DC BUILD_DATE=20000529
             HOST mvc2_port mvc2_lib mvc2_dc)
target_sources(mvc2_dc PRIVATE ${PORT}/dc/dc_image.c)
# libchdr (port/ext/libchdr), for a disc image in a CHD: its codecs, with dr_flac, and its own LZMA, miniz and zstd;
# only the sectors' data is read, so no ECC, subcode or CRC
set(CHDR ${PORT}/ext/libchdr)
file(GLOB CHDR_SOURCES ${CHDR}/src/*.c)
add_library(mvc2_chdr STATIC ${CHDR_SOURCES} ${CHDR}/deps/lzma-26.02/src/LzmaDec.c ${CHDR}/deps/miniz-3.1.2/miniz.c
            ${CHDR}/deps/zstd-1.5.7/zstddeclib.c)
target_compile_definitions(mvc2_chdr PRIVATE MINIZ_NO_ARCHIVE_APIS MINIZ_NO_DEFLATE_APIS MINIZ_NO_STDIO MINIZ_NO_TIME
                           WANT_RAW_DATA_SECTOR=0 WANT_SUBCODE=0 VERIFY_BLOCK_CRC=0 LOWRAM_TARGET=0)
target_compile_options(mvc2_chdr PRIVATE -O2 -w ${HOST_ABI} ${HOST_FLOAT})

# an executable of the host and one or more programs (arc, dc), with the front end told which
function(mvc2_executable TARGET)
    set(HOST_LIBS mvc2_port mvc2_lib)
    set(OBJECTS ${CMAKE_BINARY_DIR}/${TARGET}_host.o $<TARGET_OBJECTS:mvc2_sys>)
    set(DEFS)
    foreach(p ${ARGN})
        list(APPEND OBJECTS ${CMAKE_BINARY_DIR}/${p}.o)
        if(p STREQUAL "dc")
            list(APPEND HOST_LIBS mvc2_dc)
            list(APPEND DEFS MVC2_DC)
        else()
            list(APPEND DEFS MVC2_ARCADE)
        endif()
    endforeach()
    # the host's layers as one relocatable too, between the span names (renamed host_span_*), so that
    # their data can be put back as it started
    set(HOST_RSP ${GEN}/${TARGET}_host.rsp)
    set(HOST_CONTENT "$<TARGET_OBJECTS:mvc2_span_begin>\n")
    set(HOST_DEPENDS mvc2_span_begin mvc2_span_end $<TARGET_OBJECTS:mvc2_span_begin> $<TARGET_OBJECTS:mvc2_span_end>)
    foreach(h ${HOST_LIBS})
        string(APPEND HOST_CONTENT "$<JOIN:$<TARGET_OBJECTS:${h}>,\n>\n")
        list(APPEND HOST_DEPENDS ${h} $<TARGET_OBJECTS:${h}>)
    endforeach()
    string(APPEND HOST_CONTENT "$<TARGET_OBJECTS:mvc2_span_end>\n")
    file(GENERATE OUTPUT ${HOST_RSP} CONTENT "${HOST_CONTENT}")
    set(RENAME)
    foreach(s data_begin data_end bss_begin bss_end)
        list(APPEND RENAME --redefine-sym ${SYM_PREFIX}span_${s}=${SYM_PREFIX}host_span_${s})
    endforeach()
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${TARGET}_host.o
        COMMAND ${CMAKE_C_COMPILER} -r -nostdlib ${RELOC_FLAGS} ${TRIG_WRAPS} @${HOST_RSP} -o ${GEN}/${TARGET}_host_all.o
        COMMAND ${CMAKE_OBJCOPY} ${RENAME} ${GEN}/${TARGET}_host_all.o ${CMAKE_BINARY_DIR}/${TARGET}_host.o
        DEPENDS ${HOST_DEPENDS} ${HOST_RSP}
        COMMENT "Making the host's layers one relocatable for ${TARGET}")
    set_source_files_properties(${CMAKE_BINARY_DIR}/${TARGET}_host.o PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)
    add_executable(${TARGET} ${OBJECTS} ${FRONT_SOURCES} ${PORT}/sys/restart.c)
    if(MVC2_PGO_LINK)
        target_link_options(${TARGET} PRIVATE ${MVC2_PGO_LINK})
    endif()
    target_include_directories(${TARGET} PRIVATE ${PORT}/include ${PORT}/render ${PORT}/snd)
    target_compile_definitions(${TARGET} PRIVATE ${DEFS})
    if("dc" IN_LIST ARGN)
        target_link_libraries(${TARGET} PRIVATE mvc2_chdr)
    endif()
    target_compile_options(${TARGET} PRIVATE -O2 -std=gnu17 -Wall ${HOST_FLOAT})
    foreach(p ${ARGN})
        # the addresses of the names the program keeps in place (NAME.sys_font_thd)
        target_link_options(${TARGET} PRIVATE @${GEN}/${p}/host_defsyms.rsp)
        set_property(TARGET ${TARGET} APPEND PROPERTY LINK_DEPENDS ${GEN}/${p}/host_defsyms.rsp)
    endforeach()
    if(NINTENDO_3DS)
        target_include_directories(${TARGET} PRIVATE ${PORT}/ctr ${PORT}/sdl)
        target_link_libraries(${TARGET} PRIVATE citro3d ctru m)
        if(OLD_3DS_TEST)
            target_compile_definitions(${TARGET} PRIVATE OLD_3DS_TEST=1)
            set(CIA_MODE -DTITLE=MVSC2O3DS -DPRODUCT_CODE=CTR-H-MVCO -DUNIQUE_ID=0xF5C33 -DMODE_EXT=Legacy -DCPU_SPEED=268MHz
                -DL2_CACHE=false)
            set(TITLE "Marvel vs. Capcom 2 O3DS")
            set(DESCRIPTION "Old 3DS test build")
        else()
            set(CIA_MODE -DTITLE=MVSC2 -DPRODUCT_CODE=CTR-H-MVC2 -DUNIQUE_ID=0xF5C32 -DMODE_EXT=124MB -DCPU_SPEED=804MHz
                -DL2_CACHE=true)
            set(TITLE "Marvel vs. Capcom 2")
            set(DESCRIPTION "NAOMI and Dreamcast port")
        endif()
        # division by zero gives 0, as SHC's code did (divzero_arm.c)
        foreach(fn __aeabi_idiv __aeabi_uidiv __aeabi_idivmod __aeabi_uidivmod ${GAME_ZONE_WRAPS})
            target_link_options(${TARGET} PRIVATE "-Wl,--wrap=${fn}")
        endforeach()
        # an allocation that fails stops the program, saying so (ctr_alloc.c)
        target_link_options(${TARGET} PRIVATE -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc)
        # memcpy by ldm and stm with MVC2_MEMCPY_FAST=1 (ctr_memcpy.c)
        target_link_options(${TARGET} PRIVATE -Wl,--wrap=memcpy)
        # citro3d's GX queue, for MVC2_LIST_SPLIT to start it early (ctr_render.c)
        target_link_options(${TARGET} PRIVATE -Wl,--wrap=GX_BindQueue)
        target_link_options(${TARGET} PRIVATE -Wl,--no-enum-size-warning)
        target_link_options(${TARGET} PRIVATE ${TRIG_WRAPS})
        # the vertex shader, embedded as a header
        set(SHADERS ${CMAKE_BINARY_DIR}/shaders)
        add_custom_command(OUTPUT ${SHADERS}/ctr_render_shbin.h
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS}
            COMMAND ${CTR_PICASSO} -o ${SHADERS}/ctr_render.shbin ${PORT}/ctr/ctr_render.v.pica
            COMMAND ${CMAKE_COMMAND} -DIN=${SHADERS}/ctr_render.shbin -DOUT=${SHADERS}/ctr_render_shbin.h
                    -DNAME=ctr_render_shbin -P ${PORT}/cmake/3ds-bin2c.cmake
            DEPENDS ${PORT}/ctr/ctr_render.v.pica ${PORT}/cmake/3ds-bin2c.cmake
            COMMENT "Assembling the 3DS renderer's shader")
        target_sources(${TARGET} PRIVATE ${SHADERS}/ctr_render_shbin.h)
        # the pictures (port/ctr/gfx), as LZ11-packed RGBA8 textures; the opaque background RGB565, as the bottom screen
        foreach(art version left right options_bg native stretch switch controls unlock dip hires lores)
            set(format rgba8)
            if(art STREQUAL options_bg)
                set(format rgb565)
            endif()
            add_custom_command(OUTPUT ${SHADERS}/${art}_lz.h
                COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS}
                COMMAND ${CTR_TEX3DS} -f ${format} -z lz11 -r -o ${SHADERS}/${art}.lz ${PORT}/ctr/gfx/${art}.png
                COMMAND ${CMAKE_COMMAND} -DIN=${SHADERS}/${art}.lz -DOUT=${SHADERS}/${art}_lz.h -DNAME=${art}_lz
                        -P ${PORT}/cmake/3ds-bin2c.cmake
                DEPENDS ${PORT}/ctr/gfx/${art}.png ${PORT}/cmake/3ds-bin2c.cmake
                COMMENT "Packing ${art}.png")
            target_sources(${TARGET} PRIVATE ${SHADERS}/${art}_lz.h)
        endforeach()
        target_include_directories(${TARGET} PRIVATE ${SHADERS} ${PORT}/render)
        ctr_create_3dsx(${TARGET} "${TITLE}" "${DESCRIPTION}" ${PORT}/ctr/icon.png)
        set(CIA_DIR ${PORT}/ctr/cia)
        if(MVC2_PROF)
            # the profiler's builds may read the CPU's performance counters (MVC2_PERFCTR=1)
            set(CIA_DIR ${CMAKE_BINARY_DIR}/cia)
            file(READ ${PORT}/ctr/cia/mvc2.rsf RSF)
            string(REPLACE "    OutputDebugString: 61" "    OutputDebugString: 61\n    ControlPerformanceCounter: 62"
                   RSF "${RSF}")
            file(WRITE ${CIA_DIR}/${TARGET}.rsf "${RSF}")
            configure_file(${PORT}/ctr/cia/banner.png ${CIA_DIR}/banner.png COPYONLY)
            configure_file(${PORT}/ctr/cia/banner.wav ${CIA_DIR}/banner.wav COPYONLY)
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PORT}/ctr/cia/mvc2.rsf)
        endif()
        ctr_create_cia(${TARGET} "${TITLE}" "${DESCRIPTION}" ${CIA_DIR} ${PORT}/ctr/icon.png ${CIA_MODE})
    elseif(PSP)
        target_include_directories(${TARGET} PRIVATE ${PORT}/psp ${PORT}/sdl)
        # the custom firmware's calls: the crash handler (psp_crash.c) and the clock
        target_link_libraries(${TARGET} PRIVATE pspgu pspge pspdisplay pspctrl psppower pspaudio pspdebug pspkubridge
                              pspsystemctrl_user pspdmac m)
        # an allocation that fails is logged (psp_mem.c)
        target_link_options(${TARGET} PRIVATE -Wl,--wrap=malloc,--wrap=calloc,--wrap=realloc ${TRIG_WRAPS}
                            -Wl,-Map=${CMAKE_BINARY_DIR}/${TARGET}.map)
        # the routines run most first, in as few cache lines as they fit (port/tools/psp_order.py makes the list)
        if(MVC2_PSP_ORDER)
            target_link_options(${TARGET} PRIVATE -Wl,--section-ordering-file=${PORT}/psp/hot_text.ld)
            set_property(TARGET ${TARGET} APPEND PROPERTY LINK_DEPENDS ${PORT}/psp/hot_text.ld)
        endif()
        # the whole of a 64 MB model's memory (PARAM.SFO's MEMSIZE=1)
        # the XMB's icon, background and music (SND0.AT3 made by port/tools/psp_snd0.py)
        create_pbp_file(TARGET ${TARGET} TITLE "${MVC2_PSP_TITLE}" VERSION 01.00 MEMSIZE 1
                        ICON_PATH ${PORT}/psp/gfx/icon0.png BACKGROUND_PATH ${PORT}/psp/gfx/pic1.png
                        MUSIC_PATH ${PORT}/psp/SND0.AT3)
    else()
        target_link_libraries(${TARGET} PRIVATE ${SDL_LIBS} m)
        if(MVC2_MISTER)
            target_include_directories(${TARGET} PRIVATE ${PORT}/mister ${PORT}/sdl)
            target_link_libraries(${TARGET} PRIVATE pthread dl)
        endif()
        if(MVC2_SDL3)
            target_compile_definitions(${TARGET} PRIVATE MVC2_SDL3 RD_SDL3)
        endif()
        if(WIN32)
            # no console of its own (console_win32.c)
            set_target_properties(${TARGET} PROPERTIES WIN32_EXECUTABLE ON)
            # a fixed load address, so the RAM at MVC2_RAM_BASE stays free and runs are repeatable
            target_link_options(${TARGET} PRIVATE "-Wl,--disable-dynamicbase"
                -Wl,--major-os-version,6,--minor-os-version,0,--major-subsystem-version,6,--minor-subsystem-version,0
                -Wl,--nxcompat,--tsaware)
            if(CMAKE_SIZEOF_VOID_P EQUAL 4)
                # 4 GB of addresses on 64-bit Windows, not 2: the GPU driver's room at large frames
                target_link_options(${TARGET} PRIVATE -Wl,--large-address-aware)
            endif()
            # the icon and the version that Explorer shows
            if("${TARGET}" STREQUAL "mvc2dc")
                set(RC_DESCRIPTION "Marvel vs. Capcom 2 port (Dreamcast version)")
            else()
                set(RC_DESCRIPTION "Marvel vs. Capcom 2 port (arcade and Dreamcast versions)")
            endif()
            set(RC_ICON ${PORT}/sdl/mvc2.ico)
            set(RC_V1 0)
            set(RC_V2 0)
            set(RC_V3 0)
            if(MVC2_VERSION MATCHES "^v?([0-9]+)\\.([0-9]+)(\\.([0-9]+))?")
                set(RC_V1 ${CMAKE_MATCH_1})
                set(RC_V2 ${CMAKE_MATCH_2})
                if(CMAKE_MATCH_4)
                    set(RC_V3 ${CMAKE_MATCH_4})
                endif()
            endif()
            configure_file(${PORT}/sdl/mvc2.rc.in ${CMAKE_BINARY_DIR}/${TARGET}.rc @ONLY)
            target_sources(${TARGET} PRIVATE ${CMAKE_BINARY_DIR}/${TARGET}.rc)
            set_property(SOURCE ${CMAKE_BINARY_DIR}/${TARGET}.rc APPEND PROPERTY OBJECT_DEPENDS ${RC_ICON})
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
            # libgcc's division helpers, through port/sys/divzero_arm.c
            target_link_options(${TARGET} PRIVATE
                -Wl,--wrap=__aeabi_idiv,--wrap=__aeabi_uidiv,--wrap=__aeabi_idivmod,--wrap=__aeabi_uidivmod)
        endif()
        if(LINUX_X86 AND MVC2_SDL3)
            # SDL beside the executables
            set_target_properties(${TARGET} PROPERTIES INSTALL_RPATH "$ORIGIN")
        endif()
    endif()
endfunction()

# both programs, the arcade's unless the command line says --dc
if(PSP)
    mvc2_executable(mvc2dc dc)
else()
    mvc2_executable(mvc2 arc dc)
endif()
# the Dreamcast alone, while its host layer is young
if(NOT NINTENDO_3DS AND NOT PSP)
    mvc2_executable(mvc2dc dc)
endif()

# cmake --install BUILD --prefix FOLDER: the executables, SDL's DLL (on x86 Linux its .so), PLAYING.md and
# mvc2.cfg (unless there is one)
if(WIN32)
    get_filename_component(TOOL_BIN ${CMAKE_C_COMPILER} DIRECTORY)
    set(DLL_DIRS ${TOOL_BIN})
    if(TARGET SDL3::SDL3-shared)
        get_target_property(SDL_DLL SDL3::SDL3-shared LOCATION)
        get_filename_component(SDL_BIN ${SDL_DLL} DIRECTORY)
        list(PREPEND DLL_DIRS ${SDL_BIN})
    endif()
    install(TARGETS mvc2 mvc2dc
            RUNTIME_DEPENDENCIES
                DIRECTORIES ${DLL_DIRS}
                PRE_EXCLUDE_REGEXES "^api-ms-" "^ext-ms-"
                POST_EXCLUDE_REGEXES "[Ss][Yy][Ss][Tt][Ee][Mm]32" "[Ss][Yy][Ss][Ww][Oo][Ww]64"
            RUNTIME DESTINATION .)
elseif(NOT NINTENDO_3DS AND NOT PSP)
    install(TARGETS mvc2 mvc2dc RUNTIME DESTINATION .)
    if(LINUX_X86 AND TARGET SDL3::SDL3-shared)
        get_target_property(SDL_CONFIGS SDL3::SDL3-shared IMPORTED_CONFIGURATIONS)
        list(GET SDL_CONFIGS 0 SDL_CONFIG)
        get_target_property(SDL_SO SDL3::SDL3-shared IMPORTED_LOCATION_${SDL_CONFIG})
        get_target_property(SDL_SONAME SDL3::SDL3-shared IMPORTED_SONAME_${SDL_CONFIG})
        install(FILES ${SDL_SO} DESTINATION . RENAME ${SDL_SONAME})
    endif()
endif()
if(NOT NINTENDO_3DS AND NOT PSP)
    install(FILES ${PORT}/dist/PLAYING.md DESTINATION .)
    install(CODE "if(NOT EXISTS \"\${CMAKE_INSTALL_PREFIX}/mvc2.cfg\")
        file(INSTALL \"${PORT}/dist/mvc2.cfg\" DESTINATION \"\${CMAKE_INSTALL_PREFIX}\")
    endif()")
endif()

# the Media Engine test (port/psp/me_test.c): its own EBOOT, in me/, for a test folder of its own
if(PSP)
    add_executable(mvc2me ${PORT}/psp/me_test.c ${PORT}/psp/psp_me.c)
    target_compile_options(mvc2me PRIVATE -O2 -Wall)
    target_link_libraries(mvc2me PRIVATE pspdebug pspdisplay pspge pspctrl psppower pspkubridge pspsystemctrl_user)
    create_pbp_file(TARGET mvc2me TITLE "MVC2 TEST ME spike 7" VERSION 01.00 MEMSIZE 1 OUTPUT_DIR ${CMAKE_BINARY_DIR}/me)
endif()
