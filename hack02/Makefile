#   Makefile - Make script
#
#   Copyright (c) 2001 - Monaco F. J. 
#
#   This file is part of SYSeg.
#
#   SYSeg is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.



init: all 


# Update Makefile from Makefile.m4 if needed, and then invoke make again.
# If the source is from a pack-distribution, the lack of Makefile.m4
# inhibits the updating. 

ifndef UPDATED
#$(MAKECMDGOALS) : Makefile #### I removed this May 10, 2023.

Makefile_deps = Makefile.m4 TOOL_PATH/docm4.m4 TOOL_PATH/bintools.m4 TOOL_PATH/Makegyver.mk

Makefile : $(shell if test -f Makefile.m4; then echo $(Makefile_deps); fi);
	@if ! test -f .dist; then\
	  cd .. && make;\
	  make -f Makefile clean;\
	  make -f Makefile UPDATED=1 $(MAKECMDGOAL);\
	fi

endif


##
## Relevant rules
##

bin =  eg-00 eg-00a eg-01 eg-01 eg-02 eg-03 eg-04 eg-04a eg-05 eg-06 eg-07 eg-08 eg-09 # eg-10 eg-11

all: $(bin)

eg-00 : eg-00.c
	gcc -Wall -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-stack-protector -fno-pie -fno-pic $<  -o $@

eg-00a : eg-00a.c
	gcc -Wall -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-pic $<  -o $@

eg-01 : eg-01.c
	gcc -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-stack-protector -mpreferred-stack-boundary=2 $<  -o $@

eg-02 : eg-02.c
	gcc -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-stack-protector -mpreferred-stack-boundary=2 $<  -o $@

eg-03 : eg-03.c
	gcc -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-stack-protector  $<  -o $@

eg-04 : eg-04.c
	gcc -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-stack-protector  $<  -o $@

eg-04a : eg-04a.c
	gcc -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-stack-protector  $<  -o $@

eg-05 : eg-05.c
	gcc -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -no-pie $<  -o $@

eg-06 : eg-06.c
	gcc -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -no-pie $<  -o $@

eg-07 : eg-07.c
	gcc -Wall -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-pic -no-pie $<  -o $@

eg-08 : eg-08.c
	gcc -Wall -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-pie -fno-pic -no-pie $<  -o $@

eg-09 : eg-09.c 
	gcc -Wall -g -m32 -Wall -Wno-unused -O0 -fcf-protection=none -fno-stack-protector -mpreferred-stack-boundary=2 -fno-pie -fno-pic -z execstack -Wno-deprecated-declarations --ansi -no-pie $<  -o $@

eg-09.in : eg-09.bin eg-09.sh
	./eg-09.sh $<

eg-09.bin : eg-09.S
	as --32 $< -o $@.o
	ld -melf_i386 --oformat=binary -Ttext=0x0 $@.o -o $@

eg-09.elf : eg-09.S
	as --32 $< -o $@.o
	ld -melf_i386  $@.o -o $@




%/main: %
	objdump -m i386 --disassemble=main $< | sed  's/:\(\t.*\t\)/:    /g'



.PHONY: clean

clean :
	rm -f $(bin) *.o *.in *.bin *.elf

# ------------------------------------------------------------
# The following excerpt of code was copied from MakeBintools,
# part of SYSeg, Copyright 2001 Monaco F. J..
# MakeBintools is a collection of handy 'GNU make' rules for
# inspecting and comparing the contents of source and object files.
# Further information: http://gitlab.com/monaco/syseg


##
## Configuration
##


# Inform your preferred graphical diff tool e.g meld, kdiff3 etc.

DIFF_TOOL=meld


##
## You probably don't need to change beyond this line
##

# Disassemble

# ALT = intel | att  (default: att)

ifndef ASM
ifeq (,$(findstring intel, $(MAKECMDGOALS)))
ASM_SYNTAX = att
else
ASM_SYNTAX = intel
endif
else
ASM_SYNTAX= $(ASM)
endif

# BIT = 16 | 32  (default: 32)

ifndef BIT
ifeq (,$(findstring 16, $(MAKECMDGOALS)))
ASM_MACHINE = i386
else
ASM_MACHINE = i8086
endif
else

