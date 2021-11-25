#define t 10
#define MAX_ROWS 200
#define YES 1
#define NO 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
*
*
*
*The bst implementation takes around 10-15 seconds to run and produce results. Feel free to wait out the time.
*It further proves the point that bst are unsuitable for these number of keys.
*
*
*
*/

// global variables to record the read_cost,write_cost and primart_access_costs
int read_cost;
int write_cost;
int primary_access_cost;

// struct to store row and column as address for a node's left child
// or parent
struct addressing_nodes_
{
    int row_number;
    int column_number;
};

typedef struct addressing_nodes_ node_address;

// struct to store the key,left address and right address of a node
// contains an integer and 2 node_address elements.
struct cell_node_
{
    node_address left_node;
    int key;
    node_address parent_node;
};

typedef struct cell_node_ cell_node;

// Initialising primary and secondary memories of sizes 4*t and [MAX_ROWS][2t] respectively
// The primary memory can contain 2 rows of secondary memory at a time.
cell_node primary_memory[t * 4];
cell_node secondary_memory[MAX_ROWS][t * 2];

// Function to read the i'th row of secondary memory array and store in first 2*t cells of primary
void readDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        primary_memory[j] = secondary_memory[i][j];
    }
    read_cost += 10;
}

// Function to read the i'th and j'th rows of secondary memory and store in 4*t cells of primary
void read2(int i, int j)
{
    for (unsigned k = 0; k < 2 * t; k++)
    {
        primary_memory[k] = secondary_memory[i][k];
    }
    for (unsigned k = 2 * t; k < 4 * t; k++)
    {
        primary_memory[k] = secondary_memory[j][k - 2 * t];
    }
    read_cost += 10;
}

// Function to write the primary memory's first 2*t cells into i'th row of secondary memory.
void writeDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        secondary_memory[i][j] = primary_memory[j];
    }
    write_cost += 10;
}

// Function to write the primary memory into 2 rows i and j of the secondary memory.
void write2(int i, int j)
{
    for (unsigned k = 0; k < 2 * t; k++)
    {
        secondary_memory[i][k] = primary_memory[k];
    }
    for (unsigned k = 2 * t; k < 4 * t; k++)
    {
        secondary_memory[j][k - 2 * t] = primary_memory[k];
    }
    write_cost += 10;
}

// Creating a global variable to keep track of the root row of btree and the last occupied row
// of btree in secondary memory
int btree_row_num = 0;
int btree_root_row = 0;

// Function to traverse bst using recursion
void traverse_bst(int str_row)
{
    readDisk(str_row);
    primary_access_cost += 2;
    if (primary_memory[0].left_node.row_number == -1 && primary_memory[1].left_node.row_number == -1)
    {
        printf("%d : ", primary_memory[0].key);
    }
    else if (primary_memory[0].left_node.row_number == -1)
    {
        primary_access_cost++;
        printf("%d : ", primary_memory[0].key);
        traverse_bst(primary_memory[1].left_node.row_number);
        // We have to restore the primary memory here since the recursive call tampers with the primary memory
        readDisk(str_row);
    }
    else if (primary_memory[1].left_node.row_number == -1)
    {
        primary_access_cost++;
        traverse_bst(primary_memory[0].left_node.row_number);
        readDisk(str_row);
        printf("%d : ", primary_memory[0].key);
    }
    else
    {
        traverse_bst(primary_memory[0].left_node.row_number);
        readDisk(str_row);
        printf("%d : ", primary_memory[0].key);
        traverse_bst(primary_memory[1].left_node.row_number);
        readDisk(str_row);
    }
}

// Function to search for an element in a bst
// The function goes through the tree and returns the row number of the corresponding
// element where it's found.
int search_bst(int find_key)
{
    readDisk(0);
    int pres_row_num = 0;
    while (primary_memory[0].key != find_key)
    {
        primary_access_cost += 3;
        if (primary_memory[0].key < find_key && primary_memory[1].left_node.row_number != -1)
        {
            pres_row_num = primary_memory[1].left_node.row_number;
            readDisk(pres_row_num);
        }
        else if (primary_memory[0].key > find_key && primary_memory[0].left_node.row_number != -1)
        {
            primary_access_cost += 2;
            pres_row_num = primary_memory[0].left_node.row_number;
            readDisk(pres_row_num);
        }
    }
    primary_access_cost++;
    return pres_row_num;
}

