EGG_CYCLES = 20
MAX_HATCHES = 240

all: watts repeat-a balls wildarea release

remake: cleaner all

hexdir:
	mkdir -p hex

watts: hexdir
	make -f wattsfarmer.mk
	mv wattsfarmer.hex hex

repeat-a: hexdir
	make -f repeat-a.mk
	mv repeat-a.hex hex

balls: hexdir
	make -f masterballs.mk
	mv masterballs.hex hex

wildarea: hexdir
	make -e -f wildareabreeding.mk 
	mv wildareabreeding.hex hex

release: hexdir
	make -f releasebox.mk
	mv releasebox.hex hex

clean:
	rm -f *.bin *.eep *.elf *.lss *.map *.sym

cleaner: clean
	rm -rf hex obj

.PHONY: all remake clean cleaner