ifeq ($(BIT),16)
ASM_MACHINE = i8086
else
ASM_MACHINE = i386
endif

endif


intel att 16 32: 
	@echo > /dev/null

##
## Options
##

opts = $(filter .optnop, $(MAKECMDGOALS))
symbol = $(filter ..%, $(MAKECMDGOALS))

$(opts) $(symbol):
	@echo > /dev/null

objdump_nop = "cat"
objdump_disassemble = "-d"

#
# Disassemble options
#

ifneq (,$(filter d diss d* diss*, $(MAKECMDGOALS)))


ifneq (,$(findstring .optnop, $(opts)))
objdump_nop = "sed 's/:\(\t.*\t\)/:    /g'"
endif

ifneq (,$(symbol))
objdump_disassemble = "--disassemble=$(symbol:..%=%)"
endif

endif


#
# Disassemble
#

diss d diss* d* : baz=$(bar)

diss d diss* d*: $(IMG) 
	echo objdump -f $< > /dev/null 2>&1; \
	if test $$? -eq 1   ; then \
	  objdump -M $(ASM_SYNTAX) -b binary -m $(ASM_MACHINE) -D $< | "$(objdump_nop)"; \
	else \
	  if test $@ = "diss" || test $@ = "d" ; then\
	   objdump -M $(ASM_SYNTAX) -m $(ASM_MACHINE) $(objdump_disassemble) $<  | "$(objdump_nop)" ;\
	  else\
	    objdump -M $(ASM_SYNTAX) -m $(ASM_MACHINE) -D $< | "$(objdump_nop)" ; \
	 fi;\
	fi

%/diss %/d %/diss* %/d*: %
	make $(@F) IMG=$< $(filter 16 32 intel att $(opts) $(symbol), $(MAKECMDGOALS)) 

%/i16 %/16i : %
	make --quiet $</diss intel 16 $(opts) $(symbol)
%/i32 %/32i %/i: %
	make --quiet $</diss intel 32 $(opts) $(symbol)
%/a16 %/16a %/16 : %
	make --quiet $</diss att 16 $(opts) $(symbol)
%/a32 %/32a %/32 %/a: %
	make --quiet $</diss att 32 $(opts) $(symbol)

%/i16* %/16i* : %
	make --quiet $</diss* intel 16 $(opts) $(symbol)
%/i32* %/32i* %/i*: %
	make --quiet $</diss* intel 32 $(opts) $(symbol)
%/a16* %/16a* %/16* : %
	make --quiet $</diss* att 16 $(opts) $(symbol)
%/a32* %/32a* %/32* %/a*: %
	make --quiet $</diss* att 32 $(opts) $(symbol)

##
## Run on the emulator
##

# 
#
# %/run : %
# 	@i=$< &&\
# 	if test $${i##*.} = "img"; then\
# 	    make run-fd IMG=$<;\
# 	 else\
# 	   if test $${i##*.} = "bin"; then\
# 	     make run-bin IMG=$<;\
# 	    fi;\
# 	fi
#
# %/bin : %
# 	make run-bin IMG=$<
#
# %/fd : %
# 	make run-fd IMG=$<

%/run : %
	make run IMG=$<

run: $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none

# These are deprecate; use %/run, instead.

run-bin: $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none
	@echo "Shortcut run-bin is deprecated: use 'make run' instead."

run-iso: $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none
	@echo "Shortcut run-iso is deprecated: use 'make run' instead."

run-fd : $(IMG)
	qemu-system-i386 -drive format=raw,file=$< -net none
	@echo "Shortcut run-fd is deprecated: use 'make run' instead."



# Dump contents in hexadecimal

hex raw dump: $(IMG)
	hexdump -C $<


%/hex %raw %/dump : %
	make --quiet dump IMG=$< 


# Diff-compare


MISSING_DIFF_TOOL="Can't find $(DIFF_TOOL); please edit syseg/tools/bintools.m4"

# Compare objects