// Function to insert elements into the bst
void insert_bst(int ins_key, int row_num)
{
    readDisk(0);
    int pres_row_num = 0;
    while (primary_memory[0].left_node.row_number != -1 || primary_memory[1].left_node.row_number != -1)
    {
        primary_access_cost += 3;
        if (primary_memory[0].key < ins_key)
        {
            primary_access_cost++;
            if (primary_memory[1].left_node.row_number != -1)
            {
                primary_access_cost++;
                pres_row_num = primary_memory[1].left_node.row_number;
                readDisk(pres_row_num);
            }
            else
            {
                break;
            }
        }
        else if (primary_memory[0].key > ins_key)
        {
            primary_access_cost += 2;
            if (primary_memory[0].left_node.row_number != -1)
            {
                primary_access_cost++;
                pres_row_num = primary_memory[0].left_node.row_number;
                readDisk(pres_row_num);
            }
            else
            {
                break;
            }
        }
    }
    primary_access_cost += 3;
    if (primary_memory[0].key == -1)
    {
        primary_access_cost += 2;
        primary_memory[0].key = ins_key;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = -1;
        writeDisk(pres_row_num);
    }
    else if (primary_memory[0].key < ins_key)
    {
        primary_access_cost += 6;
        primary_memory[1].left_node.row_number = row_num;
        writeDisk(pres_row_num);
        readDisk(row_num);
        primary_memory[0].key = ins_key;
        primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = pres_row_num;
        writeDisk(row_num);
    }
    else
    {
        primary_access_cost += 4;
        primary_memory[0].left_node.row_number = row_num;
        writeDisk(pres_row_num);
        readDisk(row_num);
        primary_memory[0].key = ins_key;
        primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = pres_row_num;
        writeDisk(row_num);
    }
}

// Function to find successor of a key in a given btree
// The function traverses the right subtree and keeps going
// left to find the smallest key in the subtree
int find_successor(int find_key)
{
    int succ_row;
    primary_access_cost++;
    succ_row = primary_memory[1].left_node.row_number;
    readDisk(succ_row);
    while (primary_memory[0].left_node.row_number != -1)
    {
        primary_access_cost += 2;
        succ_row = primary_memory[0].left_node.row_number;
        readDisk(succ_row);
    }
    primary_access_cost++;
    return succ_row;
}

// Function to delete an element from a given bst
void delete_bst(int del_key)
{
    int row = search_bst(del_key);
    readDisk(row);
    primary_access_cost += 2;
    if (primary_memory[0].left_node.row_number == -1 && primary_memory[1].left_node.row_number == -1)
    {
        primary_access_cost++;
        int parent_row = primary_memory[0].parent_node.row_number;
        readDisk(parent_row);
        primary_access_cost++;
        if (primary_memory[0].left_node.row_number == row)
        {
            primary_access_cost++;
            primary_memory[0].left_node.row_number = -1;
        }
        else if (primary_memory[1].left_node.row_number == row)
        {
            primary_access_cost += 2;
            primary_memory[1].left_node.row_number = -1;
        }
        writeDisk(parent_row);
        readDisk(row);
        primary_access_cost += 5;
        primary_memory[0].key = -1;
        primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = -1;
        writeDisk(row);
    }
    else if (primary_memory[0].left_node.row_number == -1 || primary_memory[1].left_node.row_number == -1)
    {
        primary_access_cost += 4;
        int child_row;
        int parent_row = primary_memory[0].parent_node.row_number;
        if (primary_memory[0].left_node.row_number == -1)
        {
            primary_access_cost++;
            child_row = primary_memory[1].left_node.row_number;
        }
        else if (primary_memory[1].left_node.row_number == -1)
        {
            primary_access_cost += 2;
            child_row = primary_memory[0].left_node.row_number;
        }
        readDisk(child_row);
        int child_of_child;
        primary_access_cost++;
        if (primary_memory[0].left_node.row_number != -1)
        {
            primary_access_cost += 3;
            child_of_child = primary_memory[0].left_node.row_number;
            readDisk(child_of_child);
            primary_memory[0].parent_node.row_number = row;
            primary_memory[1].parent_node.row_number = row;
            writeDisk(child_of_child);
        }
        primary_access_cost++;
        if (primary_memory[1].left_node.row_number != -1)
        {
            primary_access_cost += 4;
            child_of_child = primary_memory[1].left_node.row_number;
            child_of_child = primary_memory[0].left_node.row_number;
            readDisk(child_of_child);
            primary_memory[0].parent_node.row_number = row;
            primary_memory[1].parent_node.row_number = row;
            writeDisk(child_of_child);
        }
        readDisk(child_row);
        primary_access_cost += 4;
        primary_memory[0].parent_node.row_number = parent_row;
        primary_memory[1].parent_node.row_number = parent_row;
        writeDisk(row);
        primary_memory[0].key = primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = -1;
        writeDisk(child_row);
    }
    else
    {
        primary_access_cost += 2;
        int succ_row = find_successor(del_key);
        readDisk(succ_row);
        int succ_key = primary_memory[0].key;
        delete_bst(succ_key);
        readDisk(row);
        primary_memory[0].key = succ_key;
        writeDisk(row);
    }
}

