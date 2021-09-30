/*
    Song player with playlists using stacks and queues.
    Copyright (C) 2021 Srivatsan T

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/




#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//This struct is used for storing songs as linked list as well as for the playlist and temporary queues.
struct song_node
{
    char name[5];
    struct song_node *next;
};

//The following head pointer is for initialising the head pointer of the storing song's linked list.
struct song_node *head = NULL;

//Constructor function for the song_node class
void song_initialiser(struct song_node a, char song_name[5])
{
    strcpy(a.name, song_name);
    a.next = NULL;
}

//Push function for the linked_list
void insert_songs(struct song_node *s)
{
    if (head == NULL)
    {
        head = s;
    }
    else
    {
        struct song_node *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = s;
    }
}

//Function to get all songs from the user and push into the linked list.
void get_all_songs()
{
    char a[5];
    printf("Enter all songs (as strings), End with '-1'\n");
    while (1)
    {

        scanf("%s", a);

        if (atoi(a) == -1)
        {

            break;
        }
        else
        {

            struct song_node *insert = (struct song_node *)malloc(sizeof(struct song_node));
            strcpy(insert->name, a);
            insert->next = NULL;
            insert_songs(insert);
        }
    }
    printf("All songs entered!\n");
}

//Utility function to print the names of the songs given the head of linked list or front of queue.
void print_all_songs(struct song_node *start_node)
{
    struct song_node *temp = start_node;
    while (temp != NULL)
    {
        printf("%s : ", temp->name);
        temp = temp->next;
    }
    printf("\n");
}

//Function to search for a song in the linked_list given a string a[5] as argument
int search_songs(char a[5])
{
    struct song_node *temp = head;
    while (temp != NULL)
    {
        if (strcmpi(temp->name, a) == 0)
        {
            return 0;
        }
        temp = temp->next;
    }
    printf("Not Found in the song library\n");
    return 1;
}

//struct song_queue interface to be used in playlist queue and temporary songs queue
//It contains pointers for head and tail of the queue.
struct song_queue
{
    struct song_node *song_queue_head;
    struct song_node *song_queue_tail;
};

//Constructor function for song_queue interface.
void initialise_song_queue(struct song_queue *q)
{
    q->song_queue_head = NULL;
    q->song_queue_tail = NULL;
}

//Enque function to insert elements to the queue.
void song_queue_enque(struct song_queue *q, struct song_node *s)
{
    if (q->song_queue_tail == NULL)
    {
        q->song_queue_head = s;
        q->song_queue_tail = s;
        s->next = NULL;
    }
    else
    {
        q->song_queue_tail->next = s;
        s->next = NULL;
        q->song_queue_tail = s;
    }
}

//Deque function to delete elements from the queue.
struct song_node *song_queue_deque(struct song_queue *q)
{
    if (q->song_queue_head == NULL)
    {
        printf("Nothing to deque\n");
    }
    else if (q->song_queue_head == q->song_queue_tail)
    {
        struct song_node *prev_head = q->song_queue_head;
        q->song_queue_head = NULL;
        q->song_queue_tail = NULL;
        return prev_head;
    }
    else
    {
        struct song_node *prev_head = q->song_queue_head;
        q->song_queue_head = q->song_queue_head->next;
        return prev_head;
    }
}

//Utility function to return the size of playlist.
int song_queue_size(struct song_queue *q)
{
    int size = 0;
    struct song_node *temp = q->song_queue_head;
    while (temp != NULL)
    {
        size++;
        temp = temp->next;
    }
    return size;
}

/*
void print_song_queue(struct song_queue *q)
{
    struct song_node *temp = q->song_queue_head;
    while (temp != NULL)
    {
        printf("%s :", temp->name);
        temp = temp->next;
    }
    printf("\n");
}
*/

//Function to get songs from the list...check if its in the linked list and add it to the playlist if it is.
struct song_queue *create_playlist()
{
    struct song_queue *q = malloc(sizeof(struct song_queue));
    initialise_song_queue(q);
    char a[5];
    printf("Create Playlist, end with '-1'\n");
    while (1)
    {
        scanf("%s", a);
        if (atoi(a) == -1)
        {
            break;
        }
        else
        {
            int result = search_songs(a);
            if (result == 0)
            {
                struct song_node *insert = (struct song_node *)malloc(sizeof(struct song_node));
                strcpy(insert->name, a);
                insert->next = NULL;
                song_queue_enque(q, insert);
                printf("Found, Enqueued\n");
            }
        }
    }
    printf("Playlist ready!\n");
    printf("The playlist size is now %d\n", song_queue_size(q));
    return q;
}

