# Marvel vs. Capcom 2 (NAOMI) - the program ROM, epr-23085a.ic11 (same steps as build.bat)
# Set SHC to the bin folder of Hitachi SHC 5.1 Release 08 and SDK_LIBS to the SDK's library
# folders (see README.md, Building).
PYTHON ?= python
ifndef SHC
$(error set SHC to the folder of SHC 5.1 Release 08)
endif
ifndef SDK_LIBS
$(error set SDK_LIBS to the SDK library folders, separated by ;)
endif
export SHC_LIB = $(SHC)

# Capcom's options, from the response file left in the ROM padding (README.md)
COPTS = -cpu=sh4 -endian=little -fpu=single -round=nearest -macsave=0 -pic=0 -optimize=1 -size \
	-string=const -extra=a=400 -define=_STRICT_UNION_,__DEV_TYPE_DC__
CFLAGS = $(COPTS) -include=include
AFLAGS = -cpu=sh4 -endian=little

# the link order is mvc2.sub's
SRCS = $(wildcard src/*.c src/*.src)
# modules of Sega's libraries, each library with its options in src/lib/LIBRARY/options
# (a module's own src/lib/LIBRARY/MODULE.options replaces them)
LIBSRCS = $(wildcard src/lib/*/*.c)
LIBASMS = $(wildcard src/lib/*/*.src)
OBJS = $(patsubst src/%,build/obj/%.obj,$(basename $(SRCS))) \
	$(patsubst %,build/obj/%.obj,$(basename $(notdir $(LIBSRCS) $(LIBASMS))))
# the tools take Windows paths (asmsh reads / as an option)
win = '$(subst /,\,$1)'

all: rom

# what the source does not have, cut from your own ROM in rom/
FROM_ROM = build/orig/data_340000.bin build/orig/hole_3E6C00.bin build/orig/hole_3E9400.bin \
	build/orig/hole_3F5C00.bin build/orig/nla6149.h
build/orig/data_340000.bin:
	$(PYTHON) tools/mvc2rom.py rom build/orig
$(filter-out build/orig/data_340000.bin,$(FROM_ROM)): build/orig/data_340000.bin
build/obj/nlam.obj: build/orig/nla6149.h

# each compile has its own folder for SHC's temporary files, so make -j works
build/obj/%.obj: src/%.c | build/obj
	@echo $<
	@mkdir -p build/tmp/$*; SHC_TMP='build\tmp\$*' "$(SHC)/shc.exe" $(call win,$<) $(CFLAGS) \
		-object=$(call win,$@) >$(@:.obj=.log) 2>&1; r=$$?; rm -rf build/tmp/$*; \
		grep ') : ' $(@:.obj=.log); rm -f $(@:.obj=.log); [ $$r = 0 ]

define LIBRARY
build/obj/$(basename $(notdir $1)).obj: $1 $(or $(wildcard $(1:.c=.options)),$(dir $1)options) $(wildcard $(dir $1)*.h) | build/obj
	@echo $$<
	@mkdir -p build/tmp/$2; SHC_TMP='build\tmp\$2' "$$(SHC)/shc.exe" $$(call win,$$<) $$(shell cat $(or $(wildcard $(1:.c=.options)),$(dir $1)options)) \
		-include=$$(call win,$(dir $1)),include,'build\orig' $(if $3,-code=asmcode -object='build\tmp\$2\$2.src',-object=$$(call win,$$@)) \
		>$$(@:.obj=.log) 2>&1; r=$$$$?; \
		$(if $3,[ $$$$r = 0 ] && { "$$(SHC)/asmsh.exe" 'build\tmp\$2\$2.src' -object=$$(call win,$$@) $$(AFLAGS) >>$$(@:.obj=.log); r=$$$$?; };) \
		rm -rf build/tmp/$2; grep ') : \|(E)' $$(@:.obj=.log); rm -f $$(@:.obj=.log); [ $$$$r = 0 ]
endef
# a module with inline assembler (the third argument) is compiled to assembler source and assembled,
# as Sega's makefiles did
$(foreach s,$(LIBSRCS),$(eval $(call LIBRARY,$s,$(basename $(notdir $s)),$(shell grep -l 'pragma inline_asm' $s))))

# Sega's assembler modules
vpath %.src $(sort $(dir $(LIBASMS)))
build/obj/%.obj: %.src | build/obj
	@echo $<
	@"$(SHC)/asmsh.exe" $(call win,$<) -object=$(call win,$@) $(AFLAGS) >$(@:.obj=.log); r=$$?; \
		grep '(E)\|(W)' $(@:.obj=.log); rm -f $(@:.obj=.log); [ $$r = 0 ]

build/obj/%.obj: src/%.src | build/obj
	@echo $<
	@"$(SHC)/asmsh.exe" $(call win,$<) -object=$(call win,$@) $(AFLAGS) >$(@:.obj=.log); r=$$?; \
		grep '(E)\|(W)' $(@:.obj=.log); rm -f $(@:.obj=.log); [ $$r = 0 ]

# the test program's own modules (src/test); the rest of it is main's start-up and work, and Sega's
TESTSRCS = $(wildcard src/test/*.c src/test/*.src)
TESTOBJS = $(patsubst src/test/%,build/obj/test/%.obj,$(basename $(TESTSRCS)))

build/obj/test/%.obj: src/test/%.c | build/obj/test
	@echo $<
	@mkdir -p build/tmp/test_$*; SHC_TMP='build\tmp\test_$*' "$(SHC)/shc.exe" $(call win,$<) $(COPTS) \
		-include='src\test',include -object=$(call win,$@) >$(@:.obj=.log) 2>&1; r=$$?; rm -rf build/tmp/test_$*; \
		grep ') : ' $(@:.obj=.log); rm -f $(@:.obj=.log); [ $$r = 0 ]

build/obj/test/%.obj: src/test/%.src | build/obj/test
	@echo $<
	@"$(SHC)/asmsh.exe" $(call win,$<) -object=$(call win,$@) $(AFLAGS) >$(@:.obj=.log); r=$$?; \
		grep '(E)\|(W)' $(@:.obj=.log); rm -f $(@:.obj=.log); [ $$r = 0 ]

# shc writes no dependency lists, so a header change rebuilds everything
$(OBJS) $(TESTOBJS): $(wildcard include/*.h)
$(TESTOBJS): $(wildcard src/test/*.h)

build/obj:
	mkdir -p build/obj

build/obj/test:
	mkdir -p build/obj/test

# the libraries the links take modules from: build/sdk/N has those of the Nth folder of SDK_LIBS
build/sdk/libs: mvc2.sub mvc2test.sub
	@n=0; printf '%s\n' '$(SDK_LIBS)' | tr ';' '\n' | while read -r d; do n=$$((n + 1)); mkdir -p build/sdk/$$n; \
		for f in $$(grep -oh "sdk.$$n.[a-z0-9_]*[.][a-z]*" mvc2.sub mvc2test.sub | sort -u | sed 's/sdk.[0-9]*.//'); do \
			cp "$$d/$$f" build/sdk/$$n/ || exit 1; done; done
	@touch $@

build/mvc2.abs: $(OBJS) build/sdk/libs mvc2.sub
	"$(SHC)/lnk.exe" -subcommand=mvc2.sub >build/lnk.log 2>&1; r=$$?; grep '^\*\*' build/lnk.log; [ $$r = 0 ]

build/mvc2.bin: build/mvc2.abs
	rm -f $@; cd build && "$(SHC)/elf2bin.exe" -b ff -s 0C020000 mvc2.abs mvc2.bin >mvc2.log 2>&1

# the test program, linked like the main one (mvc2test.sub)
build/mvc2test.abs: $(OBJS) $(TESTOBJS) build/sdk/libs mvc2test.sub
	"$(SHC)/lnk.exe" -subcommand=mvc2test.sub >build/lnktest.log 2>&1; r=$$?; grep '^\*\*' build/lnktest.log; [ $$r = 0 ]

# the characters' data (data/*.c): each file is a part of a character's pack, linked alone at 0
# (its tables hold offsets from its own start); tools/chrpack.py packs them as the ROM has them
DATASRCS = $(wildcard data/*.c)
DATABINS = $(patsubst data/%.c,build/data/%.bin,$(DATASRCS))

build/data/%.bin: data/%.c $(wildcard include/*.h) | build/data
	@echo $<
	@mkdir -p build/tmp/data_$*; SHC_TMP='build\tmp\data_$*' "$(SHC)/shc.exe" $(call win,$<) $(CFLAGS) \
		-object='build\data\$*.obj' >build/data/$*.log 2>&1; r=$$?; rm -rf build/tmp/data_$*; \
		grep ') : ' build/data/$*.log; [ $$r = 0 ] && \
		printf 'input build\\data\\$*.obj\r\noutput build\\data\\$*.abs\r\nelf\r\nform a\r\nstart C(0)\r\nexit\r\n' >build/data/$*.sub && \
		"$(SHC)/lnk.exe" -subcommand='build\data\$*.sub' >build/data/$*.log 2>&1 && rm -f $@ && \
		(cd build/data && "$(SHC)/elf2bin.exe" $*.abs $*.bin >$*.log 2>&1) && rm -f build/data/$*.log build/data/$*.sub

build/data:
	mkdir -p build/data

build/chardata.bin: $(DATABINS) tools/chrpack.py
	$(PYTHON) tools/chrpack.py build/data $@

# the whole program ROM, build/epr-23085a.ic11. After the main program comes 0xFF up to ROM
# 0x250000, then the characters' data, then seven model files that the game never reads (from your
# ROM), then the test program at 0x3C0000 and 0xFF to the end. Each 0xFF is cut to length from a
# link of nothing else. The test program's image is cut in four, with a hole after each of the
# first three: left-overs of older builds that no build gives, also from your ROM.
ROM = build/epr-23085a.ic11
ROM_SHA1 = f58c31b245fc33fa541f9f074548402a63f7c3d3
ROM_DATA = build/chardata.bin build/orig/data_340000.bin
HOLES = build/orig/hole_3E6C00.bin build/orig/hole_3E9400.bin build/orig/hole_3F5C00.bin
ELF2BIN = "$(SHC)/elf2bin.exe"

build/fill.abs: | build/obj
	printf "          .SECTION  FILL,DATA,ALIGN=4\r\n          .DATAB.B  H'10000,H'FF\r\n          .END\r\n" >build/fill.src
	printf 'input fill.obj\r\noutput fill.abs\r\nelf\r\nform a\r\nstart FILL(0)\r\nexit\r\n' >build/fill.sub
	cd build && "$(SHC)/asmsh.exe" fill.src -object=fill.obj $(AFLAGS) >fill.log && "$(SHC)/lnk.exe" -subcommand=fill.sub >>fill.log

build/mvc2test.bin: build/mvc2test.abs $(HOLES)
	cd build && rm -f test1.bin test2.bin test3.bin test4.bin && \
		$(ELF2BIN) -b ff -s 0C020000 -e 0C046C00 mvc2test.abs test1.bin >fill.log 2>&1 && \
		$(ELF2BIN) -b ff -s 0C048C00 -e 0C049400 mvc2test.abs test2.bin >>fill.log 2>&1 && \
		$(ELF2BIN) -b ff -s 0C04B000 -e 0C055C00 mvc2test.abs test3.bin >>fill.log 2>&1 && \
		$(ELF2BIN) -b ff -s 0C056000 mvc2test.abs test4.bin >>fill.log 2>&1
	cat build/test1.bin build/orig/hole_3E6C00.bin build/test2.bin build/orig/hole_3E9400.bin \
		build/test3.bin build/orig/hole_3F5C00.bin build/test4.bin >$@

$(ROM): build/mvc2.bin build/mvc2test.bin build/fill.abs $(ROM_DATA)
	cd build && rm -f fill.bin tail.bin && \
		$(ELF2BIN) -s 0 -e $$(printf %X $$((0x250000 - $$(wc -c <mvc2.bin)))) fill.abs fill.bin >>fill.log 2>&1 && \
		$(ELF2BIN) -s 0 -e $$(printf %X $$((0x40000 - $$(wc -c <mvc2test.bin)))) fill.abs tail.bin >>fill.log 2>&1
	cat build/mvc2.bin build/fill.bin $(ROM_DATA) build/mvc2test.bin build/tail.bin >$@

rom: $(ROM)
	@echo "$(ROM_SHA1) *$(ROM)" | sha1sum -c - >/dev/null 2>&1 && echo "$(ROM) is the arcade's: SHA-1 $(ROM_SHA1)" \
		|| { echo "$(ROM) is not the arcade's"; exit 1; }

clean:
	rm -rf build/obj build/data build/chardata.bin build/sdk/libs build/sdk/[1-9] build/tmp build/mvc2.abs build/mvc2.map \
		build/mvc2.bin build/lnk.log build/mvc2test.abs build/mvc2test.map build/mvc2test.bin \
		build/lnktest.log build/mvc2.log build/test?.bin build/tail.bin build/fill.* $(FROM_ROM) $(ROM)

.PHONY: all clean rom
