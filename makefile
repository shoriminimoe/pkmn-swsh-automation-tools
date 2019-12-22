all:
	make -f wattsfarmer.mk
	make -f repeat-a.mk
	make -f masterballs.mk
	make -f wildareabreeding.mk
	make clean

watts:
	make -f wattsfarmer.mk
	make clean

repeat-a:
	make -f repeat-a.mk
	make clean

balls:
	make -f masterballs.mk
	make clean

wildarea:
	make -f wildareabreeding.mk
	make clean

clean:
	rm -f *.bin *.eep *.elf *.lss *.map *.sym