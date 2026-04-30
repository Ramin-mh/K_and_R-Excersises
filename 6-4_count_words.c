#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

struct treeNode {
    char *word;
    int count;

    struct treeNode *left;
    struct treeNode *right;
};

struct treeNode *addTree(struct treeNode *p, char *w);
void printTree(struct treeNode *p);
void treeToArray(struct treeNode *p, struct treeNode ***arr, size_t *size);
void printTreeArray(struct treeNode **arr, size_t len);
int cmp_tree_count_wrapper(void *a, void *b);

int getword(char *w, int lim);
void quick_sort(void *arr, int left, int right, size_t size, int (*cmp)(void *, void *));

int main(void) {
    struct treeNode *root = NULL;
    char word[MAXWORD];

    struct treeNode **treeNodes = NULL;
    size_t len = 0;

    while (getword(word, MAXWORD) != EOF) {
        root = addTree(root, word);
    }

    treeToArray(root, &treeNodes, &len);
    quick_sort(treeNodes, 0, len-1, (sizeof(struct treeNode *)), cmp_tree_count_wrapper);

    printf("-------------------------\n");
    printTreeArray(treeNodes, len);
    
    return 0;
}

struct treeNode *allocTree(void) {
    return malloc(sizeof(struct treeNode));
}

struct treeNode *addTree(struct treeNode *p, char *w) {
    int cond;

    if (p == NULL) {
        p = allocTree();
        p->word = strdup(w);
        p->count = 1;

        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0) {
        p->count++;
    }
    else if (cond < 0) {
        p->left = addTree(p->left, w);
    }
    else {
        p->right = addTree(p->right, w);
    }

    return p;
}

void treeToArray(struct treeNode *p, struct treeNode ***arr, size_t *size) {
    if (p != NULL) {
        treeToArray(p->left, arr, size);
        *arr = realloc(*arr, (*size + 1) * sizeof(struct treeNode *));
        (*arr)[(*size)++] = p;
        treeToArray(p->right, arr, size);
    }
}

void printTreeArray(struct treeNode **arr, size_t len) {
    int i;

    for (i = 0; i < len; i++) {
        printf("%s: %d\n", (arr[i])->word, (arr[i])->count);
    }
}

void printTree(struct treeNode *p) {
    if (p != NULL) {
        printTree(p->left);
        printf("%s: %d\n", p->word, p->count);
        printTree(p->right);
    }
}

static int buf = EOF;

int getch(void) {
    int tmp;

    if (buf != EOF) {
        tmp = buf;
        buf = EOF;
        return tmp;
    }
    else {
        return getchar();
    }
}

void ungetch(int c) {
    if (buf == EOF) {
        buf = c;
    }
    else {
        fprintf(stderr, "ungetch: buffer overflow.\n");
        exit(1);
    }
}

int getword(char *w, int lim) {
    int c;
    char *p = w;
    char *end = w + lim - 1;

    if (lim <= 0) {
        return EOF;
    }
    
    while (!isalpha((c = getch()))) {
        if (c == EOF) return EOF;
    }
    
    do {
        if (p < end) {
            *p++ = c;
        }
    } while (isalpha((c = getch())));
    *p = '\0';

    ungetch(c);

    return w[0];
}

void swap(void *a, void *b, size_t size) {
    char *p = a;
    char *q = b;
    char tmp;
    size_t i;

    /* swap each byte untill swapped size bytes */
    for (i = 0; i < size; i++) {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

void quick_sort(void *arr, int left, int right, size_t size, int (*cmp)(void *, void *)) {
    char *p = arr;
    int i, last;
    
    if (left >= right) {
        return;
    }

    swap(p + left * size, p + ((left + right)/2) * size, size);

    last = left;
    for (i = left + 1; i <= right; i++) {
        if (cmp(p + i * size, p + left * size) > 0) {
            ++last;
            swap(p + i * size, p + last * size, size);
        }
    }
    swap(p + left * size, p + last * size, size);

    quick_sort(arr, left, last - 1, size, cmp);
    quick_sort(arr, last + 1, right, size, cmp);
}

int cmp_tree_count(struct treeNode **a, struct treeNode **b) {
    if ((*a)->count > (*b)->count) {
        return 1;
    }
    else if ((*a)->count < (*b)->count){
        return -1;
    }
    else {
        return 0;
    }
}

int cmp_tree_count_wrapper(void *a, void *b) {
    return cmp_tree_count((struct treeNode **)a, (struct treeNode **)b);
}
