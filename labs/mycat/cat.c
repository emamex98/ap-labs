// ORIGINAL CAT

#include <stdio.h>
#include <time.h>  
#include <signal.h>
#include <stdlib.h>
#include <string.h>

double time_spent;
clock_t begin;
clock_t end;

void  INThandler(int sig)
{
    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime elpased is %f seconds\n", time_spent);
    exit(0);
}

/* filecopy:  copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);

}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
	time_spent = 0.0;
    begin = clock();

    FILE *fp;
    void filecopy(FILE *, FILE *);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1) { /* no args; copy standard input */
        signal(SIGINT, INThandler);
        filecopy(stdin, stdout);
    }
    else
        while (--argc > 0)
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "%s: can′t open %s\n",
			prog, *argv);
                return 1;
            } else {
                filecopy(fp, stdout);
                fclose(fp);
            }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time elpased is %f seconds\n", time_spent);

    return 0;
}
