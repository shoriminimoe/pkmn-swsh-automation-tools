all:  watts dig balls wildarea autotrader

watts:
	make -f wattsfarmer.mk
	mkdir -p hex
	mv wattsfarmer.hex hex

dig:
	make -f spamA.mk
	mkdir -p hex
	mv spamA.hex hex

balls:
	make -f masterballs.mk
	mkdir -p hex
	mv masterballs.hex hex

wildarea:
	make -f wildareabreeding.mk
	mkdir -p hex
	mv wildareabreeding.hex hex

autotrader:
	make -f autotrader.mk
	mkdir -p hex
	mv autotrader.hex hex

clean:
	rm -f *.bin *.eep *.elf *.lss *.map *.sym hex/*
