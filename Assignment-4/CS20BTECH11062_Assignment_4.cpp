#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <math.h>
#include <algorithm>
#include <queue>
#include <functional>
#include <fstream>

#define INFECTED 0
#define RECOVERED 1

//Run this C++ and follow it up by running the graph.py file attached with this folder
//This C++ file creates and populates the CSV files required for simulating the discrete event
//The python script outputs the graphs of the simulations

class node
{
public:
    int node_id;
    int time_stamp;
    int event_type;
    int distance;
};

// Creating a comparer class to overload the constructor of node class
struct comparator
{
    bool operator()(const node &n1, const node &n2)
    {
        return n1.time_stamp > n2.time_stamp;
    }
};

//Class to store the population and their adjacents
class graph
{
public:
    int node_id;
    std::vector<int> adjacency_list;
};

//Vectors to hold population, Suspecteds ,infecteds and recovered
std::vector<graph> Population;

std::vector<int> S;
std::vector<int> I;
std::vector<int> R;

//Priority queue to hold the infected and recovery events
std::priority_queue<node, std::vector<node>, comparator> min_queue;

// Function to print the adjacency list of nodes
void print_adjacency_list(std::vector<graph> pop_graph)
{
    for (unsigned i = 1; i <= 100; i++)
    {
        std::cout << "Adjacency list of node " << i << "\n";
        for (unsigned j = 0; j < pop_graph[i].adjacency_list.size(); j++)
        {
            std::cout << pop_graph[i].adjacency_list[j] << ":";
        }
        std::cout << "\n";
    }
}

// Function to get the total population graph by tossing a coin to determine an edge between 2 persons
void get_population_graph(std::vector<graph> &pop_graph)
{
    graph temp;
    temp.node_id = -1;
    pop_graph.push_back(temp);
    for (unsigned i = 1; i <= 100; i++)
    {
        temp.node_id = i;
        pop_graph.push_back(temp);
        // Initialising All persons in susceptible list
        S.push_back(i);
    }
    int toss, count;
    for (unsigned i = 1; i <= 100; i++)
    {
        for (unsigned j = i + 1; j <= 100; j++)
        {
            toss = abs(rand()) % 2;

            // We consider toss = 0 to be heads and toss = 1 to be tails
            if (toss == 0)
            {
                // We check if the link already exists if not we create it
                if (std::find(pop_graph[i].adjacency_list.begin() + 1, pop_graph[i].adjacency_list.end(), j) == pop_graph[i].adjacency_list.end() && std::find(pop_graph[j].adjacency_list.begin() + 1, pop_graph[j].adjacency_list.end(), i) == pop_graph[j].adjacency_list.end())
                {
                    pop_graph[j].adjacency_list.push_back(i);
                    pop_graph[i].adjacency_list.push_back(j);
                }
            }
        }
    }
}

// Function to select a person at random and add him to infected set
int select_arbitrary_person()
{
    // Generating a random number from 0 to 99
    // and adding 1 to it make it from 1 to 100
    int person_id = rand() % 100;
    person_id++;

    //Creating infection and recovery event for the first infected and pushing them into the priority queue
    node new_node_inf, new_node_rec;
    new_node_inf.node_id = person_id;
    new_node_inf.event_type = INFECTED;
    new_node_inf.time_stamp = 0;
    new_node_inf.distance = 0;

    new_node_rec.node_id = person_id;
    new_node_rec.event_type = RECOVERED;
    new_node_rec.time_stamp = 1 + rand() % 6;
    new_node_rec.distance = 0;

    min_queue.push(new_node_inf);
    min_queue.push(new_node_rec);
    return person_id;
}


