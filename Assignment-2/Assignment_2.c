#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song_node_
{
    char name[15];
    struct song_node_ *left, *right, *parent;
};

typedef struct song_node_ song_node;

void preorder_traversal(song_node *r)
{
    if (r != NULL)
    {
        printf("%s : ", r->name);
        preorder_traversal(r->left);
        preorder_traversal(r->right);
    }
}

void inorder_traversal(song_node *r)
{
    if (r != NULL)
    {
        inorder_traversal(r->left);
        printf("%s : ", r->name);
        inorder_traversal(r->right);
    }
}

song_node *insert_bst(song_node *r, char new_name[])
{
    song_node *new_node = (song_node *)malloc(sizeof(song_node));
    strcpy(new_node->name, new_name);
    new_node->left = new_node->right = new_node->parent = NULL;

    if (r != NULL)
    {
        song_node *temp = r;
        song_node *temp2 = r;

        while (temp != NULL)
        {
            temp2 = temp;

            if (strcmpi(temp->name, new_name) < 0)
            {
                temp = temp->right;
            }
            else
            {
                temp = temp->left;
            }
        }
        new_node->parent = temp2;
        if (strcmpi(temp2->name, new_name) < 0)
        {
            temp2->right = new_node;
        }
        else
        {
            temp2->left = new_node;
        }
    }
    return new_node;
}

song_node *search_bst(song_node *r, char n[])
{
    song_node *temp = r;
    while (strcmpi(temp->name, n))
    {
        if (strcmpi(temp->name, n) < 0)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }
    return temp;
}

song_node *find_successor(song_node *t)
{
    song_node *temp = t->right;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

song_node *delete_bst(song_node *r, char old_name[])
{
    song_node *old_node = search_bst(r, old_name);
    int parent_id;

    if(old_node->parent == NULL)
    {
        parent_id = -1;
    }
    else if (old_node->parent->left == old_node)
    {
        parent_id = 0;
    }
    else
    {
        parent_id = 1;
    }

    if (old_node->left == NULL && old_node->right == NULL)
    {
        if (parent_id == 0)
        {
            old_node->parent->left = NULL;
        }
        else
        {
            old_node->parent->right = NULL;
        }
    }

    else if (old_node->left == NULL || old_node->right == NULL)
    {
        if (old_node->left != NULL)
        {
            if (parent_id == 0)
            {
                old_node->parent->left = old_node->left;
            }
            else
            {
                old_node->parent->right = old_node->left;
            }
        }
        else
        {
            if (parent_id == 0)
            {
                old_node->parent->left = old_node->right;
            }
            else
            {
                old_node->parent->right = old_node->right;
            }
        }
    }
    else
    {
        song_node *next_node = find_successor(old_node);
        strcpy(old_node->name,next_node->name);

        if(next_node->parent->right = next_node)
        {
            next_node->parent->right = NULL;
        }
        else
        {
            next_node->parent->left = NULL;
        }
        old_node = next_node;
    }
    free(old_node);
}

song_node *insertion_set(song_node *r)
{
    r = insert_bst(r, "Closer");
    insert_bst(r, "Alone");
    insert_bst(r, "Believer");
    insert_bst(r, "Despacito");
    insert_bst(r, "Camila");
    insert_bst(r,"Akira");
    inorder_traversal(r);
    return r;
}

int main()
{
    song_node *root = NULL;
    root = insertion_set(root);
    delete_bst(root, "Alone");
    inorder_traversal(root);
    return EXIT_SUCCESS;
}