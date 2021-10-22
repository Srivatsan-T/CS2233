#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// The struct used for both BST and AVL trees
struct song_node_
{
    char name[15];
    struct song_node_ *left, *right, *parent;
    int height;
};

typedef struct song_node_ song_node;

// Struct used for stacks and queues once the playlist starts
struct song_linked_list_node_
{
    char name[15];
    struct song_linked_list_node_ *next;
};

typedef struct song_linked_list_node_ song_linked_list_node;

song_linked_list_node *temp_queue_front = NULL;
song_linked_list_node *temp_queue_rear = NULL;
song_linked_list_node *recents_stack_top = NULL;

// Function to insert into the stack whose top is a global variable
void insert_stack(song_linked_list_node *new_node)
{
    if (recents_stack_top == NULL)
    {
        new_node->next = NULL;
        recents_stack_top = new_node;
    }
    else
    {
        new_node->next = recents_stack_top;
        recents_stack_top = new_node;
    }
}

// Function to delete an element from the temporary stack
song_linked_list_node *delete_stack()
{
    if (recents_stack_top == NULL)
    {
        return recents_stack_top;
    }
    else if (recents_stack_top->next == NULL)
    {
        song_linked_list_node *temp_node = recents_stack_top;
        recents_stack_top = NULL;
        return temp_node;
    }
    else
    {
        song_linked_list_node *temp_node = recents_stack_top;
        recents_stack_top = recents_stack_top->next;
        return temp_node;
    }
}

// Function to print contents of the linked list
// Since stack and queue use the same struct it can be used for both of them
void print_linked_list(song_linked_list_node *t)
{
    song_linked_list_node *temp = t;
    while (temp != NULL)
    {
        printf("%s : ", temp->name);
        temp = temp->next;
    }
    printf("\n");
}

// Function to insert an element into the queue
void insert_queue(song_linked_list_node *new_node)
{
    if (temp_queue_rear == NULL)
    {
        temp_queue_front = temp_queue_rear = new_node;
        new_node->next = NULL;
    }
    else
    {
        temp_queue_rear->next = new_node;
        temp_queue_rear = new_node;
        new_node->next = NULL;
    }
}

// Function to delete an element from the queue and return the deleted element
song_linked_list_node *delete_queue()
{
    if (temp_queue_front == NULL)
    {
        return temp_queue_front;
    }
    else if (temp_queue_front == temp_queue_rear)
    {
        song_linked_list_node *temp_node = temp_queue_rear;
        temp_queue_front = temp_queue_rear = NULL;
        return temp_node;
    }
    else
    {
        song_linked_list_node *temp_node = temp_queue_front;
        temp_queue_front = temp_queue_front->next;
        return temp_node;
    }
}

// Function to print the pre-order list of a binary search tree
// Can also be used for an AVL tree since they both use the same struct
// This function isn't used in the main program but can be useful for debugging
void preorder_traversal(song_node *r)
{
    if (r != NULL)
    {
        printf("%s : ", r->name);
        preorder_traversal(r->left);
        preorder_traversal(r->right);
    }
}

// Function to print the in-order list of a binary search tree
// or an AVL tree since they use the same struct
void inorder_traversal(song_node *r)
{
    if (r != NULL)
    {
        inorder_traversal(r->left);
        printf("%s : ", r->name);
        inorder_traversal(r->right);
    }
}

// Function to get the least element from a binary search tree
// This is used to traverse the AVL playlist
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

// Function to return the height of a node in an AVL treee
// The function does it by getting the heights of the left and right of the node
// finds out what is more and assigns 1 more to the given node
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

// Function which updates the height of the nodes from song_node a
// to the root of the AVL tree
void height_node_to_root(song_node *a)
{
    song_node *temp = a;
    while (temp != NULL)
    {
        temp->height = find_height(temp);
        temp = temp->parent;
    }
}

// Function to perform a ll rotation in an AVL tree with root r
// and t being the node with imbalance.
// The function also adjusts the height of the nodes whose heights need adjusting after rotation
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

    gf->height = find_height(gf);
    f->height = find_height(f);

    return r;
}

// Function to perform a rr rotation in an AVL tree with root r
// and t being the node with imbalance.
// The function also adjusts the height of the nodes whose heights need adjusting after rotation
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

    gf->height = find_height(gf);
    f->height = find_height(f);
    return r;
}

// Function to perform a lr rotation in an AVL tree with root r
// and t being the node with imbalance.
// The function also adjusts the height of the nodes whose heights need adjusting after rotation
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

    gf->height = find_height(gf);
    f->height = find_height(f);
    s->height = find_height(s);
    return r;
}

