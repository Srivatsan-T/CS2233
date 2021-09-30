#include <stdio.h>
#include <conio.h>
#include <stdlib.h>


struct song_node
{
    char* name;
    struct song_node *parent;
    struct song_node *left;
    struct song_node *right;
};

struct song_node* root = NULL;

void song_node_constructor(struct song_node a,char* n, struct song_node *p, struct song_node *l, struct song_node *r)
{
    a.name = n;
    a.parent = p;
    a.left = l;
    a.right = r;
}

void insert_bst(struct song_node a)
{
    
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