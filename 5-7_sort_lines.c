#include <stdio.h>
#include <string.h>

#define MAXLINES 5000
#define STORAGE 10000

int getAline(char *arr, int maxlen);
int readlines(char **arr, int maxlines, char *storage, int storage_size);
void writelines(char **arr, int nlines);

void quick_sort_lines(char **arr, int left, int right);
void swap(char **arr, int index1, int index2);

int main(void){
    char *arr[MAXLINES];
    char lines_storage[STORAGE];
    int nlines;

    printf("Enter many lines:-\n\n");

    if ((nlines = readlines(arr, MAXLINES, lines_storage, STORAGE)) >= 0){   
        quick_sort_lines(arr, 0, nlines - 1);
        writelines(arr, nlines);
        return 0;
    }
    else {
        fprintf(stderr, "error: input too big to sort\n");
        return 1;
    }
}

int getAline(char *arr, int maxlen){
    int c;
    int len = 0;

    while ((c = getchar()) != EOF && c != '\n'){
        if (len < maxlen - 1){
            arr[len++] = c;
        }
    }
    arr[len] = '\0';

    return len;
}

#define MAXLEN 1000

int readlines(char **arr, int maxlines, char *storage, int storage_size){
    int nlines, line_len;
    char line[MAXLEN];
    char *p;

    char *storage_end = storage + storage_size;

    nlines = 0;
    while ((line_len = getAline(line, MAXLEN)) > 0){
        if (nlines >= maxlines || (storage + line_len + 1) > storage_end){
            return -1;
        }
        else {
            strcpy(storage, line);
            arr[nlines++] = storage;
            storage += line_len + 1; // +1 to include the \0 at the end
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

void quick_sort_lines(char **arr, int left, int right){
    int partition_index;
    int i;
    
    if (left >= right){
        return;
    }

    swap(arr, left, (left + right) / 2);

    partition_index = left;
    for (i = left + 1; i <= right; i++){
        if (strcmp(arr[i], arr[left]) < 0){
            swap(arr, ++partition_index, i);
        }
    }
    swap(arr, left, partition_index);

    quick_sort_lines(arr, left, partition_index - 1);
    quick_sort_lines(arr, partition_index + 1, right);
}

void swap(char **arr, int index1, int index2){
    char *temp;

    temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
}
