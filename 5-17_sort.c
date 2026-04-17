#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000 /* max #lines to be sorted */
#define MAXLEN 1000 /* max #chars in a line */
#define MAXFIELDS 10 /* max #fields */

int nfields = 0;
int field_index[MAXFIELDS];
int numeric[MAXFIELDS];
int reverse[MAXFIELDS];
int nocases[MAXFIELDS];
int directory[MAXFIELDS];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void quick_sort(void *lineptr[], int left, int right, int (*comp)(void *, void *));
int cmp_str(void *, void *);
int cmp_num(void *, void *);
int cmp_field(void *, void *);
void free_lineptr(char *lineptr[], int nlines);

/* sort input lines */
int main(int argc, char *argv[]){
    char *lineptr[MAXLINES]; /* pointer array to each lines */
    int nlines; /* number of input lines read */
    
    char *str;
    int i, n;

    for (i = 1; i < argc; i++){
        str = argv[i];

        if (*str != '-'){
            fprintf(stderr, "invalid argument\n");
            return 1;
        }

        if ((n = atoi(str + 1)) > 0){
            if (nfields > 0 && field_index[nfields-1] == 0){
                fprintf(stderr, "invalid argument\n");
                return 1;
            }
            field_index[nfields] = n;
            str++;
        }
        else if (nfields > 0 && field_index[nfields-1] != 0){
            fprintf(stderr, "invalid argument\n");
            return 1;
        }

        while (*(++str) != '\0'){
            switch(*str){
                case 'n':
                    numeric[nfields] = 1;
                    break;
                case 'r':
                    reverse[nfields] = 1;
                    break;
                case 'f':
                    nocases[nfields] = 1;
                    break;
                case 'd':
                    directory[nfields] = 1;
                    break;
                default:
                fprintf(stderr, "invalid argument\n");
                return 1;
            }
        }

        if (n > 0) nfields++;
    }
   
    if ((nlines = readlines(lineptr, MAXLINES)) < 0) {
        printf("input too big to sort\n");
            free_lineptr(lineptr, nlines);
            return 1;
    }

    if (nfields == 0){
        quick_sort((void **)lineptr, 0, nlines-1, (int (*)(void*,void*))(numeric[0]? cmp_num : cmp_str));
        writelines(lineptr, nlines);
        free_lineptr(lineptr, nlines);
        return 0;
    }

    quick_sort((void **)lineptr, 0, nlines-1, (int (*)(void*,void*))cmp_field);
    writelines(lineptr, nlines);
    free_lineptr(lineptr, nlines);
    return 0;
}

void free_lineptr(char *lineptr[], int nlines){
    int i;

    for (i = 0; i < nlines; i++) {
        free(lineptr[i]);
    }
}

int readlines(char **arr, int maxlines){
    int nlines, len;
    char *line = NULL;
    char *p;
    size_t cap = 0;

    nlines = 0;
    while ((len = getline(&line, &cap, stdin)) > 0){
        if (line[len-1] == '\n'){
            line[--len] = '\0';
        }

        if (nlines >= maxlines || ((p = malloc(sizeof(char) * (len + 1))) == NULL)){
            free(line);
            return -1;
        }

        strcpy(p, line);
        arr[nlines++] = p; 
    }

    return nlines;   
}

