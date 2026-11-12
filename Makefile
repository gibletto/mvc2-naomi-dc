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

# Capcom's options, from the response file left in the ROM padding (README.md), and the program's
# own date, which the source asks for where a later build differs
COPTS = -cpu=sh4 -endian=little -fpu=single -round=nearest -macsave=0 -pic=0 -optimize=1 -size \
	-string=const -extra=a=400 -define=_STRICT_UNION_,__DEV_TYPE_DC__,BUILD_DATE=20000225
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
	"$(SHC)/lnk.exe" -subcommand=mvc2.sub >build/lnk.log 2>&1; r=$$?; grep '^[*][*]' build/lnk.log; [ $$r = 0 ]

build/mvc2.bin: build/mvc2.abs
	rm -f $@; cd build && "$(SHC)/elf2bin.exe" -b ff -s 0C020000 mvc2.abs mvc2.bin >mvc2.log 2>&1

# the test program, linked like the main one (mvc2test.sub)
build/mvc2test.abs: $(OBJS) $(TESTOBJS) build/sdk/libs mvc2test.sub
	"$(SHC)/lnk.exe" -subcommand=mvc2test.sub >build/lnktest.log 2>&1; r=$$?; grep '^[*][*]' build/lnktest.log; [ $$r = 0 ]

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

# The Dreamcast program, 1ST_READ.BIN of the USA disc (make dc). SDK_LIBS has two more folders
# (README.md). Each object that mvc2dc.sub links is compiled from the first of src/dc, src,
# src/dc/lib and src/lib that has it, with the switches TARGET_DC and the disc's BUILD_DATE.
# It also builds the characters' files (dcovl, below).
comma := ,
DCOPTS = $(subst BUILD_DATE=20000225,TARGET_DC$(comma)BUILD_DATE=20000529,$(COPTS))
DC_SHA1 = b0c3604e3494b12f751793a74fc9b494ed67a30c
DCNAMES = $(shell tr -d '\r' <mvc2dc.sub | sed -n 's/^input build.obj.dc.\(.*\)\.obj$$/\1/p')
DCOBJS = $(patsubst %,build/obj/dc/%.obj,$(DCNAMES))
dcsrc = $(firstword $(wildcard src/dc/$1.c src/dc/$1.src src/$1.c src/$1.src src/dc/lib/*/$1.c \
	src/dc/lib/*/$1.src src/lib/*/$1.c src/lib/*/$1.src))
# a library module's options: the disc's for it in src/dc/lib, its own, its folder's, or else its library's in src/lib
dcopts = $(or $(wildcard src/dc/lib/$(notdir $(patsubst %/,%,$(dir $1)))/$(notdir $(1:.c=.options))),$(wildcard $(1:.c=.options)),$(wildcard $(dir $1)options),src/lib/$(notdir $(patsubst %/,%,$(dir $1)))/options)
sega = $(filter src/lib/% src/dc/lib/%,$1)$(wildcard $(1:.c=.options))