// Function to test if a given row corresponds to a leaf node in a btree
// The function tests if the left subtree of the first key is nil
int is_leaf(int row_num)
{
    readDisk(row_num);
    primary_access_cost++;
    if (primary_memory[0].left_node.row_number == -1)
    {
        return YES;
    }
    return NO;
}

// Function to test if a given row corresponds to a root node in a btree
// The function tests if the parent of the first key is nil
int is_root(int row_num)
{
    readDisk(row_num);
    primary_access_cost++;
    if (primary_memory[0].parent_node.row_number == -1)
    {
        return YES;
    }
    return NO;
}

// Function to return the number of keys present in a given row
// It iterates through the keys till it encounters a -1 key
int num_nodes_in_row(int row_num)
{
    int num_nodes = 0;
    int i = 0;
    while (primary_memory[i].key != -1)
    {
        primary_access_cost++;
        num_nodes++;
        i++;
    }
    primary_access_cost++;
    return num_nodes;
}

// Function to traverse a given btree and print all its elements in in orderly manner
void traverse_btree(int str_row)
{
    if (is_leaf(str_row) == YES)
    {
        for (unsigned i = 0; primary_memory[i].key != -1; i++)
        {
            primary_access_cost += 2;
            printf("%d :", primary_memory[i].key);
        }
    }
    else
    {
        int count = num_nodes_in_row(str_row);
        for (unsigned i = 0; i <= count; i++)
        {
            primary_access_cost++;
            traverse_btree(primary_memory[i].left_node.row_number);
            readDisk(str_row);
            primary_access_cost++;
            if (primary_memory[i].key != -1)
            {
                primary_access_cost++;
                printf("%d :", primary_memory[i].key);
            }
        }
    }
}

//Function to insert ins_key in the row row_num and sorts it in place
//It creats a vacant node and defines its key as ins_key.
//Point to be noted is that the left address is NULL for this new node.
int sort_row(int row_num, int ins_key)
{
    int i = 0;
    while (primary_memory[i].key < ins_key && primary_memory[i].key != -1)
    {
        primary_access_cost += 2;
        i++;
    }
    primary_access_cost++;
    int j, k;
    for (j = 0; j < 2 * t; j++)
    {
        primary_access_cost++;
        if (primary_memory[j].key == -1)
            break;
    }
    for (k = j; k >= i; k--)
    {
        primary_access_cost++;
        primary_memory[k + 1] = primary_memory[k];
    }
    primary_access_cost++;
    primary_memory[i].key = ins_key;
    writeDisk(row_num);
    return i;
}

// Function to test if the key find_key exists in row_num row.
// Returns 1 if exists else returns 0
int key_in_row(int row_num, int find_key)
{
    readDisk(row_num);
    for (unsigned i = 0; i < 2 * t - 1; i++)
    {
        primary_access_cost++;
        if (primary_memory[i].key == find_key)
        {
            return 1;
        }
    }
    return 0;
}

// Function to search for an element in the btree
// Loops through the btree and finds the row and column of the element if present
// Stores it in a node_address pointer and returns it.
node_address *search_btree(int find_key)
{
    int pres_row_num = btree_root_row;
    unsigned i;
    while (key_in_row(pres_row_num, find_key) == 0)
    {
        for (i = 0; i < 2 * t - 1; i++)
        {
            primary_access_cost += 2;
            if (primary_memory[i].key < find_key && primary_memory[i + 1].key > find_key)
            {
                primary_access_cost++;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
            }
            else if (primary_memory[i].key < find_key && primary_memory[i + 1].key == -1)
            {
                primary_access_cost += 3;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
            }
            else if (primary_memory[i].key > find_key && i == 0)
            {
                primary_access_cost += 2;
                pres_row_num = primary_memory[i].left_node.row_number;
            }
        }
    }
    node_address *add = malloc(sizeof(node_address));
    add->row_number = pres_row_num;
    for (unsigned i = 0; i < 2 * t - 1; i++)
    {
        primary_access_cost++;
        if (primary_memory[i].key == find_key)
        {
            add->column_number = i;
            break;
        }
    }
    return add;
}

