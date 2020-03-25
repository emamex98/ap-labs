// MODIFIED CAT

#include <stdio.h>
#include <time.h>  
#include <fcntl.h> 
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

double time_spent;
clock_t begin;
clock_t end;

void  INThandler(int sig)
{
    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    write(1, "\nTime elpased is: ", strlen("Time elpased is: "));
    printf("%f seconds\n", time_spent);
    exit(0);
}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
	time_spent = 0.0;
    begin = clock();

    int fd;
    char *prog = argv[0];   /* program name for errors */
    char c;

    if (argc == 1)  /* no args; copy standard input */
        while(read(0, &c, 1) > 0)
        {
            write(1, &c, sizeof(c));
            signal(SIGINT, INThandler);
        }
    else
        while (--argc > 0)
            if ((fd = open(*++argv, O_RDONLY)) < 0) {
                write(1, "Error opening file(s).\n", strlen("Error opening file(s).\n"));
                return 1;
            } else {
                while(read(fd, &c, 1) == 1){
                    write(1, &c, sizeof(c));
                }
                if (close(fd) < 0)  { 
                    write(1, "Error closing file(s).\n", strlen("Error closing file(s).\n"));
                }
            }

    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    write(1, "Time elpased is: ", strlen("Time elpased is: "));
    printf("%f seconds\n", time_spent);

    return 0;
}
