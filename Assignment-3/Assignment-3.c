#define t 10
#define MAX_ROWS 200
#define YES 1
#define NO 0

#include <stdio.h>

// Initialising primary and secondary memories
cell_node primary_memory[t * 2];
cell_node secondary_memory[MAX_ROWS][t * 2];

struct addressing_nodes_
{
    int row_number;
    int column_number;
};

typedef struct addressing_nodes_ node_address;

struct cell_node_
{
    int does_left_exist;
    int key;
};

typedef struct cell_node_ cell_node;

void readDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        primary_memory[i] = secondary_memory[i][j];
    }
}

void writeDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        secondary_memory[i][j] = primary_memory[i];
    }
}

void search_bst(int find_key)
{
    readDisk(0);
    int pres_row_num = 0;
    while (primary_memory[0].key != find_key)
    {
        if (primary_memory[0].key < find_key && primary_memory[0].left != NO)
        {
            pres_row_num = primary_memory[0].left.row_number;
            readDisk(pres_row_num);
        }
        else if (primary_memory[0].key > find_key && primary_memory[1].left != NO)
        {
            pres_row_num = primary_memory[1].left.row_number;
            readDisk(pres_row_num);
        }
    }
}

void insert_bst(int ins_key)
{
    readDisk(0);
    int pres_row_num = 0;
    while (primary_memory[0].left != NO || primary_memory[1].left != NO)
    {
        if (primary_memory[0].key < ins_key && primary_memory[0].left != NO)
        {
            pres_row_num = primary_memory[0].left.row_number;
            readDisk(pres_row_num);
        }
        else if (primary_memory[0].key > ins_key && primary_memory[1].left != NO)
        {
            pres_row_num = primary_memory[1].left.row_number;
            readDisk(pres_row_num);
        }
    }
    if (primary_memory[0].key < ins_key)
    {
        primary_memory[0].left = YES;
        writeDisk(pres_row_num);
        readDisk(2 * pres_row_num);
        primary_memory[0].key = ins_key;
        primary_memory[0].left = NO;
        primary_memory[1].left = NO;
        writeDisk(2 * pres_row_num);
    }
    else
    {
        primary_memory[1].left = YES;
        writeDisk(pres_row_num);
        readDisk(2 * pres_row_num + 1);
        primary_memory[0].key = ins_key;
        primary_memory[0].left = NO;
        primary_memory[1].left = NO;
        writeDisk(2 * pres_row_num + 1);
    }
}

int main()
{

    return 0;
}