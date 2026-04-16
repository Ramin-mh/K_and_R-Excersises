#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000 /* max #lines to be sorted */
#define MAXLEN 1000 /* max #chars in a line */

int numeric = 0; /* 1 if numeric sort */
int reverse = 0; /* -1 if reverse sort (descending)*/
int nocases = 0; /* 1 if fold upper and lower together */
int directory = 0; /* 1 if directory order */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void quick_sort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int cmp_str(void *, void *);
int cmp_num(void *, void *);
void free_lineptr(char *lineptr[], int nlines);

/* sort input lines */
int main(int argc, char *argv[])
{
    char *lineptr[MAXLINES]; /* pointer array to each lines */
    int nlines; /* number of input lines read */
    
    char *str;
    int i;

    for (i = 1; i < argc; i++){
        str = argv[i];

        if (*str != '-'){
            fprintf(stderr, "invalid argument\n");
            return 1;
        }

        while (*(++str) != '\0'){
            switch(*str){
                case 'n':
                    numeric = 1;
                    break;
                case 'r':
                    reverse = 1;
                    break;
                case 'f':
                    nocases = 1;
                    break;
                case 'd':
                    directory = 1;
                    break;
                default:
                fprintf(stderr, "invalid argument\n");
                return 1;
            }
        }
        
    }

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        quick_sort((void **)lineptr, 0, nlines-1, (int (*)(void*,void*))(numeric? cmp_num : cmp_str));
        writelines(lineptr, nlines);
        free_lineptr(lineptr, nlines);
        return 0;
    }
    else {
        printf("input too big to sort\n");
        free_lineptr(lineptr, nlines);
        return 1;
    }
}

int readlines(char **arr, int maxlines){
    int nlines, len;
    char *line;
    char *p;
    size_t cap = 0;

    nlines = 0;
    while ((len = getline(&line, &cap, stdin)) > 0){
        if (nlines >= maxlines || (p = malloc(sizeof(char) * (len + 1))) == NULL){
            return -1;
        }
        else {
            strcpy(p, line);
            free(line);
            arr[nlines++] = p;            
        }
    }

    return nlines;   
}

void writelines(char **arr, int nlines){
    int i;
    for (i = 0; i < nlines; i++){
        printf("%s", arr[i]);
    }
}

void swap(void *v[], int i, int j)
{
    void *temp;

    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void quick_sort(void *v[], int left, int right,int (*comp)(void *, void *)){
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
int numcmp(const char *s1, const char *s2)
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

int isdir(int c){
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '9') ||
           c == ' ';
}

int string_cmp(const char *s1, const char *s2){
    int c1, c2;
    while (1){
        if (directory){
            if (!isdir(*s1)) s1++;
            if (!isdir(*s2)) s2++;
        }

        c1 = *s1;
        c2 = *s2;

        if (nocases){
            if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
            if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        }

        if (c1 != c2){
            return c1 - c2;
        }

        if (c1 == '\0'){
            return 0;
        }

        s1++;
        s2++;
    }
}

int cmp_str(void *s1, void *s2){
    if (reverse){
        return string_cmp((char *)s2, (char *)s1);
    }
    return string_cmp((char *)s1, (char *)s2);
}

int cmp_num(void *s1, void *s2){
    if (reverse){
        return numcmp((char *)s1, (char *)s2);
    }
    return numcmp((char *)s1, (char *)s2);
}

void free_lineptr(char *lineptr[], int nlines){
    int i;

    for (i = 0; i < nlines; i++) {
        free(lineptr[i]);
    }
}