void writelines(char **arr, int nlines){
    int i;
    printf("\n=========================\n");
    for (i = 0; i < nlines; i++){
        printf("%s\n", arr[i]);
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

int cmp_num(void *s1, void *s2){
    if (reverse[0]){
        return numcmp((char *)s1, (char *)s2);
    }
    return numcmp((char *)s1, (char *)s2);
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
        if (directory[0]){
            if (!isdir(*s1)){
                s1++;
                continue;
            }
            if (!isdir(*s2)){
                s2++;
                continue;
            }
        }

        c1 = *s1;
        c2 = *s2;

        if (nocases[0]){
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
    if (reverse[0]){
        return string_cmp((char *)s2, (char *)s1);
    }
    return string_cmp((char *)s1, (char *)s2);
}

int find_field(char *s, char **start, char **end, int field){
    int current_field = 1;

    *start = NULL;
    *end = NULL;
    
    // skip initial spaces
    while (*s == ' ' || *s == '\t'){
        s++;
    }
    // loop until reach the field needed
    while (current_field < field){
        // skip non-space and check for error
        while (*s != ' ' && *s != '\t'){            
            if (*s == '\0'){
                return 1;
            }
            s++;
        }
        // skip the spaces
        while (*s == ' ' || *s == '\t'){
            s++;
        }
        if (*s == '\0'){
            return 1;
        }
        current_field++;
    }
    // s is now the first non-space of the field
    *start = s;
    while (*s != ' ' && *s != '\t' && *s != '\0'){
        s++;
    }
    // s is now the char just ahead of the last char included in the field
    *end = s;
    // now the field is in the interval [start, end)

    return 0;
}

int field_strcmp(char *s1_start, char *s1_end, char *s2_start, char *s2_end, int pf_index){
    int c1, c2;

    // loop until one or both fields reach end
    while (s1_start < s1_end && s2_start < s2_end){
        // if not directory, skip that part 
        if (directory[pf_index]){
            if (!isdir(*s1_start) || !isdir(*s2_start)){
                if (!isdir(*s1_start)){
                    s1_start++;
                }
                if (!isdir(*s2_start)){
                    s2_start++;
                }
                continue;
            }
        }

        c1 = *s1_start;
        c2 = *s2_start;

        // convert upper to lower for folding cases
        if (nocases[pf_index]){
            if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
            if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        }

        // return diffrence if not equal
        if (c1 != c2){
            return c1 - c2;
        }

        s1_start++;
        s2_start++;
    }

    // return 0 if both fields reach end
    if (s1_start == s1_end && s2_start == s2_end){
        return 0;
    }
    // return 1 if the second field only reached end (implies, first field has more chars and thus larger)
    if (s2_start == s2_end){
        return 1;
    }
    // return -1 if first field only reached end (implies, first field has less chars thus smaller)
    if (s1_start == s1_end){
        return -1;
    }

    return 0; // to remove warning
}

int field_numcmp(char *s1_start, char *s1_end, char *s2_start, char *s2_end){
    // save end chars of both fields
    int s1_end_char = *s1_end;
    int s2_end_char = *s2_end;
    double v1, v2;

    // assign end chars to \0
    *s1_end = '\0';
    *s2_end = '\0';

    v1 = atof(s1_start);
    v2 = atof(s2_start);

    // reset end of both fields
    *s1_end = s1_end_char;
    *s2_end = s2_end_char;
    
    if (v1 > v2){
        return 1;
    }
    else if (v1 < v2){
        return -1;
    }
    else {
        return 0;
    }
}

int fieldcmp(char *s1, char *s2){
    int cpf; // current priority field
    int cpf_index;
    int field;
    int i;
    int res;

    char *s1_start, *s1_end;
    char *s2_start, *s2_end;
    
    for (cpf = 1; cpf <= nfields; cpf++){
        for (i = 0; field_index[i] != cpf; i++);
        cpf_index = i; // i now becomes the index which can be used to call all flags regarding i+1 th field
        field = i + 1; // this is the current field as the index is according to the no.of fields

        find_field(s1, &s1_start, &s1_end, field);
        find_field(s2, &s2_start, &s2_end, field);

        if (s1_start == NULL && s2_start == NULL){
            return 0;
        }
        else if (s1_start == NULL){
            if (reverse[cpf_index]) return 1;
            return -1;
        }
        else if (s2_start == NULL){
            if (reverse[cpf_index]) return -1;
            return 1;
        }

        if (numeric[cpf_index]){
            if (reverse[cpf_index]){
                res = field_numcmp(s2_start, s2_end, s1_start, s1_end);
            }
            else {
                res = field_numcmp(s1_start, s1_end, s2_start, s2_end);
            }
        }
        else {
            if (reverse[cpf_index]){                    
                res = field_strcmp(s2_start, s2_end, s1_start, s1_end, cpf_index);
            }
            else {
                res = field_strcmp(s1_start, s1_end, s2_start, s2_end, cpf_index);
            }
        }

        if (res != 0){
            return res;
        }      
    }
    return 0;
}

int cmp_field(void *s1, void *s2){
    return fieldcmp((char *)s1, (char *)s2);
}

