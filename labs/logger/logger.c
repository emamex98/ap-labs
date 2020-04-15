#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdarg.h>


int infof(const char *format, ...){

	if(strlen(format) == 0){
                return -1;
        }

	int i;

	va_list arg;
	va_start (arg, format);

	write(1, "\033[32m", strlen("\033[32m"));

	for(i=0; i<strlen(format); i++) {
		if (format[i] == '%' && i+1 < strlen(format)) {
			if (format[i+1] == 'd') {
				int num  = va_arg(arg, int);
				char *numStr = malloc(sizeof(char)*num);
				sprintf(numStr, "%d", num);
				write(1, numStr, strlen(numStr));
				i++;
			} else if (format[i+1] == 'f') {
				double num  = va_arg(arg, double);
                                char *numStr = malloc(sizeof(char)*num);
                                sprintf(numStr, "%f", num);
                                write(1, numStr, strlen(numStr));
                                i++;
			} else if (format[i+1] == 's') {
				char *str = va_arg(arg, char*);
				write(1, str, strlen(str));
				i++;
			}
		} else {
			write(1, &format[i], sizeof(char));
		}
	}


	va_end(arg);
	write(1, "\033[m\n", strlen("\033[m\n"));

  	return 0;
}

int warnf(const char *format, ...){

	if(strlen(format) == 0){
                return -1;
        }

	int i;

	va_list arg;
	va_start (arg, format);

	write(1, "\033[93m", strlen("\033[93m"));

	for(i=0; i<strlen(format); i++) {
		if (format[i] == '%' && i+1 < strlen(format)) {
			if (format[i+1] == 'd') {
				int num  = va_arg(arg, int);
				char *numStr = malloc(sizeof(char)*num);
				sprintf(numStr, "%d", num);
				write(1, numStr, strlen(numStr));
				i++;
			} else if (format[i+1] == 'f') {
				double num  = va_arg(arg, double);
                                char *numStr = malloc(sizeof(char)*num);
                                sprintf(numStr, "%f", num);
                                write(1, numStr, strlen(numStr));
                                i++;
			} else if (format[i+1] == 's') {
				char *str = va_arg(arg, char*);
				write(1, str, strlen(str));
				i++;
			}
		} else {
			write(1, &format[i], sizeof(char));
		}
	}


	va_end(arg);
	write(1, "\033[m\n", strlen("\033[m\n"));

  	return 0;
}

int errorf(const char *format, ...){

	if(strlen(format) == 0){
                return -1;
        }

	int i;

	va_list arg;
	va_start (arg, format);

	write(1, "\033[31m", strlen("\033[31m"));

	for(i=0; i<strlen(format); i++) {
		if (format[i] == '%' && i+1 < strlen(format)) {
			if (format[i+1] == 'd') {
				int num  = va_arg(arg, int);
				char *numStr = malloc(sizeof(char)*num);
				sprintf(numStr, "%d", num);
				write(1, numStr, strlen(numStr));
				i++;
			} else if (format[i+1] == 'f') {
				double num  = va_arg(arg, double);
                                char *numStr = malloc(sizeof(char)*num);
                                sprintf(numStr, "%f", num);
                                write(1, numStr, strlen(numStr));
                                i++;
			} else if (format[i+1] == 's') {
				char *str = va_arg(arg, char*);
				write(1, str, strlen(str));
				i++;
			}
		} else {
			write(1, &format[i], sizeof(char));
		}
	}


	va_end(arg);
	write(1, "\033[m\n", strlen("\033[m\n"));

  	return 0;
}

int panicf(const char *format, ...){

	if(strlen(format) == 0){
                return -1;
        }

	int i;

	va_list arg;
	va_start (arg, format);

	write(1, "\033[30;41m", strlen("\033[30;41m"));

	for(i=0; i<strlen(format); i++) {
		if (format[i] == '%' && i+1 < strlen(format)) {
			if (format[i+1] == 'd') {
				int num  = va_arg(arg, int);
				char *numStr = malloc(sizeof(char)*num);
				sprintf(numStr, "%d", num);
				write(1, numStr, strlen(numStr));
				i++;
			} else if (format[i+1] == 'f') {
				double num  = va_arg(arg, double);
                                char *numStr = malloc(sizeof(char)*num);
                                sprintf(numStr, "%f", num);
                                write(1, numStr, strlen(numStr));
                                i++;
			} else if (format[i+1] == 's') {
				char *str = va_arg(arg, char*);
				write(1, str, strlen(str));
				i++;
			}
		} else {
			write(1, &format[i], sizeof(char));
		}
	}


	va_end(arg);
	write(1, "\033[m\n", strlen("\033[m\n"));

  	abort();
}