//The function splits the row row_num into 2 halves and stores one half in new_row_num
//and keys of the remaining half becomes -1
//It alters the parent pointer of elements in new_row_num accordingly.
void spilt_node(int row_num, int new_row_num)
{
    primary_access_cost++;
    cell_node temp = primary_memory[9];
    node_address temp_parent = primary_memory[9].parent_node;
    writeDisk(new_row_num);
    for (unsigned i = t - 1; i < 2 * t; i++)
    {
        primary_access_cost += 2;
        primary_memory[i].key = -1;
        primary_memory[i].left_node.row_number = -1;
    }
    primary_access_cost++;
    primary_memory[t - 1].left_node = temp.left_node;
    writeDisk(row_num);
    readDisk(new_row_num);
    for (int i = 0; i < t - 1; i++)
    {
        primary_access_cost += 4;
        primary_memory[i] = primary_memory[i + 10];
        primary_memory[i].parent_node = temp_parent;
        primary_memory[i + 10].key = -1;
        primary_memory[i + 10].left_node.row_number = -1;
    }
    primary_access_cost += 2;
    primary_memory[t - 1].key = primary_memory[t - 1].left_node.row_number = -1;
    writeDisk(new_row_num);

    if (temp_parent.row_number != -1)
    {
        primary_access_cost += 2;
        readDisk(temp_parent.row_number);
        int col = sort_row(temp_parent.row_number, temp.key);
        primary_memory[col].left_node.row_number = row_num;
        primary_memory[col + 1].left_node.row_number = new_row_num;
        writeDisk(temp_parent.row_number);
    }
    else
    {
        btree_root_row = ++btree_row_num;
        readDisk(row_num);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            primary_memory[i].parent_node.row_number = btree_root_row;
        }
        writeDisk(row_num);
        readDisk(new_row_num);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            primary_memory[i].parent_node.row_number = btree_root_row;
        }
        writeDisk(new_row_num);

        readDisk(btree_root_row);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            primary_memory[i].parent_node.row_number = -1;
        }
        primary_access_cost += 4;
        primary_memory[0].key = temp.key;
        primary_memory[0].left_node.row_number = row_num;
        primary_memory[1].left_node.row_number = new_row_num;
        primary_memory[1].key = -1;
        writeDisk(btree_root_row);
    }
}

