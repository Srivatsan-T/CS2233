#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

struct song_node
{
    char *name;
    struct song_node *parent;
    struct song_node *left;
    struct song_node *right;
};

struct song_node *root = NULL;

void song_node_constructor(struct song_node *a, char *n, struct song_node *p, struct song_node *l, struct song_node *r)
{
    a->name = n;
    a->parent = p;
    a->left = l;
    a->right = r;
}

void song_node_name_init(struct song_node *a, char *n)
{
    a->name = n;
    a->parent = NULL;
    a->left = NULL;
    a->right = NULL;
}

int is_root(struct song_node *a)
{
    if (a->parent == NULL)
    {
        return 1;
    }
    return 0;
}

int is_leaf(struct song_node *a)
{
    if (a->left == NULL && a->right == NULL)
    {
        return 1;
    }
    return 0;
}

void traverse_bst(struct song_node *a)
{
    struct song_node *temp = a;

    if (temp == NULL)
    {
        return;
    }
    else if (is_leaf(temp) == 1)
    {
        printf("%s : ", temp->name);
    }
    else
    {
        traverse_bst(temp->left);
        printf("%s : ", temp->name);
        traverse_bst(temp->right);
    }
}

void insert_bst(struct song_node *a)
{
    if (root != NULL)
    {
        struct song_node *temp = root;
        struct song_node *temp2 = root;
        while (temp != NULL)
        {
            temp2 = temp;
            if (strcmpi(temp->name, a->name) < 0)
            {
                temp = temp->right;
            }
            else
            {
                temp = temp->left;
            }
        }
        if (strcmpi(temp2->name, a->name) < 0)
        {
            temp2->right = a;
        }
        else
        {
            temp2->left = a;
        }
    }
    else
    {
        root = a;
    }
}

void delete_bst(struct song_node *a)
{
    struct song_node *temp = a;
    
}

char *read_from_file(char *file_name)
{
    char *file_content = malloc(100 * sizeof(char));
    FILE *fptr;
    fptr = fopen(file_name, "r");
    if (fptr != NULL)
    {
        fgets(file_content, 100, fptr);
    }
    fclose(fptr);
    return file_content;
}

void insertion_set()
{
    struct song_node a;
    song_node_name_init(&a, "Closer");
    insert_bst(&a);
    struct song_node b;
    song_node_name_init(&b, "Alone");
    insert_bst(&b);
    struct song_node c;
    song_node_name_init(&c, "Believer");
    insert_bst(&c);
    struct song_node d;
    song_node_name_init(&d, "Despacito");
    insert_bst(&d);
    traverse_bst(root);
}

int main()
{
    printf("%s", read_from_file("songs.txt"));
    insertion_set();
    return EXIT_SUCCESS;
}