objdiff bindiff : $(wordlist 2, 4, $(MAKECMDGOALS))
	if  test -z $$(which $(DIFF_TOOL)); then echo $(MISSING_DIFF_TOOL); exit 1; fi
	if test $(words $^) -lt 4 ; then\
	  bash -c "$(DIFF_TOOL) <(make $(wordlist 1,1,$^)/diss $(ASM) $(BIT)) <(make $(wordlist 2,2,$^)/diss $(ASM) $(BIT))";\
	else\
	  bash -c "$(DIFF_TOOL) <(make $(wordlist 1,1,$^)/diss $(ASM) $(BIT)) <(make $(wordlist 2,2,$^)/diss $(ASM) $(BIT)) <(make $(wordlist 3,3,$^)/diss $(ASM) $(BIT))";\
	fi

# Compare sources

srcdiff : $(wordlist 2, 4, $(MAKECMDGOALS))
	if  test -z $$(which $(DIFF_TOOL)); then echo $(MISSING_DIFF_TOOL); exit 1; fi
	if test $(words $^) -lt 3 ; then\
	  bash -c "$(DIFF_TOOL) $(wordlist 1,1,$^) $(wordlist 2,2,$^)";\
	else\
	  bash -c "$(DIFF_TOOL) $(wordlist 1,1,$^) $(wordlist 2,2,$^) $(wordlist 3,3,$^)";\
	fi

# Compare hex

hexdiff : $(wordlist 2, 4, $(MAKECMDGOALS))
	if  test -z $$(which $(DIFF_TOOL)); then echo $(MISSING_DIFF_TOOL); exit 1; fi
	if test $(words $^) -lt 4 ; then\
	  bash -c "$(DIFF_TOOL) <(make $(wordlist 1,1,$^)/hex $(ASM) $(BIT)) <(make $(wordlist 2,2,$^)/hex $(ASM) $(BIT))";\
	else\
	  bash -c "$(DIFF_TOOL) <(make $(wordlist 1,1,$^)/hex $(ASM) $(BIT)) <(make $(wordlist 2,2,$^)/hex $(ASM) $(BIT)) <(make $(wordlist 3,3,$^)/hex $(ASM) $(BIT))";\
	fi


# Compare objects and sources

diff : $(word 2, $(MAKECMDGOALS))
	@echo $(wordlist 2, 4, $(MAKECMDGOALS))
	@EXT=$(suffix $<);\
	case $$EXT in \
	.bin | .o)\
		make --quiet objdiff $(wordlist 2, 4, $(MAKECMDGOALS))\
		;;\
	.asm | .S | .s | .i | .c | .h | .hex)\
		make --quiet srcdiff $(wordlist 2, 4, $(MAKECMDGOALS))\
		;;\
	.img )\
		make --quiet hexdiff $(wordlist 2, 4, $(MAKECMDGOALS))\
		;;\
	*)\
		echo "I don't know how to compare filetype $$EXT"\
		;;\
	esac


# Choose between intel|att and 16-bit|32-bot

diffi16 di16 i16:
	make --quiet diff $(wordlist 2, 4, $(MAKECMDGOALS)) ASM=intel BIT=16

diffi32 di32 i32:
	make --quiet diff $(wordlist 2, 4, $(MAKECMDGOALS)) ASM=intel BIT=32

diffa16 da16 a16:
	make --quiet diff $(wordlist 2, 4, $(MAKECMDGOALS)) ASM=att BIT=16

diffa32 da32 a32:
	make --quiet diff $(wordlist 2, 4, $(MAKECMDGOALS)) ASM=att BIT=32

##
## Create bootable USP stick if BIOS needs it
##

%.iso : %.bin
	xorriso -as mkisofs -b $< -o $@ -isohybrid-mbr $< -no-emul-boot -boot-load-size 4 ./

%.img : %.bin
	dd if=/dev/zero of=$@ bs=1024 count=1440
	dd if=$< of=$@ seek=0 conv=notrunc



stick: $(IMG)
	@if test -z "$(DEVICE)"; then \
	echo "*** ATTENTION: make IMG=foo.bin DEVICE=/dev/X"; exit 1; fi 
	dd if=$< of=$(DEVICE)

%/stick: %
	make stick IMG=$* DEVICE=$(wordlist 2, 2, $(MAKECMDGOALS))



.PHONY: clean clean-extra intel att 16 32 diss /diss /i16 /i32 /a16 /a32


# End of MakeBintools.
# -------------------------------------------------------------


