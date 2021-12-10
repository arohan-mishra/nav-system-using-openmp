#include <limits.h>
#include <stdio.h>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

#define V 51 //define dimension of matix
#define INFINITY 100000

//to find the minimum path from the current node
int minPathLength(int length[], bool setPath[])
{
	int min = INFINITY, min_index;

	for (int v = 0; v < V; v++)
		if (setPath[v] == false && length[v] <= min)
			min = length[v], min_index = v; //min length set to length of the node frm infinity

	return min_index; //returns the index with the min path
}

//prints the solution
void print(int length[], int n)
{
	printf("VERTEX \t\t DISTANCE FROM SOURCE\n");
	for (int i = 0; i < V; i++)
    {
        if(length[i]==INFINITY)
            printf("%d \t\t INFINITY\n", i);
        else 
            printf("%d \t\t %d\n", i, length[i]);
    }
}

//the core code where the dijkstra algorithm is implemented
//takes the graph and a value zero as input 
void dijkstra_code(int map[V][V], int src)
{
	int length[V]; //length of the node in the graph

	bool setPath[V]; //if the node is in the shortes path it will be true

	for (int i = 0; i < V; i++)
		length[i] = INFINITY, setPath[i] = false; //initiale node length is infinite and setPath value is false
	length[src] = 0;

	for (int count = 0; count < V - 1; count++) {

		int u = minPathLength(length, setPath); //sets the minimum path length

		setPath[u] = true;

		for (int v = 0; v < V; v++)
			if (!setPath[v] && map[u][v] && length[u] != INFINITY
				&& length[u] + map[u][v] < length[v])
				length[v] = length[u] + map[u][v];
	}

	print(length, V);
}

int main()
{
	FILE* city = fopen("city4.txt", "r"); //the iput graph is taken from a txt file

    char c;
    int n, no = 0, i = 0, j = 0;
    int map[51][51];

    while(!feof(city)) 
    {
        c = getc(city);

        if(isdigit(c))
        {
            no = no * 10 + int(c) - 48;
        }

        else if(c == ' ')
        {
            map[i][j] = no;
            no = 0;
            j++;
        }

        else if(int(c) == 10 || int(c) == -1)
        {
            map[i][j] = no;
            n = j;
            no = 0;
            j = 0;
            i++;
        }
    }

    fclose(city);

    auto start = high_resolution_clock::now(); //the start method to calculate runtime 
	
	dijkstra_code(map, 0); //implement the algorithm

    auto end = high_resolution_clock::now(); //end the run time

    auto time_duration = duration_cast<microseconds>(end - start); 
  
    cout << "Running time: "
         << time_duration.count() << " ms\n";

	return 0;
}