//Function to loop through the neighbours of an infected person and checking if they are infected or not
//And then infect a few of them based on tosses and generate corresponding infection and recovery event for them
void search_sus_neighbours(node deleted_node)
{
    int toss;
    node ins_heap_inf;
    node ins_heap_rec;
    std::vector<int> sus_list = Population[deleted_node.node_id].adjacency_list;
    for (int i = 0; i < sus_list.size(); i++)
    {
        if ((std::find(I.begin(), I.end(), sus_list[i]) == I.end()) && (std::find(R.begin(), R.end(), sus_list[i]) == R.end()) /* not in infected or recovered */)
        {
            int inf_time = 0;
            for (int j = 1; j <= 5; j++)
            {
                toss = rand() % 2;
                if (toss == 0)
                {
                    inf_time = j;
                    break;
                }
            }
            if (inf_time == 0)
            {
                continue;
            }
            else
            {
                ins_heap_inf.node_id = sus_list[i];
                ins_heap_inf.event_type = INFECTED;
                ins_heap_inf.time_stamp = deleted_node.time_stamp + inf_time;
                ins_heap_inf.distance = deleted_node.distance + 1;
                min_queue.push(ins_heap_inf);

                int random_number = deleted_node.time_stamp + inf_time + (rand() % 6);
                ins_heap_rec.node_id = sus_list[i];
                ins_heap_rec.event_type = RECOVERED;
                ins_heap_rec.time_stamp = random_number;
                ins_heap_rec.distance = deleted_node.distance + 1;
                min_queue.push(ins_heap_rec);
            }
        }
        else
        {
            continue;
        }
    }
}

//Function to simulate the discrete event by looping around and handling infection and recovery events till the queue becomes empty
void simulation()
{
    //CSV files to hold the timestamps and S,I,R values and another to hold the distance and corresponding infection times
    std::ofstream csv_file;
    std::ofstream dist_csv;
    dist_csv.open("inf_dist.csv");
    csv_file.open("DES.csv");
    csv_file << "Time Stamp"
             << ","
             << "Susceptible"
             << ","
             << "Infected"
             << ","
             << "Recovered"
             << ","
             << "\n";

    dist_csv << "Infected time"
             << ","
             << "Distance from starting"
             << ","
             << "\n";

    int curr_time_stamp = 0;
    select_arbitrary_person();
    node deleted_node;
    while (!min_queue.empty())
    {
        deleted_node = min_queue.top();
        min_queue.pop();
        if (deleted_node.event_type == RECOVERED)
        {
            if ((std::find(R.begin(), R.end(), deleted_node.node_id) == R.end()) && (std::find(I.begin(), I.end(), deleted_node.node_id) != I.end()))
            {
                //Transferring from I to R
                R.push_back(deleted_node.node_id);
                I.erase(std::remove(I.begin(), I.end(), deleted_node.node_id), I.end());
                if (deleted_node.time_stamp != curr_time_stamp)
                {
                    // update the csv with time stamp and S I R values
                    csv_file << curr_time_stamp << "," << S.size() << "," << I.size() << "," << R.size() << ","
                             << "\n";
                    curr_time_stamp = deleted_node.time_stamp;
                }
            }
        }
        else if (deleted_node.event_type == INFECTED)
        {
            if ((std::find(I.begin(), I.end(), deleted_node.node_id) == I.end()) && (std::find(S.begin(), S.end(), deleted_node.node_id) != S.end()))
            {
                //Transferring from S to I
                I.push_back(deleted_node.node_id);
                dist_csv << curr_time_stamp << "," << deleted_node.distance << ","
                         << "\n";
                S.erase(std::remove(S.begin(), S.end(), deleted_node.node_id), S.end());
                search_sus_neighbours(deleted_node);
                if (deleted_node.time_stamp != curr_time_stamp)
                {
                    // update the csv with time stamp and S I R values
                    csv_file << curr_time_stamp << "," << S.size() << "," << I.size() << "," << R.size() << ","
                             << "\n";
                    curr_time_stamp = deleted_node.time_stamp;
                }
            }
        }
    }
}

int main()
{
    get_population_graph(Population);
    simulation();
    printf(" S: %d , I : %d , R : %d ", S.size(), I.size(), R.size());
    return 0;
}