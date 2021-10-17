#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct song_node_
{
    char name[15];
    struct song_node_ *left, *right, *parent;
    int h_diff;
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
        printf("%s(%d) : ", r->name, r->h_diff);
        inorder_traversal(r->right);
    }
}

int find_height(song_node *a)
{
    if (a == NULL)
    {
        return 0;
    }
    else
    {
        int r = find_height(a->right);
        int l = find_height(a->left);

        if (r >= l)
        {
            return r + 1;
        }
        else
        {
            return l + 1;
        }
    }
}

void assign_hdiff(song_node *a)
{
    if (a != NULL)
    {
        assign_hdiff(a->left);
        a->h_diff = find_height(a->left) - find_height(a->right);
        assign_hdiff(a->right);
    }
}

song_node *ll_rot(song_node *r, song_node *t)
{
    song_node *gf = t;
    song_node *f = t->left;
    song_node *s = f->left;

    song_node *f_right_old = f->right;
    song_node *gf_parent_old = gf->parent;
    if (gf == r)
    {
        r = f;
    }
    else
    {
        if (gf->parent->right = gf)
        {
            gf->parent->right = f;
        }
        else
        {
            gf->parent->left = f;
        }
    }
    f->right = gf;
    gf->parent = f;
    f->parent = gf_parent_old;
    if (f_right_old != NULL)
    {
        f_right_old->parent = gf;
    }
    gf->left = f_right_old;
    return r;
}

song_node *rr_rot(song_node *r, song_node *t)
{
    song_node *gf = t;
    song_node *f = t->right;
    song_node *s = f->right;

    song_node *f_left_old = f->left;
    song_node *gf_parent_old = gf->parent;

    if (gf == r)
    {
        r = f;
    }
    else
    {
        if (gf->parent->right = gf)
        {
            gf->parent->right = f;
        }
        else
        {
            gf->parent->left = f;
        }
    }
    f->left = gf;
    gf->parent = f;
    f->parent = gf_parent_old;
    if (f_left_old != NULL)
    {
        f_left_old->parent = gf;
    }

    gf->right = f_left_old;
    return r;
}

song_node *lr_rot(song_node *r, song_node *t)
{
    song_node *gf = t;
    song_node *f = t->left;
    song_node *s = f->right;

    song_node *s_left_old = s->left;
    song_node *s_right_old = s->right;
    song_node *gf_parent_old = gf->parent;

    if (gf == r)
    {
        r = s;
    }
    else
    {
        if (gf->parent->left == gf)
        {
            gf->parent->left = s;
        }
        else
        {
            gf->parent->right = s;
        }
    }
    s->parent = gf_parent_old;
    s->left = f;
    s->right = gf;
    gf->parent = f->parent = s;
    if (s_right_old != NULL)
    {
        s_left_old->parent = f;
    }
    if (s_right_old != NULL)
    {
        s_right_old->parent = gf;
    }

    f->right = s_left_old;
    gf->left = s_right_old;
    return r;
}

song_node *rl_rot(song_node *r, song_node *t)
{
    song_node *gf = t;
    song_node *f = t->right;
    song_node *s = f->left;

    song_node *s_left_old = s->left;
    song_node *s_right_old = s->right;
    song_node *gf_parent_old = gf->parent;
    if (gf == r)
    {
        r = s;
    }
    else
    {
        if (gf->parent->left == gf)
        {
            gf->parent->left = s;
        }
        else
        {
            gf->parent->right = s;
        }
    }
    s->parent = gf_parent_old;
    s->right = f;
    s->left = gf;
    gf->parent = f->parent = s;
    if (s_right_old != NULL)
    {
        s_right_old->parent = f;
    }
    if (s_left_old != NULL)
    {
        s_left_old->parent = gf;
    }

    f->left = s_right_old;
    gf->right = s_left_old;

    return r;
}

song_node *avl_ins_rotations(song_node *root, song_node *new_node)
{
    song_node *temp = new_node;
    while (temp != NULL)
    {
        if (abs(temp->h_diff) == 2)
        {
            if (temp->h_diff == 2 && temp->left->h_diff == 1)
            {
                root = ll_rot(root, temp);
                break;
            }
            else if (temp->h_diff == 2 && temp->left->h_diff == -1)
            {
                root = lr_rot(root, temp);
                break;
            }
            else if (temp->h_diff == -2 && temp->right->h_diff == -1)
            {
                root = rr_rot(root, temp);
                break;
            }
            else if (temp->h_diff == -2 && temp->right->h_diff == 1)
            {
                root = rl_rot(root, temp);
                break;
            }
        }
        else
        {
            temp = temp->parent;
        }
    }
    return root;
}

