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
int getword(char *w, int lim);

int main(void) {
    struct treeNode *root = NULL;
    char word[MAXWORD];

    while (getword(word, MAXWORD) != EOF) {
        root = addTree(root, word);
    }

    printTree(root);
    
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
