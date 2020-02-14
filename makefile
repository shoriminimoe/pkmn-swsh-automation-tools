all: watts repeat-a balls wildarea release

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
	make -f wildareabreeding.mk
	mv wildareabreeding.hex hex

release: hexdir
	make -f releasebox.mk
	mv releasebox.hex hex

clean:
	rm -f *.bin *.eep *.elf *.lss *.map *.sym

cleaner: clean
	rm -rf hex
