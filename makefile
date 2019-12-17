all:
	make -f wattsfarmer.mk
	make -f spamA.mk
	make -f masterballs.mk
	# make -f shinyhunt.mk

watts:
	make -f wattsfarmer.mk

dig:
	make -f spamA.mk

balls:
	make -f masterballs.mk

# shiny:
# 	make -f shinyhunt.mk