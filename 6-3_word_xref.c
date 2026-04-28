#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100
#define MAXLEN 100

struct treeNode {
    char *word;
    int line_nos[MAXLEN];
    int top;

    struct treeNode *left;
    struct treeNode *right;
};

struct treeNode *addTree(struct treeNode *p, char *w, int line_no);
void printTree(struct treeNode *p);

int getword(char *w, int size, int *line_no);

int main(void) {
    struct treeNode *root = NULL;
    
    char w[MAXWORD];
    int k;
    int line_no = 1;

    if (MAXLEN <= 0) {
        fprintf(stderr, "No size allocated to array! exiting..\n");
        return 1;
    }

    while ((k = getword(w, MAXWORD, &line_no)) != EOF && k != '\0') {
        root = addTree(root, w, line_no);
    }

    if (k == '\0') {
        fprintf(stderr, "the word exceeded given size.\n");
        return 1;
    }

    printTree(root);

    return 0;
}

struct treeNode *allocTree(void) {
    return malloc(sizeof(struct treeNode));
}

struct treeNode *addTree(struct treeNode *p, char *w, int line_no) {
    int k;
    
    if (p == NULL) {
        if ((p = allocTree()) == NULL) {
            fprintf(stderr, "allocation failed\n");
            exit(1);
        }
        p->word = strdup(w);
        p->top = 0;
        p->line_nos[p->top++] = line_no;

        p->left = p->right = NULL;
    }
    else if ((k = strcmp(w, p->word)) == 0) {
        if (p->top >= MAXLEN) {
            fprintf(stderr, "Array size exceeded!, exiting..\n");
            exit(1);
        }

        if (p->line_nos[p->top - 1] != line_no) {
            p->line_nos[p->top++] = line_no;
        }
    }
    else if (k < 0) {
        p->left = addTree(p->left, w, line_no);
    }
    else {
        p->right = addTree(p->right, w, line_no);
    }

    return p;
}

void printTree(struct treeNode *p) {
    int i;
    
    if (p != NULL) {
        printTree(p->left);
        printf("%s:-\n", p->word);
        for (i = 0; i < p->top; i++) {
            printf("%d, ", p->line_nos[i]);
        }
        putchar('\n');
        printTree(p->right);
    }
}

int is_noise_word(char *s) {
    return strcmp(s, "and") == 0 ||
           strcmp(s, "the") == 0 ||
           strcmp(s, "or") == 0 ||
           strcmp(s, "a") == 0 ||
           strcmp(s, "when") == 0;
}

void to_lower(char *s) {
    for (; *s; s++) {
        *s = tolower(*s);
    }
}

static int buf = EOF;

int getch(void) {
    int tmp;
    
    if (buf == EOF) {
        return getchar();
    }
    else {
        tmp = buf;
        buf = EOF;
        return tmp;    
    }
}

void ungetch(int c) {
    if (buf == EOF) {
        buf = c;
    }
    else {
        fprintf(stderr, "buffer overflow!\n");
        exit(1);
    }
}

int getword(char *w, int size, int *line_no) {
    char *p;
    int i;
    int c;

    while (1) {
        p = w;
        i = size;
        
        /* Skip non-alphabets */
        while (!isalpha((c = getch()))) {
            if (c == EOF) return EOF;
            if (c == '\n') (*line_no)++;
        }

        do {
            *p++ = c;
            i--;
            if (i < 1) {
                *(--p) = '\0';
                return '\0';
            }
        } while (isalpha((c = getch())));
        *p = '\0';

        ungetch(c); /* leave the char to be checked by the top loop */
        
        to_lower(w);
        if (!is_noise_word(w)) {
            return w[0];
        }
    }
}
