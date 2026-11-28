#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "syscalls.h"
#define PERMS 0666 /* RW for owner, group, others */

FILE _iob[OPEN_MAX] = { /* stdin, stdout, stderr */
    {0, (char *) 0, (char *) 0, {1, 0, 0, 0, 0}, 0},
    {0, (char *) 0, (char *) 0, {0, 1, 0, 0, 0}, 1},
    {0, (char *) 0, (char *) 0, {0, 1, 0, 0, 1}, 2} };

FILE *fopen(char *name, char *mode)
{
    int fd;
    FILE *fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if (fp->flag.read == 0 && fp->flag.write == 0)
            break; /* found free slot */
    if (fp >= _iob + OPEN_MAX) /* no free slots */
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a')
    {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    }
    else
        fd = open(name, O_RDONLY, 0);
    if (fd == -1) /* couldn't access name */ 
        return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    if (*mode == 'r') fp->flag.read = 1;
    else fp->flag.write = 1;
    
    return fp;
}

/* fillbuf: allocate and fill input buffer */
int _fillbuf(FILE *fp)
{
    int bufsize;

    if (fp->flag.eof ||  fp->flag.err || !fp->flag.read)
        return NULL;
    bufsize = (fp->flag.unbuf)? 1 : BUFSIZ;
    if (fp->base == NULL) /* no buffer yet */
        if ((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF;
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0)
    {
        if (fp->cnt == -1)
            fp->flag.eof = 1;
        else
            fp->flag.err = 1;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

int _flushbuf(int x, FILE *fp)
{
    int bufsize, bufn, n;
    
    if (fp->flag.eof || fp->flag.err || !fp->flag.write)
        return NULL;
    bufsize = (fp->flag.unbuf)? 1 : BUFSIZ;
    if (fp->base == NULL) /* no buffer yet */
    {
        if ((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF;
        fp->cnt = bufsize;
    }
    if (fp->cnt == -1) /* cnt = -1 if buffer is filled */
    {
        for (bufn = bufsize; bufn > 0; bufn -= n)
        {
            if((n = write(fp->fd, fp->base, bufn)) == -1)
                return EOF;
        }
        fp->cnt = bufsize;
    }
    fp->ptr = fp->base;
    *fp->ptr++ = x;
    --fp->cnt;

    return (unsigned char) x;
}

int fflush(FILE *fp)
{
    int bufsize, bufn, n;
     
    if (fp->flag.eof || fp->flag.err || !fp->flag.write)
        return NULL;
    bufsize = (fp->flag.unbuf)? 1 : BUFSIZ;
    if (fp->base == NULL) return 0;

    for (bufn = bufsize-fp->cnt; bufn > 0;bufn -= n)
    {
        if((n = write(fp->fd, fp->base, bufn)) == -1)
            return EOF;
    }
    fp->cnt = bufsize;
    fp->ptr = fp->base;

    return 0;
}

int fclose(FILE *fp)
{
    if (fp->flag.write)
        fflush(fp);
    if (fp->base != NULL)
    {
        free(fp->base);
        fp->base = fp->ptr = NULL;
    }
    fp->flag = (FLAGS){0};
    if (close(fp->fd) == -1) return EOF;
    return 0;
}

/* for testing */
int main(void)
{
    int x;
    FILE *fpi = fopen("toread.txt", "r");
    FILE *fpo = fopen("tmp.txt", "w");

    while ((x = getc(fpi)) != EOF)
    {
        putc(x, fpo);
    }
    fclose(fpo);
    fclose(fpi);

    return 0;
}
