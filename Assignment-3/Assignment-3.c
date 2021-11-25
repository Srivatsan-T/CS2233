#define t 10
#define MAX_ROWS 200
#define YES 1
#define NO 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct addressing_nodes_
{
    int row_number;
    int column_number;
};

typedef struct addressing_nodes_ node_address;

struct cell_node_
{
    node_address left_node;
    int key;
    node_address parent_node;
};

typedef struct cell_node_ cell_node;

// Initialising primary and secondary memories
cell_node primary_memory[t * 4];
cell_node secondary_memory[MAX_ROWS][t * 2];

void readDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        primary_memory[j] = secondary_memory[i][j];
    }
}

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
}

void writeDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        secondary_memory[i][j] = primary_memory[j];
    }
}

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
}

int btree_row_num = 0;
int btree_root_row = 0;

void traverse_bst(int str_row)
{
    readDisk(str_row);
    if (primary_memory[0].left_node.row_number == -1 && primary_memory[1].left_node.row_number == -1)
    {
        printf("%d : ", primary_memory[0].key);
    }
    else if (primary_memory[0].left_node.row_number == -1)
    {
        printf("%d : ", primary_memory[0].key);
        traverse_bst(primary_memory[1].left_node.row_number);
        readDisk(str_row);
    }
    else if (primary_memory[1].left_node.row_number == -1)
    {
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

int search_bst(int find_key)
{
    readDisk(0);
    int pres_row_num = 0;
    while (primary_memory[0].key != find_key)
    {
        if (primary_memory[0].key < find_key && primary_memory[1].left_node.row_number != -1)
        {
            pres_row_num = primary_memory[1].left_node.row_number;
            readDisk(pres_row_num);
        }
        else if (primary_memory[0].key > find_key && primary_memory[0].left_node.row_number != -1)
        {
            pres_row_num = primary_memory[0].left_node.row_number;
            readDisk(pres_row_num);
        }
    }
    return pres_row_num;
}

void insert_bst(int ins_key, int row_num)
{
    readDisk(0);
    int pres_row_num = 0;
    while (primary_memory[0].left_node.row_number != -1 || primary_memory[1].left_node.row_number != -1)
    {
        if (primary_memory[0].key < ins_key)
        {
            if (primary_memory[1].left_node.row_number != -1)
            {
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
            if (primary_memory[0].left_node.row_number != -1)
            {
                pres_row_num = primary_memory[0].left_node.row_number;
                readDisk(pres_row_num);
            }
            else
            {
                break;
            }
        }
    }
    if (primary_memory[0].key == -1)
    {
        primary_memory[0].key = ins_key;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = -1;
        writeDisk(pres_row_num);
    }
    else if (primary_memory[0].key < ins_key)
    {
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
        primary_memory[0].left_node.row_number = row_num;
        writeDisk(pres_row_num);
        readDisk(row_num);
        primary_memory[0].key = ins_key;
        primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = pres_row_num;
        writeDisk(row_num);
    }
}

int find_successor(int find_key)
{
    int succ_row;
    succ_row = primary_memory[1].left_node.row_number;
    readDisk(succ_row);
    while (primary_memory[0].left_node.row_number != -1)
    {
        succ_row = primary_memory[0].left_node.row_number;
        readDisk(succ_row);
    }
    return succ_row;
}

void delete_bst(int del_key)
{
    int row = search_bst(del_key);
    readDisk(row);
    if (primary_memory[0].left_node.row_number == -1 && primary_memory[1].left_node.row_number == -1)
    {
        int parent_row = primary_memory[0].parent_node.row_number;
        readDisk(parent_row);
        if (primary_memory[0].left_node.row_number == row)
        {
            primary_memory[0].left_node.row_number = -1;
        }
        else if (primary_memory[1].left_node.row_number == row)
        {
            primary_memory[1].left_node.row_number = -1;
        }
        writeDisk(parent_row);
        readDisk(row);
        primary_memory[0].key = -1;
        primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = -1;
        writeDisk(row);
    }
    else if (primary_memory[0].left_node.row_number == -1 || primary_memory[1].left_node.row_number == -1)
    {
        int child_row;
        int parent_row = primary_memory[0].parent_node.row_number;
        if (primary_memory[0].left_node.row_number == -1)
        {
            child_row = primary_memory[1].left_node.row_number;
        }
        else if (primary_memory[1].left_node.row_number == -1)
        {
            child_row = primary_memory[0].left_node.row_number;
        }
        readDisk(child_row);
        int child_of_child;
        if (primary_memory[0].left_node.row_number != -1)
        {
            child_of_child = primary_memory[0].left_node.row_number;
            readDisk(child_of_child);
            primary_memory[0].parent_node.row_number = row;
            primary_memory[1].parent_node.row_number = row;
            writeDisk(child_of_child);
        }
        if (primary_memory[1].left_node.row_number != -1)
        {
            child_of_child = primary_memory[1].left_node.row_number;
            child_of_child = primary_memory[0].left_node.row_number;
            readDisk(child_of_child);
            primary_memory[0].parent_node.row_number = row;
            primary_memory[1].parent_node.row_number = row;
            writeDisk(child_of_child);
        }
        readDisk(child_row);
        primary_memory[0].parent_node.row_number = parent_row;
        primary_memory[1].parent_node.row_number = parent_row;
        writeDisk(row);
        primary_memory[0].key = primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
        primary_memory[0].parent_node.row_number = primary_memory[1].parent_node.row_number = -1;
        writeDisk(child_row);
    }
    else
    {
        int succ_row = find_successor(del_key);
        readDisk(succ_row);
        int succ_key = primary_memory[0].key;
        delete_bst(succ_key);
        readDisk(row);
        primary_memory[0].key = succ_key;
        writeDisk(row);
    }
}

int is_leaf(int row_num)
{
    readDisk(row_num);
    if (primary_memory[0].left_node.row_number == -1)
    {
        return YES;
    }
    return NO;
}

int is_root(int row_num)
{
    readDisk(row_num);
    if (primary_memory[0].parent_node.row_number == -1)
    {
        return YES;
    }
    return NO;
}

int num_nodes_in_row(int row_num)
{
    readDisk(row_num);
    int num_nodes = 0;
    int i = 0;
    while (primary_memory[i].key != -1)
    {
        num_nodes++;
        i++;
    }
    return num_nodes;
}

void traverse_btree(int str_row)
{
    readDisk(str_row);
    if (is_leaf(str_row) == YES)
    {
        for (unsigned i = 0; primary_memory[i].key != -1; i++)
        {
            printf("%d :", primary_memory[i].key);
        }
    }
    else
    {
        for (unsigned i = 0; i <= num_nodes_in_row(str_row); i++)
        {
            traverse_btree(primary_memory[i].left_node.row_number);
            readDisk(str_row);
            if (primary_memory[i].key != -1)
            {
                printf("%d :", primary_memory[i].key);
            }
        }
    }
}



int sort_row(int row_num, int ins_key)
{

    int i = 0;
    while (primary_memory[i].key < ins_key && primary_memory[i].key != -1)
    {
        i++;
    }
    int j, k;
    for (j = 0; j < 2 * t; j++)
    {
        if (primary_memory[j].key == -1)
            break;
    }
    for (k = j; k >= i; k--)
    {
        primary_memory[k + 1] = primary_memory[k];
    }

    primary_memory[i].key = ins_key;
    writeDisk(row_num);
    return i;
}


int key_in_row(int row_num, int find_key)
{
    readDisk(row_num);
    for (unsigned i = 0; i < 2 * t - 1; i++)
    {
        if (primary_memory[i].key == find_key)
        {
            return 1;
        }
    }
    return 0;
}

node_address *search_btree(int find_key)
{
    int pres_row_num = btree_root_row;
    readDisk(pres_row_num);
    while (key_in_row(pres_row_num, find_key) == 0)
    {
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            if (primary_memory[i].key < find_key && primary_memory[i + 1].key > find_key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                // break;
            }
            else if (primary_memory[i].key < find_key && primary_memory[i + 1].key == -1)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                // break;
            }
            else if (primary_memory[i].key > find_key && i == 0)
            {
                pres_row_num = primary_memory[i].left_node.row_number;
                readDisk(pres_row_num);
                // break;
            }
        }
    }
    node_address *add = malloc(sizeof(node_address));
    add->row_number = pres_row_num;
    for (unsigned i = 0; i < 2 * t - 1; i++)
    {
        if (primary_memory[i].key == find_key)
        {
            add->column_number = i;
        }
    }
    return add;
}

void spilt_node(int row_num, int new_row_num)
{
    readDisk(row_num);
    cell_node temp = primary_memory[9];
    node_address temp_parent = primary_memory[9].parent_node;
    writeDisk(new_row_num);
    for (unsigned i = t - 1; i < 2 * t; i++)
    {
        primary_memory[i].key = -1;
        primary_memory[i].left_node.row_number = -1;
    }
    primary_memory[t - 1].left_node = temp.left_node;
    writeDisk(row_num);
    readDisk(new_row_num);
    for (int i = 0; i < t - 1; i++)
    {
        primary_memory[i] = primary_memory[i + 10];
        primary_memory[i].parent_node = temp_parent;
        primary_memory[i + 10].key = -1;
        primary_memory[i + 10].left_node.row_number = -1;
    }
    primary_memory[t - 1].key = primary_memory[t - 1].left_node.row_number = -1;
    writeDisk(new_row_num);

    if (temp_parent.row_number != -1)
    {
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
            primary_memory[i].parent_node.row_number = btree_root_row;
        }
        writeDisk(row_num);
        readDisk(new_row_num);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_memory[i].parent_node.row_number = btree_root_row;
        }
        writeDisk(new_row_num);

        readDisk(btree_root_row);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_memory[i].parent_node.row_number = -1;
        }
        primary_memory[0].key = temp.key;
        primary_memory[0].left_node.row_number = row_num;
        primary_memory[1].left_node.row_number = new_row_num;
        primary_memory[1].key = -1;
        writeDisk(btree_root_row);
    }
}