song_node *avl_del_rotations(song_node *root, song_node *old_parent)
{
    song_node *temp = old_parent;
    while (temp != NULL)
    {
        if (abs(temp->h_diff) == 2)
        {
            if (temp->h_diff == 2 && temp->left->h_diff == 1)
            {
                root = ll_rot(root, temp);
            }
            else if (temp->h_diff == 2 && temp->left->h_diff == -1)
            {
                root = lr_rot(root, temp);
            }
            else if (temp->h_diff == -2 && temp->right->h_diff == -1)
            {
                root = rr_rot(root, temp);
            }
            else if (temp->h_diff == -2 && temp->right->h_diff == 1)
            {
                root = rl_rot(root, temp);
            }
            temp = temp->parent;
        }
        else
        {
            temp = temp->parent;
        }
    }
    return root;
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
        assign_hdiff(r);
        r = avl_ins_rotations(r, new_node);
        assign_hdiff(r);
        return r;
    }
    else
    {
        assign_hdiff(new_node);
        return new_node;
    }
}

song_node *search_bst(song_node *r, char n[])
{
    song_node *temp = r;
    while (temp != NULL && strcmpi(temp->name, n))
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
    song_node *next;
    if (old_node == NULL)
    {
        printf("Sorry but the song doesn't exist!\n");
        return r;
    }
    else
    {
        int parent_id;

        if (old_node->parent == NULL)
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
            else if (parent_id == 1)
            {
                old_node->parent->right = NULL;
            }
            else
            {
                r = NULL;
            }
            next = old_node->parent;
            free(old_node);
        }

        else if (old_node->left == NULL || old_node->right == NULL)
        {
            if (old_node->left != NULL)
            {
                if (parent_id == 0)
                {
                    old_node->parent->left = old_node->left;
                }
                else if (parent_id == 1)
                {
                    old_node->parent->right = old_node->left;
                }
                else
                {
                    r = old_node->left;
                    r->parent = NULL;
                }
                old_node->left->parent = old_node->parent;
                next = old_node->left;
            }
            else
            {
                if (parent_id == 0)
                {
                    old_node->parent->left = old_node->right;
                }
                else if (parent_id == 1)
                {
                    old_node->parent->right = old_node->right;
                }
                else
                {
                    r = old_node->right;
                    r->parent = NULL;
                }
                next = old_node->right;
                old_node->right->parent = old_node->parent;
            }
            free(old_node);
        }
        else
        {
            song_node *next_node = find_successor(old_node);
            next = next_node->parent;
            char next_name[20];
            strcpy(next_name, next_node->name);
            r = delete_bst(r, next_node->name);
            strcpy(old_node->name, next_name);
        }
        assign_hdiff(r);

        //r = avl_del_rotations(r, next);
        printf("Song deleted!\n");
        return r;
    }
}



char *read_from_file(char *file_name)
{
    char *file_content = malloc(50);
    FILE *fptr;
    fptr = fopen(file_name, "r");
    if (fptr != NULL)
    {
        fgets(file_content, 50, fptr);
    }
    fclose(fptr);
    return file_content;
}

song_node *pre_to_bst(song_node *r, char *songs_list)
{
    char *token = strtok(songs_list, ":");
    char *temp_token;
    while (token != NULL)
    {
        temp_token = token;
        // printf(" %s\n", token);
        token = strtok(NULL, ":");
        r = insert_bst(r, temp_token);
    }
    return r;
}

void print_node_details(song_node *root)
{
    song_node *temp = root;
    if (temp != NULL)
    {
        print_node_details(temp->left);
        printf("Node :%s ", temp->name);
        if (temp->left != NULL)
        {
            printf("with left as %s ", temp->left->name);
        }
        if (temp->right != NULL)
        {
            printf("with right as %s ", temp->right->name);
        }
        if (temp->parent != NULL)
        {
            printf("with parent %s ", temp->parent->name);
        }
        print_node_details(temp->right);
    }
    printf("\n");
}

int main()
{
    char *songs = read_from_file("songs.txt");
    song_node *root = NULL;
    root = pre_to_bst(root, songs);
    inorder_traversal(root);
    root = delete_bst(root, "Despacito");
    print_node_details(root);
    return EXIT_SUCCESS;
}