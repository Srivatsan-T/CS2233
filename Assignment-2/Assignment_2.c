#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct song_node_
{
    char name[15];
    struct song_node_ *left, *right, *parent;
    int height;
};

typedef struct song_node_ song_node;

struct song_linked_list_node_
{
    char name[15];
    struct song_linked_list_node_ *next;
};

typedef struct song_linked_list_node_ song_linked_list_node;

song_linked_list_node *insert_stack(song_linked_list_node *t, song_linked_list_node *new_node)
{
    new_node->next = t;
    return new_node;
}

song_linked_list_node *delete_stack(song_linked_list_node *t)
{
    song_linked_list_node *temp = t;
    if (temp != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

song_linked_list_node *insert_queue(song_linked_list_node *t, song_linked_list_node *new_node)
{
    song_linked_list_node *temp = t;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    song_linked_list_node *temp2 = temp->next;
    temp->next = NULL;
    return temp2;
}

song_linked_list_node *delete_queue(song_linked_list_node *t)
{
    song_linked_list_node *temp = t;
    if(temp != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

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
        printf("%s(%d) : ", r->name, r->height);
        inorder_traversal(r->right);
    }
}

song_node *get_least(song_node *r)
{
    song_node *temp = r;
    song_node *temp2 = temp;
    while (temp != NULL)
    {
        temp2 = temp;
        temp = temp->left;
    }
    return temp2;
}

int find_height(song_node *a)
{
    if (a == NULL)
    {
        return 0;
    }
    else
    {
        int r, l;
        if (a->right == NULL)
        {
            r = 0;
        }
        else
        {
            r = a->right->height;
        }
        if (a->left == NULL)
        {
            l = 0;
        }
        else
        {
            l = a->left->height;
        }
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

void height_node_to_root(song_node *a)
{
    song_node *temp = a;
    while (temp != NULL)
    {
        temp->height = find_height(temp);
        temp = temp->parent;
    }
}

/*
void assign_hdiff(song_node *a)
{
    if (a != NULL)
    {
        assign_hdiff(a->left);
        a->h_diff = find_height(a->left) - find_height(a->right);
        assign_hdiff(a->right);
    }
}
*/

song_node *ll_rot(song_node *r, song_node *t)
{
    // printf("ll rot with %s\n", t->name);
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

    gf->height = find_height(gf);
    f->height = find_height(f);

    return r;
}

song_node *rr_rot(song_node *r, song_node *t)
{
    // printf("rr rot with %s\n", t->name);
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

    gf->height = find_height(gf);
    f->height = find_height(f);
    return r;
}

song_node *lr_rot(song_node *r, song_node *t)
{
    // printf("lr rot with %s\n", t->name);
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

    gf->height = find_height(gf);
    f->height = find_height(f);
    s->height = find_height(s);
    return r;
}

song_node *rl_rot(song_node *r, song_node *t)
{
    // printf("rl rot with %s\n", t->name);
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

    gf->height = find_height(gf);
    f->height = find_height(f);
    s->height = find_height(s);
    return r;
}

int find_h_diff(song_node *t)
{
    return find_height(t->left) - find_height(t->right);
}

song_node *avl_ins_rotations(song_node *root, song_node *new_node)
{
    song_node *temp = new_node;
    int h_diff;
    while (temp != NULL)
    {
        if (abs(find_h_diff(temp)) == 2)
        {
            if (find_h_diff(temp) == 2 && find_h_diff(temp->left) == 1)
            {
                root = ll_rot(root, temp);
                break;
            }
            else if (find_h_diff(temp) == 2 && find_h_diff(temp->left) == -1)
            {
                root = lr_rot(root, temp);
                break;
            }
            else if (find_h_diff(temp) == -2 && find_h_diff(temp->right) == -1)
            {
                root = rr_rot(root, temp);
                break;
            }
            else if (find_h_diff(temp) == -2 && find_h_diff(temp->right) == 1)
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
        if (abs(find_h_diff(temp)) == 2)
        {
            if (find_h_diff(temp) == 2 && find_h_diff(temp->left) == 1)
            {
                root = ll_rot(root, temp);
            }
            else if (find_h_diff(temp) == 2 && find_h_diff(temp->left) == -1)
            {
                root = lr_rot(root, temp);
            }
            else if (find_h_diff(temp) == -2 && find_h_diff(temp->right) == -1)
            {
                root = rr_rot(root, temp);
            }
            else if (find_h_diff(temp) == -2 && find_h_diff(temp->right) == 1)
            {
                root = rl_rot(root, temp);
            }
            temp = temp->parent;
        }
        else
        {
            temp = temp->parent;
        }
        height_node_to_root(temp);
        // assign_hdiff(root);
    }
    return root;
}

song_node *insert_bst(song_node *r, char new_name[], int choice)
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
        if (choice == 1)
        {
            new_node->height = 1;
            height_node_to_root(new_node);
            // assign_hdiff(r);
            r = avl_ins_rotations(r, new_node);
            // assign_hdiff(r);
        }
        return r;
    }
    else
    {
        new_node->height = 1;
        // assign_hdiff(new_node);
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

song_node *delete_bst(song_node *r, char old_name[], int choice)
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
                old_node->right->parent = old_node->parent;
            }
            next = old_node->parent;
            free(old_node);
        }
        else
        {
            song_node *next_node = find_successor(old_node);
            next = next_node->parent;
            char next_name[20];
            strcpy(next_name, next_node->name);
            r = delete_bst(r, next_node->name, choice);
            strcpy(old_node->name, next_name);
        }
        if (choice == 1)
        {
            height_node_to_root(next);
            // assign_hdiff(r);
            r = avl_del_rotations(r, next);
        }
        printf("Song deleted!\n");
        return r;
    }
}

char *read_from_file(char *file_name)
{
    char *file_content = malloc(1000);
    FILE *fptr;
    fptr = fopen(file_name, "r");
    if (fptr != NULL)
    {
        fgets(file_content, 1000, fptr);
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
        r = insert_bst(r, temp_token, 1);
    }
    return r;
}

song_linked_list_node *temp_queue_to_recent_stack(song_linked_list_node *top, song_linked_list_node *front)
{
    while (front != NULL)
    {
        insert_stack(top, delete_queue(front));
    }
    return top;
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
    song_node *root_avl = NULL;
    song_linked_list_node *temp_queue_front = NULL;
    song_linked_list_node *temp_queue_rear = NULL;
    song_linked_list_node *recents_stack_top = NULL;
    root = pre_to_bst(root, songs);
    printf("The songs in the library are\n");
    inorder_traversal(root);
    printf("\n");
    // print_node_details(root);
    char del_choice;
    printf("Do you want to delete nodes from the library? (y/n) ");
    scanf(" %c", &del_choice);
    while (del_choice == 'y')
    {
        char del_song[15];
        printf("Enter the song you want to delete \n");
        scanf("%s", &del_song);
        song_node *temp_song = search_bst(root, del_song);
        if (temp_song != NULL)
        {
            root = delete_bst(root, del_song, 0);
        }
        else
        {
            printf("Sorry but the song entered was not in the library! \n");
        }

        printf("Do you want to delete nodes from the library? (y/n) ");
        scanf(" %c", &del_choice);
    }
    printf("The songs in library are now \n");
    inorder_traversal(root);
    printf("\n");
    printf("Library is now customised time to create a playlist you love! \n");
    char ins_choice;
    printf("Do you want to insert nodes from the library into the playlist? (y/n) ");
    scanf(" %c", &ins_choice);
    while (ins_choice == 'y')
    {
        char ins_song[15];
        printf("Enter the song you want to enter \n");
        scanf("%s", &ins_song);
        song_node *temp_song = search_bst(root, ins_song);
        if (temp_song == NULL)
        {
            printf("Sorry but the song is not in the library!\n ");
        }
        else
        {
            root = delete_bst(root, ins_song, 0);
            root_avl = insert_bst(root_avl, ins_song, 1);
        }
        printf("Do you want to insert nodes from the library into the playlist? (y/n) ");
        scanf(" %c", &ins_choice);
    }
    printf("The songs in the playlist are\n");
    inorder_traversal(root_avl);
    printf("\n");
    char del_playlist;
    printf("Do you want to delete songs from the playlist? (y/n) ");
    scanf(" %c", &del_playlist);
    while (del_playlist == 'y')
    {
        char del_song[15];
        printf("Enter the song you want to delete \n");
        scanf("%s", &del_song);
        song_node *temp_song = search_bst(root_avl, del_song);
        if (temp_song == NULL)
        {
            printf("Sorry but the song is not present in the playlist \n");
        }
        else
        {
            root_avl = delete_bst(root_avl, del_song, 1);
        }
        printf("Do you want to delete more songs from the playlist? (y/n) ");
        scanf(" %c", &del_playlist);
    }
    print_node_details(root_avl);
    printf("Now lets start the playlist and listen to some music! \n");
    char choice[4];
    printf("Do you want to play next song or n recent songs or end? (n/p:k/e) ");
    scanf("%s", &choice);
    while (1)
    {
        if (strcmpi(choice, "n") == 0)
        {
            if (root_avl == NULL)
            {
                printf("Sorry you've reached the end of playlist\n");
            }
            else
            {
                song_linked_list_node *new_node = malloc(sizeof(song_linked_list_node));
                strcpy(new_node->name, get_least(root_avl)->name);
                root_avl = delete_bst(root_avl, new_node->name, 1);
                printf("Playing %s \n", new_node->name);
                recents_stack_top = insert_stack(recents_stack_top, new_node);
            }
        }
        else if (choice[0] == 'p' && choice[1] == ':' && atoi(choice + 2) != 0)
        {
            int k = atoi(choice + 2);
            for (unsigned i = 0; i < k; i++)
            {
                if (recents_stack_top == NULL)
                {
                    printf("Sorry but there weren't %d recent songs\n", k);
                }
                else
                {
                    song_linked_list_node *popped = recents_stack_top;
                    recents_stack_top = delete_stack(recents_stack_top);
                    printf("Playing %s \n", popped->name);
                    insert_queue(temp_queue_front, popped);
                }
            }
            recents_stack_top = temp_queue_to_recent_stack(recents_stack_top, temp_queue_front);
        }
        else if (strcmpi(choice, "e") == 0)
        {
            printf("Bye!\n");
            break;
        }
        printf("Do you want to play next song or n recent songs or end? (n/p:k/e) ");
        scanf("%s", &choice);
    }
    // print_node_details(root);
    return EXIT_SUCCESS;
}