void insert_btree(int ins_key)
{
    int pres_row_num = btree_root_row;
    readDisk(pres_row_num);
    while (is_leaf(pres_row_num) != YES)
    {
        if (num_nodes_in_row(pres_row_num) < 2 * t - 1)
        {
            for (unsigned i = 0; i < 2 * t - 1; i++)
            {
                if (primary_memory[i].key < ins_key && primary_memory[i + 1].key > ins_key)
                {
                    pres_row_num = primary_memory[i + 1].left_node.row_number;
                    readDisk(pres_row_num);
                    break;
                }
                else if (primary_memory[i].key < ins_key && primary_memory[i + 1].key == -1)
                {
                    pres_row_num = primary_memory[i + 1].left_node.row_number;
                    readDisk(pres_row_num);
                    break;
                }
                else if (primary_memory[i].key > ins_key && i == 0)
                {
                    pres_row_num = primary_memory[i].left_node.row_number;
                    readDisk(pres_row_num);
                    break;
                }
            }
        }
        else
        {
            node_address temp_parent = primary_memory[0].parent_node;
            spilt_node(pres_row_num, ++btree_row_num);
            pres_row_num = temp_parent.row_number;
            readDisk(pres_row_num);
        }
    }

    if (num_nodes_in_row(pres_row_num) < 2 * t - 1)
    {
        sort_row(pres_row_num, ins_key);
    }
    else
    {
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
            if (primary_memory[i].key < ins_key && primary_memory[i + 1].key > ins_key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
            else if (primary_memory[i].key < ins_key && primary_memory[i + 1].key == -1)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
            else if (primary_memory[i].key > ins_key && i == 0)
            {
                pres_row_num = primary_memory[i].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
        }
        sort_row(pres_row_num, ins_key);
    }
}

cell_node remove_element_from_row(int row_num, int ele_key)
{
    readDisk(row_num);
    int i;
    for (i = 0; i < 2 * t; i++)
    {
        if (primary_memory[i].key == ele_key)
        {
            break;
        }
    }
    cell_node ret = primary_memory[i];
    for (unsigned j = i; j < 2 * t-1; j++)
    {
        primary_memory[j] = primary_memory[j + 1];
    }
    writeDisk(row_num);
    return ret;
}

void get_from_right_sib(int parent_row, int parent_col, int right_sib, int child_row)
{
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

void get_from_left_sub(int parent_row, int parent_col, int left_sib, int child_row)
{
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

int merge_with_right(int parent_row, int parent_col, int child_row, int right_sib)
{
    readDisk(parent_row);
    int parent_key = primary_memory[parent_col].key;
    if (is_leaf(child_row) == NO)
    {
        for (unsigned i = 0; i < 2 * t; i++)
        {
            int new_row_num = primary_memory[i].left_node.row_number;
            readDisk(new_row_num);
            for (unsigned j = 0; primary_memory[j].key != -1; j++)
            {
                primary_memory[i].parent_node.row_number = right_sib;
            }
            writeDisk(new_row_num);
        }
    }
    read2(child_row, right_sib);
    primary_memory[t - 1].key = parent_key;
    for (unsigned i = 2 * t; i <= 3 * t - 1; i++)
    {
        primary_memory[i - t] = primary_memory[i];
    }
    writeDisk(right_sib);
    readDisk(parent_row);

    if (is_root(parent_row) == YES && num_nodes_in_row(parent_row) == 1)
    {
        btree_root_row = right_sib;
        readDisk(right_sib);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_memory[i].parent_node.row_number = -1;
        }
        writeDisk(right_sib);
    }
    remove_element_from_row(parent_row, parent_key);
    return parent_key;
}

int merge_with_left(int parent_row, int parent_col, int child_row, int left_sib)
{
    readDisk(parent_row);
    cell_node parent = primary_memory[parent_col - 1];
    if (is_leaf(child_row) == NO)
    {
        for (unsigned i = 0; i < 2 * t; i++)
        {
            int new_row_num = primary_memory[i].left_node.row_number;
            readDisk(new_row_num);
            for (unsigned j = 0; primary_memory[j].key != -1; j++)
            {
                primary_memory[i].parent_node.row_number = left_sib;
            }
            writeDisk(new_row_num);
        }
    }
    read2(left_sib, child_row);
    primary_memory[t - 1].key = parent.key;
    for (unsigned i = 2 * t; i <= 3 * t - 1; i++)
    {
        primary_memory[i - t] = primary_memory[i];
    }
    writeDisk(left_sib);
    readDisk(parent_row);
    if (is_root(parent_row) == YES && num_nodes_in_row(parent_row) == 1)
    {
        btree_root_row = left_sib;
        readDisk(left_sib);
        remove_element_from_row(left_sib, parent.key);
        primary_memory[parent_col].left_node.row_number = parent.left_node.row_number;
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_memory[i].parent_node.row_number = -1;
        }
        writeDisk(left_sib);
    }
    else
    {
        readDisk(parent_row);
        remove_element_from_row(parent_row, parent.key);
        primary_memory[parent_col - 1].left_node.row_number = parent.left_node.row_number;
        writeDisk(parent_row);
    }
    return parent.key;
}

int delete_predecessor(int row_num, int col, int key)
{
    readDisk(row_num);
    int pres_row_num = primary_memory[col].left_node.row_number;
    readDisk(pres_row_num);
    while (is_leaf(pres_row_num) != YES)
    {
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            if (primary_memory[i].key < key && primary_memory[i + 1].key > key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
            }
            else if (primary_memory[i].key < key && primary_memory[i + 1].key == -1)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
            }
            else if (primary_memory[i].key > key && i == 0)
            {
                pres_row_num = primary_memory[i].left_node.row_number;
                readDisk(pres_row_num);
            }
        }
    }
    int del_key = primary_memory[num_nodes_in_row(pres_row_num) - 1].key;
    return del_key;
}

