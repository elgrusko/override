
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void        log_wrapper(char *state, char *filename)
{
    char buf[255];
    strcpy(buf, state);
    snprintf(buf + strlen(buf), 255 - strlen(buf) - 1, filename);
    buf[strcspn(buf, "\n")] = '\0';
}

int         main(int argc, char *argv[])
{
    char letter = EOF;
    char dst[100];
    FILE *fd_file, *fd_log, *fd_write;

    if (argc != 2)
        printf("Usage: %s filename\n", argv[0]);
    fd_log = fopen("./backups/.log", "w");
    if (fd_log == NULL)
    {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(-1);
    }
    log_wrapper(fd_log, "Starting back up:", argv[1]);
    fd_file = fopen(argv[1], "r");
    if (fd_file == NULL)
    {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(-1);
    }
    strcpy(dst, "./backups/");
    strncat(dst, argv[1], 100 - strlen(dst)-1);
    fd_write = open(dst, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd_write < 0)
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(EXIT_FAILURE);
    }
    while( ( letter = fgetc(fd_file) ) != EOF)
        write(fd_write, &letter, 1);
    log_wrapper(fd_log, "Finished back up", argv[1]);
    fclose(fd_file);
    close(fd_write);
    return (0);
}