// Function to insert a element ins_key in the btree.
//It does a single pass meaning it checks if the node overflows in advance
//before traversing it fixes it and moves on to next step.
void insert_btree(int ins_key)
{
    int pres_row_num = btree_root_row;
    while (is_leaf(pres_row_num) != YES)
    {
        if (num_nodes_in_row(pres_row_num) < 2 * t - 1)
        {
            for (unsigned i = 0; i < 2 * t - 1; i++)
            {
                primary_access_cost += 2;
                if (primary_memory[i].key < ins_key && primary_memory[i + 1].key > ins_key)
                {
                    primary_access_cost++;
                    pres_row_num = primary_memory[i + 1].left_node.row_number;
                    break;
                }
                else if (primary_memory[i].key < ins_key && primary_memory[i + 1].key == -1)
                {
                    primary_access_cost += 3;
                    pres_row_num = primary_memory[i + 1].left_node.row_number;
                    break;
                }
                else if (primary_memory[i].key > ins_key && i == 0)
                {
                    primary_access_cost += 2;
                    pres_row_num = primary_memory[i].left_node.row_number;
                    break;
                }
            }
        }
        else
        {
            primary_access_cost++;
            node_address temp_parent = primary_memory[0].parent_node;
            spilt_node(pres_row_num, ++btree_row_num);
            pres_row_num = temp_parent.row_number;
            readDisk(pres_row_num);
        }
    }
    //Insertion is done at the leaf so checking leaf for overflow and splitting node if it does
    if (num_nodes_in_row(pres_row_num) < 2 * t - 1)
    {
        sort_row(pres_row_num, ins_key);
    }
    else
    {
        primary_access_cost++;
        node_address temp_left = primary_memory[0].parent_node;
        spilt_node(pres_row_num, ++btree_row_num);
        if (temp_left.row_number != -1)
        {
            readDisk(temp_left.row_number);
        }
        else
        {
            readDisk(btree_root_row);
        }
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            primary_access_cost += 2;
            if (primary_memory[i].key < ins_key && primary_memory[i + 1].key > ins_key)
            {
                primary_access_cost++;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
            else if (primary_memory[i].key < ins_key && primary_memory[i + 1].key == -1)
            {
                primary_access_cost += 3;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
            else if (primary_memory[i].key > ins_key && i == 0)
            {
                primary_access_cost += 2;
                pres_row_num = primary_memory[i].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
        }
        sort_row(pres_row_num, ins_key);
    }
}

// Function to remove an element ele_key from the row row_num
// The function removes the key and corresponding left child address from the row.
// Returns the node
cell_node remove_element_from_row(int row_num, int ele_key)
{
    readDisk(row_num);
    int i;
    for (i = 0; i < 2 * t; i++)
    {
        primary_access_cost++;
        if (primary_memory[i].key == ele_key)
        {
            break;
        }
    }
    cell_node ret = primary_memory[i];
    for (unsigned j = i; j < 2 * t - 1; j++)
    {
        primary_access_cost++;
        primary_memory[j] = primary_memory[j + 1];
    }
    writeDisk(row_num);
    return ret;
}

//Function to get a node from the right sibling of a row child_row
//Gets the first element from right_sib and deletes it from that row
//It then send the element to the parent_row's parent's col position and gets that element
//to the child_row to prevent underflow
void get_from_right_sib(int parent_row, int parent_col, int right_sib, int child_row)
{
    primary_access_cost += 6;
    readDisk(right_sib);
    int del_key = primary_memory[0].key;
    cell_node right_cell = remove_element_from_row(right_sib, del_key);
    readDisk(parent_row);
    int parent_key = primary_memory[parent_col].key;
    primary_memory[parent_col].key = right_cell.key;
    writeDisk(parent_row);
    readDisk(child_row);
    int num_nodes = num_nodes_in_row(child_row);
    primary_memory[num_nodes].key = parent_key;
    primary_memory[num_nodes + 1].left_node.row_number = right_cell.left_node.row_number;
    primary_memory[num_nodes + 1].key = -1;
    writeDisk(child_row);
    readDisk(child_row);
}

//Function to get a node from the left sibling of a row child_row
//Gets the last element from left_sib and deletes it from that row
//It then send the element to the parent_row's parent's col position and gets that element
//to the child_row to prevent underflow
void get_from_left_sub(int parent_row, int parent_col, int left_sib, int child_row)
{
    primary_access_cost += 6;
    readDisk(left_sib);
    cell_node left_cell;
    left_cell.key = primary_memory[num_nodes_in_row(left_sib) - 1].key;
    left_cell.left_node.row_number = primary_memory[num_nodes_in_row(left_sib)].left_node.row_number;
    primary_memory[num_nodes_in_row(left_sib) - 1].key = -1;
    writeDisk(left_sib);
    readDisk(parent_row);
    int parent_key = primary_memory[parent_col].key;
    primary_memory[parent_col].key = left_cell.key;
    writeDisk(parent_row);
    readDisk(child_row);
    sort_row(child_row, parent_key);
    primary_memory[0].left_node.row_number = left_cell.left_node.row_number;
    writeDisk(child_row);
    readDisk(child_row);
}

//Function to merge child_rows's element into right_sib's element to prevent underflow during deletion
int merge_with_right(int parent_row, int parent_col, int child_row, int right_sib)
{
    readDisk(parent_row);
    primary_access_cost++;
    int parent_key = primary_memory[parent_col].key;
    if (is_leaf(child_row) == NO)
    {
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            int new_row_num = primary_memory[i].left_node.row_number;
            readDisk(new_row_num);
            for (unsigned j = 0; primary_memory[j].key != -1; j++)
            {
                primary_access_cost += 2;
                primary_memory[i].parent_node.row_number = right_sib;
            }
            writeDisk(new_row_num);
        }
    }
    read2(child_row, right_sib);
    primary_access_cost++;
    primary_memory[t - 1].key = parent_key;
    for (unsigned i = 2 * t; i <= 3 * t - 1; i++)
    {
        primary_access_cost++;
        primary_memory[i - t] = primary_memory[i];
    }
    writeDisk(right_sib);
    if (is_root(parent_row) == YES && num_nodes_in_row(parent_row) == 1)
    {
        btree_root_row = right_sib;
        readDisk(right_sib);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            primary_memory[i].parent_node.row_number = -1;
        }
        writeDisk(right_sib);
    }
    remove_element_from_row(parent_row, parent_key);
    return parent_key;
}

//Function to merge child_rows's element into left_sib's element to prevent underflow during deletion
int merge_with_left(int parent_row, int parent_col, int child_row, int left_sib)
{
    readDisk(parent_row);
    primary_access_cost++;
    cell_node parent = primary_memory[parent_col - 1];
    if (is_leaf(child_row) == NO)
    {
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            int new_row_num = primary_memory[i].left_node.row_number;
            readDisk(new_row_num);
            for (unsigned j = 0; primary_memory[j].key != -1; j++)
            {
                primary_access_cost += 2;
                primary_memory[i].parent_node.row_number = left_sib;
            }
            writeDisk(new_row_num);
        }
    }
    read2(left_sib, child_row);
    primary_access_cost++;
    primary_memory[t - 1].key = parent.key;
    for (unsigned i = 2 * t; i <= 3 * t - 1; i++)
    {
        primary_access_cost++;
        primary_memory[i - t] = primary_memory[i];
    }
    writeDisk(left_sib);
    if (is_root(parent_row) == YES && num_nodes_in_row(parent_row) == 1)
    {
        btree_root_row = left_sib;
        readDisk(left_sib);
        remove_element_from_row(left_sib, parent.key);
        primary_access_cost++;
        primary_memory[parent_col].left_node.row_number = parent.left_node.row_number;
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            primary_memory[i].parent_node.row_number = -1;
        }
        writeDisk(left_sib);
    }
    else
    {
        primary_access_cost++;
        readDisk(parent_row);
        remove_element_from_row(parent_row, parent.key);
        primary_memory[parent_col - 1].left_node.row_number = parent.left_node.row_number;
        writeDisk(parent_row);
    }
    return parent.key;
}

