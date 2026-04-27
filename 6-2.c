#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 100

struct treeNode {
    char *word;

    struct treeNode *right;
    struct treeNode *left;
};

struct groupNode {
    struct treeNode *treeRoot; /* root of the tree in that group */

    struct groupNode *right;
    struct groupNode *left;  
};

struct groupNode *addGroup(struct groupNode *, char *, int);
struct groupNode *allocGroup(void);

struct treeNode *addTree(struct treeNode *, char *);
struct treeNode *allocTree(void);

void printGroup(struct groupNode *);
void printTree(struct treeNode *);

int is_type_keyword(char *s);
int getvar(char *s);

char testCases[7][8] = {
    "123456e",
    "123456d",
    "234567d",
    "123456a",
    "123456g",
    "234567a",
    "234567e",
};

int main(int argc, char *argv[]){
    struct groupNode *groupRoot;
    char word[MAXWORD];
    int n;

    if (argc == 2) {
        n = atoi(argv[1]);
        if (n == 0) {
            fprintf(stderr, "invalid input.\n");
            return 1;
        }
    }
    else if (argc > 2) {
        fprintf(stderr, "invalid input.\n");
        return 1;
    }
    else {
        n = 6;
    }

    groupRoot = NULL;
    while (getvar(word) != EOF) {
        if (strlen(word) > n) {
            groupRoot = addGroup(groupRoot, word, n);
        }
    }

    printGroup(groupRoot);
    
    return 0;
}

void cut_string(char *dest, char *src, int n) {
    int i;
    for (i = 0; i < n; i++) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

struct groupNode *addGroup(struct groupNode *groupRoot, char *word, int n) {
    int cond;
    char word_part[n+1];
    char s[n+1];

    
    if (groupRoot == NULL) {
        groupRoot = allocGroup();
        groupRoot->left = groupRoot->right = NULL;
        groupRoot->treeRoot = addTree(groupRoot->treeRoot, word);
    }
    else {
        cut_string(word_part, word, n);
        cut_string(s, groupRoot->treeRoot->word, n);

        if ((cond = strcmp(word_part, s)) < 0) {
            groupRoot->left = addGroup(groupRoot->left, word, n);
        }
        else if (cond > 0) {
            groupRoot->right = addGroup(groupRoot->right, word, n);
        }
        else {
            groupRoot->treeRoot = addTree(groupRoot->treeRoot, word);
        }
    }
    return groupRoot;
}

struct groupNode *allocGroup(void) {
    return (struct groupNode *)malloc(sizeof(struct groupNode));
}

struct treeNode *addTree(struct treeNode *root, char *word) {
    int cond;
    
    if (root == NULL) {
        root = allocTree();
        root->word = strdup(word);
        root->left = root->right = NULL;
    }
    else if ((cond = strcmp(word, root->word)) < 0) {
        root->left =  addTree(root->left, word);
    }
    else if (cond > 0) {
        root->right = addTree(root->right, word);
    }

    return root;
}

struct treeNode *allocTree(void) {
    return (struct treeNode *)malloc(sizeof(struct treeNode));
}

void printGroup(struct groupNode *p) {
    if (p != NULL) {
        printGroup(p->left);
        printf("-----Group-----\n");
        printTree(p->treeRoot);
        printf("---------------\n");
        printGroup(p->right);
    }
}

void printTree(struct treeNode *p) {
    if (p != NULL) {
        printTree(p->left);
        printf("%s\n", p->word);
        printTree(p->right);
    }
}

void skip_string_constants(int c) {
    int k;

    while ((k = getchar()) != c){
        if (k == '\\'){
            getchar();
        }
    }
}

int getvar(char *s) {
    char tmp[MAXWORD];
    int datatype_reached = 0;
    int i;
    int c;

    do {
        while (isspace((c = getchar()))) { };

        if (c == EOF) break;

        if (!isalpha(c)) {
            do {
                if (c == '"' || c == '\'') {
                    skip_string_constants(c);
                }
            } while (!isalpha((c = getchar())));

            if (c == EOF) break;
        }
        
        if (isalpha(c)) {
            tmp[0] = c;
            for (i = 1; isalnum((c = getchar())); i++) {
                tmp[i] = c;
            }
            tmp[i] = '\0';

            if (c == EOF) break;

            if (c == '"' || c == '\'') {
                skip_string_constants(c);
            }

            if (is_type_keyword(tmp)) {
                datatype_reached = 1;
            }
            else if (datatype_reached == 1) {
                strcpy(s, tmp);
                return s[0];
                datatype_reached = 0;
            }
        }
    } while (c != EOF);

    return EOF;
}

int is_type_keyword(char *s) {
    return strcmp(s, "int") == 0 ||
           strcmp(s, "char") == 0 ||
           strcmp(s, "void") == 0 ||
           strcmp(s, "long") == 0 ||
           strcmp(s, "short") == 0 ||
           strcmp(s, "signed") == 0 ||
           strcmp(s, "unsigned") == 0 ||
           strcmp(s, "float") == 0 ||
           strcmp(s, "double") == 0 ||
           strcmp(s, "const") == 0 ||
           strcmp(s, "volatile") == 0;
}
