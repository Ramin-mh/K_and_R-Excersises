#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct nlist { /* table entry */
    struct nlist *next; /* next entry in chain */ 
    char *name; /* defined name */
    char *defn; /* replacement text */
};

#define HASHSIZE 101

struct nlist *hashtab[HASHSIZE]; /* pointer table */

struct nlist *install(char *name, char *defn);
struct nlist *lookup(char *s);

#define MAXWORD 100

int getword(char *w, size_t lim);
void getdefn(char *w, size_t lim);

int main(void)
{
    struct nlist *np;

    int k;
    char w[MAXWORD];
    char name[MAXWORD];
    char defn[MAXWORD];

    int hash_seen = 0;
    
    while ((k = getword(w, MAXWORD)) != EOF)
    {
        if (strlen(w) > 1)
        {
            if (k != '"' && k != '\'' && (np = lookup(w)) != NULL) {
                printf("%s ", np->defn);
            }
            else
                printf("%s ", w);
        }
        else 
            putchar(k);

        if (k == '#')
        {
            hash_seen = 1;
            continue;
        }

        if (hash_seen)
        {
            if (strcmp("define", w) == 0)
            {
                getword(name, MAXWORD);
                getdefn(defn, MAXWORD);

                if (install(name, defn) == NULL)
                    return 1;

                printf("%s ", name);
                printf("%s\n", defn);
            }
            hash_seen = 0;
        }
    }

    return 0;
}

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

#define BUFSIZE 1
static int buf[BUFSIZE];
static int top = -1;

int getch(void)
{
    return (top == -1)? getchar() : buf[top--];
}

void ungetch(int c)
{
    if (top < BUFSIZE - 1) {
        buf[++top] = c;
    }
    else {
        fprintf(stderr, "Buffer overflow!");
        exit(1);
    }
}

int getword(char *w, size_t lim)
{
    char *p = w;
    char *end = w + lim - 1;
    int c, tmp;

    while (1)
    {
        while (isspace(c = getch()) && c != '\n') /* skip spaces excluding '\n' */
            ;

        if (c == '"' || c == '\'') /* skip string literals */
        {
            *p++ = c;

            tmp = c;
            while ((c = getch()) != tmp)
            {
                *p++ = c;

                if (c == '\\')
                    *p++ = getch();

                if (c == EOF)
                    return EOF;
            }
            *p++ = tmp;
            *p = '\0';
            return w[0];
        }

        if (c == EOF)
            return EOF;

        if (!isalpha(c) && c != '_') {
            *p++ = c;
            *p = '\0';

            return c;
        }
    
        do
        {
            if (p < end)
                *p++ = c;     
        } while (isalpha((c = getch())));
        *p = '\0';

        ungetch(c);

        return w[0];
    }
}

void getdefn(char *w, size_t lim)
{
    int c;

    while (isspace((c = getchar())))
        ;

    do
    {
        *w++ = c;
    } while ((c = getchar()) != '\n' && c != EOF);
    *w = '\0';
}
