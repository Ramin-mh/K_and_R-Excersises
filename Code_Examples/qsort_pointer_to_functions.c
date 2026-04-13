#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000 /* max #lines to be sorted */
#define MAXLEN 1000 /* max #chars in a line */

char *lineptr[MAXLINES]; /* pointers to text lines */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void quick_sort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
void swap(void *v[], int, int);
int numcmp(char *, char *);
int cmp_str(void *, void *);
int cmp_num(void *, void *);

/* sort input lines */
int main(int argc, char *argv[])
{
    int nlines; /* number of input lines read */
    int numeric = 0; /* 1 if numeric sort */

    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        numeric = 1;
    }
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        quick_sort((void**) lineptr, 0, nlines-1, (int (*)(void*,void*))(numeric? cmp_num : cmp_str));
        writelines(lineptr, nlines);
        return 0;
    }
    else {
        printf("input too big to sort\n");
        return 1;
    }
}

/* quick_sort: sort v[left]...v[right] into increasing order */
void quick_sort(void *v[], int left, int right,int (*comp)(void *, void *))
{
    int i, last;

    if (left >= right){
        return;
    }    
    swap(v, left, (left + right)/2);
    last = left;
    for (i = left+1; i <= right; i++){
        if ((*comp)(v[i], v[left]) < 0){
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    quick_sort(v, left, last-1, comp);
    quick_sort(v, last+1, right, comp);
}

/* numcmp: compare s1 and s2 numerically */
int numcmp(char *s1, char *s2)
{
    double v1, v2;

    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2){
        return -1;
    }
    else if (v1 > v2) {
        return 1;
    }
    else {
     return 0;   
    }
}

void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

int cmp_str(void *s1, void *s2){
    return (strcmp((char *)s1, (char *)s2));
}

int cmp_num(void *s1, void *s2){
    return (numcmp((char *)s1, (char *)s2));
}

int readlines(char **arr, int maxlines){
    int nlines, len;
    char *line;
    size_t size = MAXLEN;
    char *p;

    nlines = 0;
    while ((len = getline(&line, &size, stdin)) > 0){
        if (nlines >= maxlines || (p = malloc(sizeof(char) * (len + 1))) == NULL){
            return -1;
        }
        else {
            strcpy(p, line);
            arr[nlines++] = p;            
        }
    }

    return nlines;   
}

void writelines(char **arr, int nlines){
    int i;
    for (i = 0; i < nlines; i++){
        printf("%s\n", arr[i]);
    }
}
