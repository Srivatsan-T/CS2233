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

int number_children(struct song_node *a)
{
    int count = 0;
    if (a->left != NULL)
    {
        count++;
    }
    if (a->right != NULL)
    {
        count++;
    }
    return count;
}

struct song_node * find_lowest(struct song_node *a)
{
    struct song_node *temp = a;
    while(temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
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

void delete_bst(char* n)
{
    struct song_node *temp = root;
    struct song_node *temp2 = root;

    while (temp->name != n)
    {
        temp2 = temp;
        if (strcmpi(temp->name, n) < 0)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }
    if (number_children(temp) == 0)
    {
        if (strcmpi(temp2->name, n) < 0)
        {
            temp2->right = NULL;
        }
        else
        {
            temp2->left = NULL;
        }
    }

    else if (number_children(temp) == 1)
    {
        struct song_node *child = NULL;
        if (temp->right != NULL)
        {
            child = temp->right;
        }
        else
        {
            child = temp->left;
        }

        if (strcmpi(temp2->name, n) < 0)
        {
            temp2->right = child;
        }
        else
        {
            temp2->left = child;
        }
    }

    else if(number_children(temp) == 2)
    {
        char* temp_name;
        struct song_node *succ = find_lowest(temp->right);
        temp_name = succ->name;
        delete_bst(succ->name);
        temp->name = temp_name;
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
    struct song_node e;
    song_node_name_init(&e, "Camila");
    insert_bst(&e);
    traverse_bst(root);
}

void deletion_set()
{
    delete_bst("Despacito");
    traverse_bst(root);
    delete_bst("Closer");
    traverse_bst(root);
}
int main()
{
    printf("%s", read_from_file("songs.txt"));
    insertion_set();
    deletion_set();
    return EXIT_SUCCESS;
}