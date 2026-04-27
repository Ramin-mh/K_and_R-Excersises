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

int main(void){
    struct groupNode *groupRoot;
    int n = 6;

    groupRoot = NULL;

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
    else if ((cond = strcmp(word, root->word) < 0)) {
        root->left =  addTree(root->left, word);
    }
    else if (cond > 0) {
        return addTree(root->right, word);
    }

    return root;
}

struct treeNode *allocTree(void) {
    return (struct treeNode *)malloc(sizeof(struct treeNode));
}
