#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct nlist { /* table entry */
    struct nlist *next; /* next entry in chain */ 
    char *name; /* defined name */
    char *defn; /* replacement text */
};

#define HASHSIZE 101

struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* hash: form hash value for string s */
unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) { /* not found */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;

        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else /* already there */
        free((void *) np->defn); /*free previous defn */

    if ((np->defn = strdup(defn)) == NULL)
        return NULL;

    return np;
}

void undef(char *s)
{
    struct nlist *np;
    struct nlist *prev_np;

    int hashval;

    prev_np = NULL;
    hashval = hash(s);
    for (np = hashtab[hashval]; np != NULL; np = np->next)
    {
        if (strcmp(s, np->name) == 0)
            break;
        
        prev_np = np;
    }

    if (np == NULL)
        return;

    if (prev_np == NULL)
        hashtab[hashval] = np->next;

    if (np->next != NULL)
    {
        prev_np->next = np->next;    
    }

    free((void *) np->name);
    free((void *) np->defn);
    free((void *) np);
}

/* test (just to prove) */
int main(void)
{
    install("MAX", "100");
    install("SIZE", "1000");
    install("NAME", "Ramin");

    printf("%s %s\n", lookup("NAME")->name, lookup("NAME")->defn);

    undef("SIZE");

    if (lookup("SIZE") == NULL)
        printf("SIZE does not exist\n");

    return 0;
}