int delete_successor(int row_num, int col, int key)
{
    readDisk(row_num);
    int pres_row_num = primary_memory[col].left_node.row_number;
    readDisk(pres_row_num);
    while (is_leaf(pres_row_num) != YES)
    {
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            if (primary_memory[i].key < key && primary_memory[i + 1].key > key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
            }
            else if (primary_memory[i].key < key && primary_memory[i + 1].key == -1)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
            }
            else if (primary_memory[i].key > key && i == 0)
            {
                pres_row_num = primary_memory[i].left_node.row_number;
                readDisk(pres_row_num);
            }
        }
    }
    int del_key = primary_memory[0].key;
    return del_key;
}

int merge_final(int row_num, int left_sib, int right_sib, int parent_key)
{
    read2(left_sib, right_sib);
    primary_memory[t - 1].key = parent_key;
    for (unsigned i = 2 * t; i <= 3 * t - 1; i++)
    {
        primary_memory[i - t] = primary_memory[i];
    }
    writeDisk(right_sib);
    readDisk(row_num);
    if (is_root(row_num) == YES && num_nodes_in_row(row_num) == 1)
    {
        btree_root_row = right_sib;
        readDisk(right_sib);
        for (unsigned i = 0; i < 2 * t; i++)
        {
            primary_memory[i].parent_node.row_number = -1;
        }
        writeDisk(right_sib);
    }
    remove_element_from_row(row_num, parent_key);
    return parent_key;
}