//Initialising the top element of recent_songs stack.
struct song_node *recent_songs_stack_top = NULL;

//Function to push items into the recent songs stack
void recent_songs_stack_push(struct song_node *s)
{
    if (recent_songs_stack_top == NULL)
    {
        recent_songs_stack_top = s;
        s->next = NULL;
    }
    else
    {
        s->next = recent_songs_stack_top;
        recent_songs_stack_top = s;
    }
}

//Function to pop items off the recent songs stack
struct song_node *recent_songs_stack_pop()
{
    struct song_node *popped = recent_songs_stack_top;
    char name[5];
    strcpy(name, popped->name);
    recent_songs_stack_top = recent_songs_stack_top->next;
    return popped;
}

//Function to calculate size of the recent songs stack at any points
int recent_songs_stack_size()
{
    int size = 0;
    struct song_node *temp = recent_songs_stack_top;
    while (temp != NULL)
    {
        size++;
        temp = temp->next;
    }
    return size;
}

/*
void print_all_recent_songs()
{
    struct song_node *temp = recent_songs_stack_top;
    while (temp != NULL)
    {
        printf("%s :", temp->name);
        temp = temp->next;
    }
    printf("\n");
}
*/

//Function to empty the temporary queue and push all its elements into recent songs stack
void temp_queue_to_recent_stack(struct song_queue *q)
{
    while (song_queue_size(q) != 0)
    {
        recent_songs_stack_push(song_queue_deque(q));
    }
}

//Function which frees all variables created using malloc
/*
please note that whenever the songs is popped from playlist queue and inserted into recent songs 
we return the pointer to that very song's memory address so we don't free the pointer.
Similarly when we send the song from recent song's to temporary queue or vice versa we return the pointer
of the temporarily created song_node pointer so its not freed in any pop or deque function but its done
once all the operations are done.
*/
void free_all_variables(struct song_queue *a, struct song_queue *b)
{
    free(a);
    free(b);
    struct song_node *temp = recent_songs_stack_top;
    while (recent_songs_stack_top != NULL)
    {
        temp = recent_songs_stack_top;
        free(temp);
        recent_songs_stack_top = recent_songs_stack_top->next;
    }

    while (head != NULL)
    {
        struct song_node *temp = head;
        free(temp);
        head = head->next;
    }
}

//Function to play playlists.
void play_songs(struct song_queue *q)
{
    struct song_queue *temporary_queue = malloc(sizeof(struct song_queue));
    initialise_song_queue(temporary_queue);
    while (1)
    {
        char a[5];
        printf("Play next song,  previous songs or end? n/p:k/e\n");
        scanf("%s", a);
        if (strcmpi(a, "n") == 0)
        {
            if (song_queue_size(q) == 0)
            {
                printf("You've reached the end of your playlist!\n");
            }
            else
            {
                struct song_node *popped = song_queue_deque(q);
                printf("Playing %s\n", popped->name);
                recent_songs_stack_push(popped);
            }
        }
        else if (strcmpi(a, "e") == 0)
        {
            printf("Bye!\n");
            break;
        }
        // The below (optional) code can be used to check the recent stacks size and temporary queue size whenever needed
        /*
        else if(strcmpi(a,"ssize") == 0)
        {
            printf("Recent songs stack size is %d\n",recent_songs_stack_size());
            print_all_recent_songs();
        }
        else if(strcmpi(a,"tqsize") == 0)
        {
            printf("Temporary queue size is %d\n",song_queue_size(temporary_queue));
            print_song_queue(temporary_queue);
        }
        */
        else if (toupper(a[0]) == 'P' && a[1] == ':' && atoi(a + 2) != 0)
        {
            int k = atoi(a + 2);
            for (int i = 0; i < k; i++)
            {
                if (recent_songs_stack_size() == 0)
                {
                    printf("Sorry, there weren't %d recent songs\n", k);
                    break;
                }
                else
                {
                    struct song_node *popped = recent_songs_stack_pop();
                    printf("Playing %s\n", popped->name);
                    song_queue_enque(temporary_queue, popped);
                }
            }
            temp_queue_to_recent_stack(temporary_queue);
        }

        //The below code prints out the playlist queue and the recent songs stack after every iterations 
        //and can be used to check the implementation and keep track of the same.

        /*
        printf("playlist queue is ");
        print_all_songs(q->song_queue_head);
        printf("recent songs stack is ");
        print_all_songs(recent_songs_stack_top);
        */
    }
    free_all_variables(temporary_queue, q);
}

int main()
{
    get_all_songs();
    print_all_songs(head);
    struct song_queue *playlist = create_playlist();
    play_songs(playlist);
    return EXIT_SUCCESS;
}