#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


void username();
void openedfile();
void immortalprocess();
void hidedoogdoor();

void main() {
        char mode[30];
        while (1) {
                printf("------------------------------------------------------------------------\n");
                printf("                                mode\n");
                printf("------------------------------------------------------------------------\n");
                printf("        1. assign a user\n");
                printf("        2. print a list of opened file of selected user\n");
                printf("        3. make a immortal process\n");
                printf("        4. hide doogdoor\n");
                printf("        5. quit\n");
                printf("------------------------------------------------------------------------\n");
                printf("mode: ");
                fgets(mode, sizeof(mode), stdin);
                switch(atoi(mode)) {
                        case 1: username(); return;
                        case 2: openedfile(); return;
                        case 3: immortalprocess(); return;
                        case 4: hidedoogdoor(); return;
                        case 5: printf("(bye)\n"); return;
                        default : printf("Invalid input. Try again\n"); break;
                }
        }
}

void username() {
        int fd;
        fd = open("/proc/dogdoor", O_RDWR);

        char uid[32]="u";
        char buf[32];
        printf("Enter the userID: ");
        fgets(buf, sizeof(buf), stdin);

        strcat(uid, buf);
        lseek(fd, 0, SEEK_SET);
        write(fd, uid, sizeof(char)*(strlen(uid)+1));

        close(fd);
        return;
}

void openedfile() {

}

void immortalprocess() {
        int fd;
        fd = open("/proc/dogdoor", O_RDWR);

        char pid[32]="p";
        char buf[32];
        printf("Enter the process ID: ");
        fgets(buf, sizeof(buf), stdin);

        strcat(pid, buf);
        lseek(fd, 0, SEEK_SET);
        write(fd, pid, sizeof(char)*(strlen(pid)+1));

        close(fd);
        return;
}

void hidedoogdoor() {
        int fd;
        fd = open("/proc/dogdoor", O_RDWR);

        char yesorno[32]="h";
        char buf[32];
        printf("Do you want to hide DoogDoor? y/n: ");
        fgets(buf, sizeof(buf), stdin);
        if ( buf[0] != 'y' && buf[0] != 'n') {
                printf("Invalid\n");
                return;
        }

        strcat(yesorno, buf);
        lseek(fd, 0, SEEK_SET);
        write(fd, yesorno, sizeof(char)*(strlen(yesorno)+1));

        close(fd);
        return;
}