define DCMODULE
build/obj/dc/$1.obj: $2 $(if $(filter %.c,$2),$(wildcard include/*.h $(dir $2)*.h)) | build/obj/dc
	@echo $2
ifneq ($(filter %.src,$2),)
	@"$$(SHC)/asmsh.exe" $$(call win,$2) -object='build\obj\dc\$1.obj' $$(AFLAGS) >build/obj/dc/$1.log; r=$$$$?; \
		grep '(E)\|(W)' build/obj/dc/$1.log; rm -f build/obj/dc/$1.log; [ $$$$r = 0 ]
else ifneq ($(call sega,$2),)
	@mkdir -p build/tmp/dc_$1; SHC_TMP='build\tmp\dc_$1' "$$(SHC)/shc.exe" $$(call win,$2) $$(shell cat $(call dcopts,$2)) \
		-include=$$(call win,$(dir $2)),$(if $(wildcard src/lib/$(notdir $(patsubst %/,%,$(dir $2)))),$$(call win,src/lib/$(notdir $(patsubst %/,%,$(dir $2))))$$(comma))include,'build\orig' \
		$(if $(shell grep -l 'pragma inline_asm' $2),-code=asmcode -object='build\tmp\dc_$1\$1.src',-object='build\obj\dc\$1.obj') \
		>build/obj/dc/$1.log 2>&1; r=$$$$?; \
		$(if $(shell grep -l 'pragma inline_asm' $2),[ $$$$r = 0 ] && { "$$(SHC)/asmsh.exe" 'build\tmp\dc_$1\$1.src' -object='build\obj\dc\$1.obj' $$(AFLAGS) >>build/obj/dc/$1.log; r=$$$$?; };) \
		rm -rf build/tmp/dc_$1; grep ') : \|(E)' build/obj/dc/$1.log; rm -f build/obj/dc/$1.log; [ $$$$r = 0 ]
else
	@mkdir -p build/tmp/dc_$1; SHC_TMP='build\tmp\dc_$1' "$$(SHC)/shc.exe" $$(call win,$2) $$(DCOPTS) \
		-include=$(if $(filter src/dc/%,$2),'src\dc'$$(comma))include -object='build\obj\dc\$1.obj' >build/obj/dc/$1.log 2>&1; \
		r=$$$$?; rm -rf build/tmp/dc_$1; grep ') : ' build/obj/dc/$1.log; rm -f build/obj/dc/$1.log; [ $$$$r = 0 ]
endif
endef
ifneq ($(filter dc dcovl build/mvc2dc.% build/ovl/%,$(MAKECMDGOALS)),)
$(foreach n,$(DCNAMES),$(eval $(call DCMODULE,$n,$(call dcsrc,$n))))
endif

build/obj/dc:
	mkdir -p build/obj/dc

# the libraries the Dreamcast link takes, as build/sdk/libs has the arcade's
build/sdk/dclibs: mvc2dc.sub
	@n=0; printf '%s\n' '$(SDK_LIBS)' | tr ';' '\n' | while read -r d; do n=$$((n + 1)); mkdir -p build/sdk/$$n; \
		for f in $$(grep -oh "sdk.$$n.[a-z0-9_]*[.][a-z]*" mvc2dc.sub | sort -u | sed 's/sdk.[0-9]*.//'); do \
			cp "$$d/$$f" build/sdk/$$n/ || exit 1; done; done
	@touch $@

build/mvc2dc.abs: $(DCOBJS) build/sdk/dclibs mvc2dc.sub
	"$(SHC)/lnk.exe" -subcommand=mvc2dc.sub >build/lnkdc.log 2>&1; r=$$?; grep '^[*][*]' build/lnkdc.log; [ $$r = 0 ]
build/mvc2dc.FSY: build/mvc2dc.abs

# the Dreamcast fills the gaps with 0
build/mvc2dc.bin: build/mvc2dc.abs
	rm -f $@; cd build && "$(SHC)/elf2bin.exe" -b 00 -s 8C010000 mvc2dc.abs mvc2dc.bin >mvc2dc.log 2>&1

dc: build/mvc2dc.bin
	@echo "$(DC_SHA1) *build/mvc2dc.bin" | sha1sum -c - >/dev/null 2>&1 \
		&& echo "build/mvc2dc.bin is the disc's 1ST_READ.BIN: SHA-1 $(DC_SHA1)" \
		|| { echo "build/mvc2dc.bin is not the disc's 1ST_READ.BIN"; exit 1; }
	@$(MAKE) --no-print-directory dcovl

# The characters' files, S_PLxx[A-F].BIN (make dcovl). A character's module, src/s_plXX.c compiled
# for the Dreamcast, is linked six times, at 0CE30000 + 0x8000 for each slot (A to F), beside the
# main program's names: the linker's symbol file of the main program (build/mvc2dc.FSY) is assembler
# source, and asmsh assembles it as it is. dcovl.sha1 has the disc's SHA-1s.
DCOVLMODS = $(patsubst src/%.c,%,$(wildcard src/s_pl[0-9a-f][0-9a-f].c))

build/ovl:
	mkdir -p build/ovl

build/ovl/%.obj: src/%.c $(wildcard include/*.h) | build/ovl
	@echo $<
	@mkdir -p build/tmp/ovl_$*; SHC_TMP='build\tmp\ovl_$*' "$(SHC)/shc.exe" $(call win,$<) $(DCOPTS) -include=include \
		-object='build\ovl\$*.obj' >build/ovl/$*.log 2>&1; r=$$?; rm -rf build/tmp/ovl_$*; \
		grep ') : ' build/ovl/$*.log; rm -f build/ovl/$*.log; [ $$r = 0 ]

build/ovl/dcsyms.obj: build/mvc2dc.FSY | build/ovl
	"$(SHC)/asmsh.exe" 'build\mvc2dc.FSY' -object='build\ovl\dcsyms.obj' $(AFLAGS) >build/ovl/dcsyms.log 2>&1; r=$$?; \
		grep '(E)' build/ovl/dcsyms.log; rm -f build/ovl/dcsyms.log; [ $$r = 0 ]

# one module's six files; a module with no constants or no work has no C or B to place (lnk's 121)
define DCOVL
build/ovl/$(shell echo $1 | tr a-z A-Z).done: build/ovl/$1.obj build/ovl/dcsyms.obj
	@for n in 0 1 2 3 4 5; do f=S_PL$(shell echo $1 | cut -c5- | tr a-z A-Z)$$$$(echo ABCDEF | cut -c$$$$((n + 1))); \
		a=$$$$(printf %08X $$$$((0x0CE30000 + 0x8000 * n))); rm -f build/ovl/$$$$f.abs build/ovl/$$$$f.BIN; \
		"$(SHC)/lnk.exe" 'build\ovl\$1.obj','build\ovl\dcsyms.obj' -output="build\\ovl\\$$$$f.abs" -elf -form=a \
			"-start=P,C,D,B($$$$a)" -udfcheck >build/ovl/$$$$f.log 2>&1; grep '^[*][*]' build/ovl/$$$$f.log | grep -v ' 121 ' && exit 1; \
		"$(SHC)/elf2bin.exe" "build\\ovl\\$$$$f.abs" "build\\ovl\\$$$$f.BIN" >/dev/null 2>&1 || exit 1; \
		rm -f build/ovl/$$$$f.abs build/ovl/$$$$f.log; done
	@touch $$@
endef
$(foreach m,$(DCOVLMODS),$(eval $(call DCOVL,$m)))

dcovl: $(foreach m,$(DCOVLMODS),build/ovl/$(shell echo $m | tr a-z A-Z).done)
	@sha1sum -c dcovl.sha1 >build/ovl/check.log 2>&1 \
		&& echo "the $$(wc -l <dcovl.sha1) characters' files are the disc's" \
		|| { grep -v ': OK$$' build/ovl/check.log; exit 1; }

clean:
	rm -rf build/obj build/data build/chardata.bin build/sdk/libs build/sdk/[1-9] build/tmp build/mvc2.abs build/mvc2.map \
		build/mvc2.bin build/lnk.log build/mvc2test.abs build/mvc2test.map build/mvc2test.bin \
		build/lnktest.log build/mvc2.log build/test?.bin build/tail.bin build/fill.* $(FROM_ROM) $(ROM) \
		build/sdk/dclibs build/linkdc build/mvc2dc.abs build/mvc2dc.map build/mvc2dc.bin build/mvc2dc.FSY build/mvc2dc.log build/lnkdc.log build/ovl

.PHONY: all clean rom dc dcovl