//Function to find predecessor of a key in the btree and returns it;
int delete_predecessor(int row_num, int col, int key)
{
    readDisk(row_num);
    primary_access_cost++;
    int pres_row_num = primary_memory[col].left_node.row_number;
    while (is_leaf(pres_row_num) != YES)
    {
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            primary_access_cost += 2;
            if (primary_memory[i].key < key && primary_memory[i + 1].key > key)
            {
                primary_access_cost++;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
            }
            else if (primary_memory[i].key < key && primary_memory[i + 1].key == -1)
            {
                primary_access_cost += 3;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
            }
            else if (primary_memory[i].key > key && i == 0)
            {
                primary_access_cost += 2;
                pres_row_num = primary_memory[i].left_node.row_number;
            }
        }
    }
    primary_access_cost++;
    int del_key = primary_memory[num_nodes_in_row(pres_row_num) - 1].key;
    return del_key;
}

//Function to return the successor of a key in the btree and returns it
int delete_successor(int row_num, int col, int key)
{
    readDisk(row_num);
    primary_access_cost++;
    int pres_row_num = primary_memory[col].left_node.row_number;
    while (is_leaf(pres_row_num) != YES)
    {
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            primary_access_cost += 2;
            if (primary_memory[i].key < key && primary_memory[i + 1].key > key)
            {
                primary_access_cost++;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
            }
            else if (primary_memory[i].key < key && primary_memory[i + 1].key == -1)
            {
                primary_access_cost += 3;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
            }
            else if (primary_memory[i].key > key && i == 0)
            {
                primary_access_cost += 2;
                pres_row_num = primary_memory[i].left_node.row_number; 
            }
        }
    }
    primary_access_cost++;
    int del_key = primary_memory[0].key;
    return del_key;
}

//The function merges left_sib and right_sib and writes it in right_sib and includes the parent_key in it.
int merge_final(int row_num, int left_sib, int right_sib, int parent_key)
{
    read2(left_sib, right_sib);
    primary_access_cost++;
    primary_memory[t - 1].key = parent_key;
    for (unsigned i = 2 * t; i <= 3 * t - 1; i++)
    {
        primary_access_cost++;
        primary_memory[i - t] = primary_memory[i];
    }
    writeDisk(right_sib);
    if (is_root(row_num) == YES && num_nodes_in_row(row_num) == 1)
    {
        btree_root_row = right_sib;
        readDisk(right_sib);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_access_cost++;
            primary_memory[i].parent_node.row_number = -1;
        }
        writeDisk(right_sib);
    }
    remove_element_from_row(row_num, parent_key);
    return parent_key;
}

