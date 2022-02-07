/*
 * Author: Emmanuel(PabloPotat0)
 * Date: January 16th 2021
 * Description: A file/binary monitor for redteam tools
 * Issues: inotify seem to not like comparing the event file with a string, the program "hangs" or doesn't return any event. So, 
 *         I'm just leave it like this for now. If I figure out a workaround, I will fix it.
 */

#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h> // library for fcntl function


#define MAX_EVENTS 1024  /* Maximum number of events to process*/
#define LEN_NAME 16  /* Assuming that the length of the filename
won't exceed 16 bytes*/
#define EVENT_SIZE  ( sizeof (struct inotify_event) ) /*size of one event*/
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))
/*buffer to store the data of events*/

int fd, wd;
char *path_to_watch[] = {"/etc/", "/usr/bin/", "/bin/", "/tmp"};
char *files_to_watch[] = {"passwd", "troll", "modbus", "test.txt"};

void sig_handler(int sig) {
    inotify_rm_watch(fd, wd);
    close(fd);
    exit(0);
}


int main (int argc, char **argv) {
    char *path;
    signal(SIGINT, sig_handler);

    fd = inotify_init();

    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0){ // error checking for fcntl
        exit(2);
    }

    int arraySize = sizeof(path_to_watch) / sizeof(path_to_watch[0]);
    for (int i = 0; i < arraySize; i++) {
        wd = inotify_add_watch(fd, path_to_watch[i], IN_ACCESS | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE | IN_MOVE_SELF | IN_OPEN);

        if (wd == -1) {
            printf("Could not watch : %s\n", path_to_watch[i]);
        } else {
            printf("Watching : %s\n", path_to_watch[i]);
        }
    }

    while(1) {
        int i = 0, length;
        char buffer[BUF_LEN];

        length = read(fd, buffer, BUF_LEN);

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];

            if (event->len) {
                if(event->mask & IN_ACCESS) {
                    printf("%s was created\n", event->name);
                } else if (event->mask & IN_DELETE) {
                    printf("%s was deleted\n", event->name);
                } else if (event->mask & IN_DELETE_SELF) {
                    printf("%s was deleted(self)\n", event->name);
                } else if (event->mask & IN_MODIFY) {
                    printf("%s was modified\n", event->name);
                } else if (event->mask & IN_MOVE) {
                    printf("%s was moved\n", event->name);
                } else if (event->mask & IN_MOVE_SELF) {
                    printf("Someone moved the gatekeeper %s\n", event->name);
                } else if (event->mask & IN_OPEN) {
                    printf("%s was open\n", event->name);
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
}