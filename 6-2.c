#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char testCases[7][8] = {
    "123456e",
    "123456d",
    "234567d",
    "123456a",
    "123456g",
    "234567a",
    "234567e",
};

int main(void){
    struct groupNode *groupRoot;
    int n = 3;
    int i;

    groupRoot = NULL;
    for (i = 0; i < 7; i++) {
        groupRoot = addGroup(groupRoot, testCases[i], 6);
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
