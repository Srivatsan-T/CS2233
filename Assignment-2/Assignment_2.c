#include <stdio.h>
#include <conio.h>
#include <stdlib.h>


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


int main()
{
    printf("%s",read_from_file("songs.txt"));
    return EXIT_SUCCESS;
}