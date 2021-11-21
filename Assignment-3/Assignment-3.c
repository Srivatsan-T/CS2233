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
cell_node primary_memory[t * 2];
cell_node secondary_memory[MAX_ROWS][t * 2];

void readDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        primary_memory[j] = secondary_memory[i][j];
    }
}

void writeDisk(int i)
{
    for (unsigned j = 0; j < 2 * t; j++)
    {
        secondary_memory[i][j] = primary_memory[j];
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
        if (primary_memory[0].left_node.row_number == -1)
        {
            child_row = primary_memory[1].left_node.row_number;
        }
        else if (primary_memory[1].left_node.row_number == -1)
        {
            child_row = primary_memory[0].left_node.row_number;
        }
        readDisk(child_row);
        int child_key = primary_memory[0].key;
        primary_memory[0].key = -1;
        writeDisk(child_row);
        readDisk(row);
        primary_memory[0].key = child_key;
        writeDisk(row);
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
        primary_memory[k + 1].key = primary_memory[k].key;
    }

    primary_memory[i].key = ins_key;
    writeDisk(row_num);
    /*
    printf("%d ", i);
    for (unsigned i = 0; i < 2 * t; i++)
    {
        printf("%d :", secondary_memory[0][i].key);
    }
    printf("\n");
    */
    return i;
}

int leaf_search_btree(int find_key)
{
    int pres_row_num = btree_root_row;
    readDisk(pres_row_num);
    while (is_leaf(pres_row_num) != YES)
    {
        for (unsigned i = 0; i < 2 * t - 2; i++)
        {
            if (primary_memory[i].key < find_key && primary_memory[i + 1].key > find_key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
            }
        }
    }
    return pres_row_num;
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
            else if (primary_memory[i].key < find_key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                // break;
            }
            else if (primary_memory[i].key > find_key)
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
                else if (primary_memory[i].key < ins_key)
                {
                    pres_row_num = primary_memory[i + 1].left_node.row_number;
                    readDisk(pres_row_num);
                    break;
                }
                else if (primary_memory[i].key > ins_key)
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
            else if (primary_memory[i].key < ins_key)
            {
                pres_row_num = primary_memory[i + 1].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
            else if (primary_memory[i].key > ins_key)
            {
                pres_row_num = primary_memory[i].left_node.row_number;
                readDisk(pres_row_num);
                break;
            }
        }
        sort_row(pres_row_num, ins_key);
    }
}

void key_from_sibling(int row_num, int sibling_code)
{
    readDisk(row_num);
    readDisk(primary_memory[0].parent_node.row_number);
    unsigned i,rem_parent;
    for (i = 0; i < 2 * t; i++)
    {
        if (primary_memory[i].left_node.row_number == row_num)
        {
            break;
        }
        else
        {
            i++;
        }
    }
    if (sibling_code == 1)
    {
        rem_parent = primary_memory[i].parent_node.row_number;
        int temp_left = primary_memory[i+1].left_node.row_number;
        readDisk(temp_left);
        cell_node temp = primary_memory[0];
        for(unsigned i = 0;i<2*t-1;i++)
        {
            primary_memory[i] = primary_memory[i+1];
        }
        writeDisk(temp_left);
        readDisk(rem_parent);
        int temp_key = primary_memory[i].key;
        primary_memory[i].key = temp.key;
        writeDisk(rem_parent);
        readDisk(row_num);
        int num_nodes = num_nodes_in_row(row_num);
        primary_memory[num_nodes -1].key = temp_key;
        primary_memory[num_nodes].key = -1;
        primary_memory[num_nodes].left_node.row_number = temp.left_node.row_number;
        writeDisk(row_num);
    }
    else if(sibling_code == 0)
    {
        rem_parent = primary_memory[i-1].parent_node.row_number;
        int temp_right = primary_memory[i-1].left_node.row_number;
        readDisk(temp_right);
        cell_node temp = primary_memory[num_nodes_in_row(temp_right)-2];
        primary_memory[num_nodes_in_row(temp_right)-2].key = -1;
        int far_right =  primary_memory[num_nodes_in_row(temp_right)-2].left_node.row_number;
        primary_memory[num_nodes_in_row(temp_right)-2].left_node.row_number = -1;
        writeDisk(temp_right);
        readDisk(rem_parent);
        int temp_key = primary_memory[i-1].key;
        primary_memory[i-1].key = temp.key;
        writeDisk(rem_parent);
        readDisk(row_num);
        int num_nodes = num_nodes_in_row(row_num);
        for(unsigned i = num_nodes;i>=0;i--)
        {
            primary_memory[i+1] = primary_memory[i];
        }
        primary_memory[0].key = temp_key;
        primary_memory[0].left_node.row_number = far_right;
        writeDisk(row_num);
    }
}

void merge_nodes()
{
    
}

void delete_btree(int del_key)
{
    int pres_row_num = btree_root_row;
    readDisk(pres_row_num);
    while (key_in_row(pres_row_num, del_key) != 1)
    {
        if (num_nodes_in_row(pres_row_num) >= t)
        {
            for (unsigned i = 0; i < 2 * t - 1; i++)
            {
                if (primary_memory[i].key < del_key && primary_memory[i + 1].key > del_key)
                {
                    pres_row_num = primary_memory[i + 1].left_node.row_number;
                    readDisk(pres_row_num);
                    break;
                }
                else if (primary_memory[i].key < del_key)
                {
                    pres_row_num = primary_memory[i + 1].left_node.row_number;
                    readDisk(pres_row_num);
                    break;
                }
                else if (primary_memory[i].key > del_key)
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
            
            pres_row_num = temp_parent.row_number;
            readDisk(pres_row_num);
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
    for (unsigned i = 0; i < 10; i++)
    {
        for (unsigned j = 0; j < 2 * t; j++)
        {
            printf("%d :", secondary_memory[i][j].key);
        }
        printf("\n");
    }
}

int main()
{
    prepare_for_bst();
    //prepare_for_btree();
    //printf("Btree root is %d and row_num is %d", btree_root_row, btree_row_num);
    return 0;
}