//Function to delete element del_key from the btree
//It takes care of underflows in traversals and borrowes from siblings
//and merges the nodes if both the siblings underflow
void delete_btree(int del_key)
{
    int pres_row_num = btree_root_row;
    while (key_in_row(pres_row_num, del_key) != 1)
    {
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            primary_access_cost += 2;
            if (primary_memory[i].key < del_key && primary_memory[i + 1].key > del_key)
            {
                primary_access_cost++;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                break;
            }
            else if (primary_memory[i].key < del_key && primary_memory[i + 1].key == -1)
            {
                primary_access_cost += 3;
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                break;
            }
            else if (primary_memory[i].key > del_key && i == 0)
            {
                primary_access_cost += 2;
                pres_row_num = primary_memory[i].left_node.row_number;
                break;
            }
        }
        readDisk(pres_row_num);
        if (num_nodes_in_row(pres_row_num) < t)
        {
            // update pres_row_num to keep loop alive
            primary_access_cost++;
            int parent_node = primary_memory[0].parent_node.row_number;
            int parent_column;
            readDisk(parent_node);
            for (parent_column = 0; parent_column < 2 * t; parent_column++)
            {
                primary_access_cost++;
                if (primary_memory[parent_column].left_node.row_number == pres_row_num)
                {
                    break;
                }
            }
            int right_sib, left_sib;
            // parent_column now has the child's pointer
            if (parent_column == 0)
            {
                // only right sibling
                primary_access_cost++;
                right_sib = primary_memory[parent_column + 1].left_node.row_number;
                int child = primary_memory[parent_column].left_node.row_number;
                readDisk(right_sib);
                if (num_nodes_in_row(right_sib) >= t)
                {
                    // get from right_sib
                    get_from_right_sib(parent_node, parent_column, right_sib, pres_row_num);
                }
                else
                {
                    // merge
                    merge_with_right(parent_node, parent_column, child, right_sib);
                    if (is_root(parent_node) == YES && num_nodes_in_row(parent_node) == 0)
                    {
                        readDisk(right_sib);
                    }
                    else
                    {
                        readDisk(parent_node);
                    }
                }
            }
            else if (parent_column == num_nodes_in_row(parent_node))
            {
                // only left sibling
                primary_access_cost++;
                left_sib = primary_memory[parent_column - 1].left_node.row_number;
                int child = primary_memory[parent_column].left_node.row_number;
                readDisk(left_sib);
                if (num_nodes_in_row(left_sib) >= t)
                {
                    // get from left
                    get_from_left_sub(parent_node, parent_column - 1, left_sib, pres_row_num);
                }
                else
                {
                    // merge
                    merge_with_left(parent_node, parent_column, child, left_sib);
                    if (is_root(parent_node) == YES)
                    {
                        readDisk(left_sib);
                    }
                    else
                    {
                        readDisk(parent_node);
                    }
                }
            }
            else
            {
                // 2 siblings
                primary_access_cost += 2;
                left_sib = primary_memory[parent_column - 1].left_node.row_number;
                right_sib = primary_memory[parent_column + 1].left_node.row_number;
                int child = primary_memory[parent_column].left_node.row_number;
                readDisk(left_sib);
                int num_left = num_nodes_in_row(left_sib);
                readDisk(right_sib);
                int num_right = num_nodes_in_row(right_sib);
                if (num_left >= t)
                {
                    // get from left
                    get_from_left_sub(parent_node, parent_column, left_sib, pres_row_num);
                }
                else if (num_right >= t)
                {
                    // get from right
                    get_from_right_sib(parent_node, parent_column, right_sib, pres_row_num);
                }
                else
                {
                    merge_with_right(parent_node, parent_column, child, right_sib);
                    if (is_root(parent_node) == YES && num_nodes_in_row(parent_node) == 0)
                    {
                        readDisk(right_sib);
                    }
                    else
                    {
                        readDisk(parent_node);
                    }
                    // merge
                }
            }
        }
    }
    // now the pres_row_num has the desired element

    if (is_leaf(pres_row_num) == YES)
    {
        remove_element_from_row(pres_row_num, del_key);
    }
    else
    {
        int del_node_column;
        for (del_node_column = 0; del_node_column < 2 * t; del_node_column++)
        {
            primary_access_cost++;
            if (primary_memory[del_node_column].key == del_key)
            {
                break;
            }
        }
        primary_access_cost += 2;
        int del_node_left = primary_memory[del_node_column].left_node.row_number;
        int del_node_right = primary_memory[del_node_column + 1].left_node.row_number;
        readDisk(del_node_left);
        int num_left = num_nodes_in_row(del_node_left);
        readDisk(del_node_right);
        int num_right = num_nodes_in_row(del_node_right);
        if (num_left >= t)
        {
            // delete predecessor key in left subtree
            int del = delete_predecessor(pres_row_num, del_node_column, del_key);
            delete_btree(del);
            readDisk(pres_row_num);
            primary_access_cost++;
            primary_memory[del_node_column].key = del;
            writeDisk(pres_row_num);
            readDisk(pres_row_num);
        }
        else if (num_right >= t)
        {
            // delete successor key in right subtree
            int del = delete_successor(pres_row_num, del_node_column + 1, del_key);
            delete_btree(del);
            readDisk(pres_row_num);
            primary_access_cost++;
            primary_memory[del_node_column].key = del;
            writeDisk(pres_row_num);
            readDisk(pres_row_num);
        }
        else
        {
            // merger
            int key = merge_final(pres_row_num, del_node_left, del_node_right, del_key);
            delete_btree(key);
            if (is_root(pres_row_num) == YES)
            {
                readDisk(del_node_right);
            }
            else
            {
                readDisk(pres_row_num);
            }
        }
    }
}

// Function to read the contents from the keys file and stores in a char* file and returns it
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

// Function to access the char* returned by read_from_file and splits it with delimeter ":"
// and returns the individual keys and converts into integer
// It then decides to insert it into btree or bst using data_str_choice
void file_to_function(char *keys, int data_str_choice)
{
    char *token = strtok(keys, ":");
    int atoi_token;
    int row_num = 0;
    while (token != NULL)
    {
        atoi_token = atoi(token);
        token = strtok(NULL, ":");
        if (data_str_choice == 1)
        {
            insert_bst(atoi_token, row_num);
            row_num++;
        }
        else if (data_str_choice == 2)
        {
            insert_btree(atoi_token);
        }
    }
}

