all:
	make -f wattsfarmer.mk
	make -f spamA.mk
	make -f masterballs.mk
	make -f wildareabreeding.mk
	make clean

watts:
	make -f wattsfarmer.mk
	make clean

dig:
	make -f spamA.mk
	make clean

balls:
	make -f masterballs.mk
	make clean

wildarea:
	make -f wildareabreeding.mk
	make clean

clean:
	rm -f *.bin *.eep *.elf *.lss *.map *.sym