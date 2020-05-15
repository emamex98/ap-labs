#define _XOPEN_SOURCE 500
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <ftw.h>
#include <sys/inotify.h>
#include "logger.h"

int nt, wd, j;
char buf[BUF_LEN] __attribute__ ((aligned(8)));
ssize_t numRead;
char *p;
struct inotify_event *event;

char paths[100000][1000];
int n = 0;

int watch();

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    
    if(tflag == FTW_D){
        if(strchr(fpath, '.') == NULL){
            strcpy(paths[n], fpath);
            n++;
        }
    }

    return 0;
}

void addWatch(){
    for (j = 0; j < n; j++) {
        wd = inotify_add_watch(nt, paths[j], IN_ALL_EVENTS);
        if (wd == -1){
            errorf("Error while reading path %s\n", paths[j]);
            exit(1);
        }
    }
}

static int alertChange(struct inotify_event *event) {
    if (event->mask & IN_CREATE){
        infof("Created: %s\n", event->name);
        return 0;
    }
    
    if (event->mask & IN_MOVED_FROM) {
        errorf("Deleted: %s ", event->name);
        if(watch() == 1){
            warnf("\rRenamed: %s            ", event->name);
        }
        errorf("\n");
        return 0;
    }
    
    if (event->mask & IN_MOVED_TO) {
        return 1;
    }
}

int watch(){    
    int r;
    numRead = read(nt, buf, BUF_LEN);

    if (numRead < 1){
        errorf("Error while reading buffer");
        exit(1);
    }

    for (p = buf; p < buf + numRead; ) {
        event = (struct inotify_event *) p;
        r = alertChange(event);
        p += sizeof(struct inotify_event) + event->len;
    }

    return r;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
	    errorf("Error - Usage: ./%s <path>", argv[0]);
	    exit(1);
    }


    for (;;) {
        n = 0;
        int tmp = nftw(argv[1], display_info, 20, 0);

        if (tmp == -1) {
            errorf("Error with NFTW");
            exit(1);
        }

        nt = inotify_init();

        if (nt < 0){
            errorf("Error with iNotify initialization");
            exit(1);
        }

        addWatch();
        watch();
    }

    return 0;
}