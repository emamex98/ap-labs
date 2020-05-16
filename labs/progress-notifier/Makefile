
include ../../common.mk

all:
	gcc -c base64.c -o base.o
	gcc -c logger.c -o logger.o
	gcc base.o logger.o -o base64

remove:
	rm logger.o
	rm base.o
	rm base64
	clear 