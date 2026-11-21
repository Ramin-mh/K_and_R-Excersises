#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int n, fd;
    char buf[BUFSIZ];
    
    if (argc == 1)
    {
        while((n = read(0, buf, sizeof(buf))) > 0)
        {
            write(1, buf, n);
        }
    }

    while (--argc > 0)
    {
        fd = open(*++argv, O_RDONLY, 0);
        while ((n = read(fd, buf, sizeof(buf))) > 0)
        {
            write(1, buf, n);
        }
        close(fd);
    }
}