// Function to prepare the setup and insert,search and delete the said keys from the
// given keys list. It also initialises read_costs and calculates individual costs for
// insertion,search and deletion and traverses the bst
void prepare_for_bst()
{
    read_cost = write_cost = primary_access_cost = 0;
    readDisk(0);
    primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
    primary_memory[0].key = -1;
    writeDisk(0);
    file_to_function(read_from_file("keys.txt"), 1);
    printf("The cost for bst insertion is %d\n", read_cost + write_cost + primary_access_cost);
    printf("The bst contains the following keys now\n");
    traverse_bst(0);
    printf("\n");
    read_cost = write_cost = primary_access_cost = 0;
    printf("The node %d is at row %d\n", 49, search_bst(49));
    printf("The node %d is at row %d\n", 27, search_bst(27));
    printf("The node %d is at row %d\n", 22, search_bst(22));
    printf("The node %d is at row %d\n", 38, search_bst(11));
    printf("The node %d is at row %d\n", 11, search_bst(11));
    printf("The node %d is at row %d\n", 55, search_bst(55));
    printf("The node %d is at row %d\n", 7, search_bst(7));
    printf("The node %d is at row %d\n", 35, search_bst(35));
    printf("The node %d is at row %d\n", 59, search_bst(59));
    printf("The cost for bst search is %d\n", read_cost + write_cost + primary_access_cost);
    read_cost = write_cost = primary_access_cost = 0;
    delete_bst(13);
    delete_bst(19);
    delete_bst(24);
    delete_bst(37);
    delete_bst(43);
    delete_bst(53);
    delete_bst(18);
    delete_bst(38);
    delete_bst(58);
    printf("The cost for bst deletion is %d\n", read_cost + write_cost + primary_access_cost);
    printf("The bst now contains the following keys\n");
    traverse_bst(0);
    printf("\n");
}

// Function to prepare the setup and insert,search and delete the said keys from the
// given keys list. It also initialises read_costs and calculates individual costs for
// insertion,search and deletion and traverses the btree
void prepare_for_btree()
{
    read_cost = write_cost = primary_access_cost = 0;
    readDisk(btree_root_row);
    for (unsigned i = 0; i < 2 * t; i++)
    {
        primary_memory[i].key = -1;
        primary_memory[i].parent_node.row_number = -1;
        primary_memory[i].left_node.row_number = -1;
    }
    writeDisk(btree_root_row);
    file_to_function(read_from_file("keys.txt"), 2);
    printf("The cost for btree insertion is %d\n", read_cost + write_cost + primary_access_cost);
    printf("The btree contains the following keys now\n");
    traverse_btree(btree_root_row);
    printf("\n");
    read_cost = write_cost = primary_access_cost = 0;
    node_address *add = search_btree(49);
    printf("The node %d is at row %d and column %d\n", 49, add->row_number, add->column_number);
    add = search_btree(27);
    printf("The node %d is at row %d and column %d\n", 27, add->row_number, add->column_number);
    add = search_btree(22);
    printf("The node %d is at row %d and column %d\n", 22, add->row_number, add->column_number);
    add = search_btree(38);
    printf("The node %d is at row %d and column %d\n", 38, add->row_number, add->column_number);
    add = search_btree(11);
    printf("The node %d is at row %d and column %d\n", 11, add->row_number, add->column_number);
    add = search_btree(55);
    printf("The node %d is at row %d and column %d\n", 55, add->row_number, add->column_number);
    add = search_btree(7);
    printf("The node %d is at row %d and column %d\n", 7, add->row_number, add->column_number);
    add = search_btree(35);
    printf("The node %d is at row %d and column %d\n", 35, add->row_number, add->column_number);
    add = search_btree(59);
    printf("The node %d is at row %d and column %d\n", 59, add->row_number, add->column_number);
    printf("The cost for btree search is %d\n", read_cost + write_cost + primary_access_cost);
    read_cost = write_cost = primary_access_cost = 0;
    delete_btree(13);
    delete_btree(19);
    delete_btree(24);
    delete_btree(37);
    delete_btree(43);
    delete_btree(53);
    delete_btree(18);
    delete_btree(38);
    delete_btree(58);
    printf("The cost for btree deletion is %d\n", read_cost + write_cost + primary_access_cost);
    printf("The btree now contains the following nodes");
    traverse_btree(btree_root_row);
    printf("\n");
}

int main()
{
    prepare_for_bst();
    prepare_for_btree();
    return 0;
}