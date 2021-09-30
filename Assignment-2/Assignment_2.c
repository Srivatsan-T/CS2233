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

void song_node_constructor(struct song_node a, char *n, struct song_node *p, struct song_node *l, struct song_node *r)
{
    a.name = n;
    a.parent = p;
    a.left = l;
    a.right = r;
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
    while (is_leaf(temp) != 0)
    {
        traverse_bst(temp->left);
        printf("%s : ", temp->name);
        traverse_bst(temp->right);
    }

    printf("%s : ", temp->name);
}

void insert_bst(struct song_node *a)
{
    struct song_node *temp = a;
    while (is_leaf(temp) != 0)
    {
        if (strcmpi(temp, a) < 0)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }
    if (strcmpi(temp, a) < 0)
    {
        temp->right = a;
        temp->left = NULL;
    }
    else
    {
        temp->left = a;
        temp->right = NULL;
    }
}

void delete_bst(struct song_node *a)
{
    struct song_node *temp = a;
    while (is_leaf(temp) != 0)
    {
        if (strcmpi(temp, a) < 0)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }
    if (strcmpi(temp, a) < 0)
    {
        temp->right = NULL;
    }
    else
    {
        temp->left = NULL;
    }
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

int main()
{
    printf("%s", read_from_file("songs.txt"));
    return EXIT_SUCCESS;
}