// Function to perform a rl rotation in an AVL tree with root r
// and t being the node with imbalance.
// The function also adjusts the height of the nodes whose heights need adjusting after rotation
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

    gf->height = find_height(gf);
    f->height = find_height(f);
    s->height = find_height(s);
    return r;
}

// Function used to return the height difference of a node by returning the
// difference of its left and right node heights.
int find_h_diff(song_node *t)
{
    return find_height(t->left) - find_height(t->right);
}

// Function which handles the AVL rotation in insertion cases.
// The function goes through the tree from new node to root and searches for the node with imbalance
// It then finds the kind of imbalance and performs the correct rotation algo.
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

// Function which handles the AVL rotation in deletion cases.
// The function goes through the tree from new node to root and searches for the node with imbalance
// It then finds the kind of imbalance and performs the correct rotation algo.
// Moreover this function doesn't stop with one rotation and continues it till it reaches the root.
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
    }
    return root;
}

// Function to insert elements into a BST or an AVL tree.
// Argument choice is used to distinguish between avl trees and regular bst
// since their insertion algo is same for the most part with avl trees needing
// rotations after regular bst's insertion routine
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
            r = avl_ins_rotations(r, new_node);
        }
        return r;
    }
    else
    {
        new_node->height = 1;
        return new_node;
    }
}

// Function to search for an element in trees and return the node
// The function takes in the song's name and returns NULL if it doesn't exist in the tree
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

// Function to return the successor node to a given node in a tree.
// Used in deletion function for deleting nodes with 2 children
song_node *find_successor(song_node *t)
{
    song_node *temp = t->right;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

// Function to delete elements into a BST or an AVL tree.
// Argument choice is used to distinguish between avl trees and regular bst
// since their deletion algo is same for the most part with avl trees needing
// rotations after regular bst's deletion routine
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
            r = avl_del_rotations(r, next);
        }
        // printf("Song deleted!\n");
        return r;
    }
}

// Function to read a txt file and return its content in a string
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

// Function to split the string returned by read_from_file using the delimeter
//  ':' and inserting the individual songs into a bst. This becomes our song library
// to choose songs for playlist from
song_node *pre_to_bst(song_node *r, char *songs_list)
{
    char *token = strtok(songs_list, ":");
    char *temp_token;
    while (token != NULL)
    {
        temp_token = token;
        token = strtok(NULL, ":");
        r = insert_bst(r, temp_token, 1);
    }
    return r;
}

// Function to transfer content from the queue to the recent stack
void temp_queue_to_recent_stack()
{
    while (temp_queue_front != NULL)
    {
        insert_stack(delete_queue());
    }
}

// Function to print the node details for all the nodes in a bst.
// The function isn't used in the main program but can be used for debugging purposes to see if the nodes
// properly arranged
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

//Function to delete songs from the main library given to us from the .txt file
song_node *customise_library(song_node *root)
{
    printf("The songs in the library are\n");
    inorder_traversal(root);
    printf("\n");
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
    return root;
}

//Function to play the playlist once all wanted songs have been moved into an AVL tree
//This implements the n,p:k,e feature from Assignment 1
song_node *play_playlist(song_node *root_avl)
{
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
                insert_stack(new_node);
                printf("The recent stack is as follows!\n");
                print_linked_list(recents_stack_top);
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
                    break;
                }
                else
                {
                    song_linked_list_node *popped = delete_stack();
                    print_linked_list(recents_stack_top);
                    printf("Playing %s \n", popped->name);
                    insert_queue(popped);
                }
            }
            printf("The temporary queue is as follows!\n");
            print_linked_list(temp_queue_front);
            temp_queue_to_recent_stack();
            printf("The recent stack is as follows!\n");
            print_linked_list(recents_stack_top);
        }
        else if (strcmpi(choice, "e") == 0)
        {
            printf("Bye!\n");
            break;
        }
        printf("Do you want to play next song or n recent songs or end? (n/p:k/e) ");
        scanf("%s", &choice);
    }
    return root_avl;
}

//This function helps to delete elements from the playlist after creating them
song_node *del_playlist(song_node *root_avl)
{
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
    printf("The current playlist is as follows\n");
    inorder_traversal(root_avl);
    printf("\n");
    return root_avl;
}

int main()
{
    char *songs = read_from_file("songs.txt");
    song_node *root = NULL;
    song_node *root_avl = NULL;
    root = pre_to_bst(root, songs);
    root = customise_library(root);

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
    root_avl = del_playlist(root_avl);
    root_avl = play_playlist(root_avl);
    return EXIT_SUCCESS;
}