void delete_btree(int del_key)
{
    int pres_row_num = btree_root_row;
    while (key_in_row(pres_row_num, del_key) != 1)
    {
        for (unsigned i = 0; i < 2 * t - 1; i++)
        {
            if (primary_memory[i].key < del_key && primary_memory[i + 1].key > del_key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
            else if (primary_memory[i].key < del_key && primary_memory[i + 1].key == -1)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
            else if (primary_memory[i].key > del_key && i == 0)
            {
                pres_row_num = primary_memory[i].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
        }
        if (num_nodes_in_row(pres_row_num) < t)
        {
            // update pres_row_num to keep loop alive
            int parent_node = primary_memory[0].parent_node.row_number;
            int parent_column;
            readDisk(parent_node);
            for (parent_column = 0; parent_column < 2 * t; parent_column++)
            {
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
                right_sib = primary_memory[parent_column + 1].left_node.row_number;
                if (num_nodes_in_row(right_sib) >= t)
                {
                    // get from right_sib
                    get_from_right_sib(parent_node, parent_column, right_sib, pres_row_num);
                }
                else
                {
                    // merge
                    readDisk(parent_node);
                    merge_with_right(parent_node, parent_column, primary_memory[parent_column].left_node.row_number, right_sib);
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
                left_sib = primary_memory[parent_column - 1].left_node.row_number;
                if (num_nodes_in_row(left_sib) >= t)
                {
                    // get from left
                    get_from_left_sub(parent_node, parent_column - 1, left_sib, pres_row_num);
                }
                else
                {
                    // merge
                    readDisk(parent_node);
                    merge_with_left(parent_node, parent_column, primary_memory[parent_column].left_node.row_number, left_sib);
                    // delete_btree(res);
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
                left_sib = primary_memory[parent_column - 1].left_node.row_number;
                right_sib = primary_memory[parent_column + 1].left_node.row_number;
                if (num_nodes_in_row(left_sib) >= t)
                {
                    // get from left
                    get_from_left_sub(parent_node, parent_column, left_sib, pres_row_num);
                }
                else if (num_nodes_in_row(right_sib) >= t)
                {
                    // get from right
                    get_from_right_sib(parent_node, parent_column, right_sib, pres_row_num);
                }
                else
                {
                    readDisk(parent_node);
                    merge_with_right(parent_node, parent_column, primary_memory[parent_column].left_node.row_number, right_sib);
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
            if (primary_memory[del_node_column].key == del_key)
            {
                break;
            }
        }
        int del_node_left = primary_memory[del_node_column].left_node.row_number;
        int del_node_right = primary_memory[del_node_column + 1].left_node.row_number;
        if (num_nodes_in_row(del_node_left) >= t)
        {
            // delete predecessor key in left subtree
            readDisk(pres_row_num);
            int del = delete_predecessor(pres_row_num, del_node_column, del_key);
            delete_btree(del);
            readDisk(pres_row_num);
            primary_memory[del_node_column].key = del;
            writeDisk(pres_row_num);
            readDisk(pres_row_num);
        }
        else if (num_nodes_in_row(del_node_right) >= t)
        {
            // delete successor key in right subtree
            readDisk(pres_row_num);
            int del = delete_successor(pres_row_num, del_node_column + 1, del_key);
            delete_btree(del);
            readDisk(pres_row_num);
            primary_memory[del_node_column].key = del;
            writeDisk(pres_row_num);
            readDisk(pres_row_num);
        }
        else
        {
            // merger
            readDisk(pres_row_num);
            int key = merge_final(pres_row_num, del_node_left, del_node_right, del_key);
            delete_btree(key);
            readDisk(pres_row_num);
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
            /*
            for (unsigned i = 0; i < 10; i++)
            {
                for (unsigned j = 0; j < 2 * t; j++)
                {
                    printf("%d :", secondary_memory[i][j].key);
                }
                printf("\n");
            }
            printf("Btree root is %d and row_num is %d", btree_root_row, btree_row_num);
            printf("END\n");
            traverse_btree(btree_root_row);
            printf("\n");
            */
        }
    }
}

void prepare_for_bst()
{
    readDisk(0);
    primary_memory[0].left_node.row_number = primary_memory[1].left_node.row_number = -1;
    primary_memory[0].key = -1;
    writeDisk(0);
    file_to_function(read_from_file("keys.txt"), 1);
    traverse_bst(0);
    printf("\n");
    delete_bst(13);
    delete_bst(19);
    delete_bst(24);
    delete_bst(37);
    delete_bst(43);
    delete_bst(53);
    delete_bst(18);
    delete_bst(38);
    delete_bst(58);
    traverse_bst(0);
    printf("BST END\n");
}

void prepare_for_btree()
{
    readDisk(btree_root_row);
    for (unsigned i = 0; i < 2 * t; i++)
    {
        primary_memory[i].key = -1;
        primary_memory[i].parent_node.row_number = -1;
        primary_memory[i].left_node.row_number = -1;
    }
    writeDisk(btree_root_row);
    file_to_function(read_from_file("keys.txt"), 2);
    printf("\n");
    /*
    delete_btree(13);
    delete_btree(19);
    delete_btree(24);
    delete_btree(37);
    delete_btree(43);
    delete_btree(53);
    delete_btree(18);
    delete_btree(38);
    delete_btree(58);
    */
    delete_btree(17);
    delete_btree(36);
    delete_btree(60);
    delete_btree(59);
    //delete_btree(58);
    delete_btree(1);
    delete_btree(2);
    delete_btree(3);
    delete_btree(4);
    delete_btree(5);
    delete_btree(6);
    delete_btree(7);
    delete_btree(19);
    delete_btree(20);
    delete_btree(21);
    delete_btree(22);
    delete_btree(23);
    delete_btree(24);
    //delete_btree(25);
    delete_btree(37);
    delete_btree(38);
    // delete_btree(26);
    delete_btree(8);
    for (unsigned i = 0; i < 10; i++)
    {
        for (unsigned j = 0; j < 2 * t; j++)
        {
            printf("%d :", secondary_memory[i][j].key);
        }
        printf("\n");
    }
    delete_btree(35);
    for (unsigned i = 0; i < 10; i++)
    {
        for (unsigned j = 0; j < 2 * t; j++)
        {
            printf("%d :", secondary_memory[i][j].key);
        }
        printf("\n");
    }
    traverse_btree(btree_root_row);
}

int main()
{
    // prepare_for_bst();
    prepare_for_btree();
    return 0;
}