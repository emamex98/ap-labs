#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int printColoured(const char *format, char *colour){

	if(strlen(format) == 0){
		return -1;
	}

	write(1, colour, strlen(colour));
	write(1, format, strlen(format));
	write(1, "\033[m\n", strlen("\033[m\n"));

	return 0;
}

int infof(const char *format){
        return printColoured(format, "\033[32m");
}

int warnf(const char *format){
        return printColoured(format, "\033[93m");
}

int errorf(const char *format){
        return printColoured(format, "\033[31m");
}

int panicf(const char *format){
        return printColoured(format, "\033[30;41m");
}
