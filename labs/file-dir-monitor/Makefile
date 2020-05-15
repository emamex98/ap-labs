include ../../common.mk

all:
	gcc -c monitor.c -o premonitor.o
	gcc -c logger.c -o logger.o
	gcc premonitor.o logger.o -o monitor

remove:
	rm logger.o
	rm premonitor.o
	rm monitor
	clear