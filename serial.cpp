#include <limits.h>
#include <stdio.h>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

#define V 3
#define INFINITY 100000

int minDistance(int dist[], bool sptSet[])
{
	int min = INFINITY, min_index;

	for (int v = 0; v < V; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

void printSolution(int dist[], int n)
{
	printf("VERTEX \t\t DISTANCE FROM SOURCE\n");
	for (int i = 0; i < V; i++)
    {
        if(dist[i]==INFINITY)
            printf("%d \t\t INFINITY\n", i);
        else 
            printf("%d \t\t %d\n", i, dist[i]);
    }
}

void dijkstra(int graph[V][V], int src)
{
	int dist[V]; 

	bool sptSet[V]; 

	for (int i = 0; i < V; i++)
		dist[i] = INFINITY, sptSet[i] = false;

	dist[src] = 0;

	for (int count = 0; count < V - 1; count++) {

		int u = minDistance(dist, sptSet);

		sptSet[u] = true;

		for (int v = 0; v < V; v++)
			if (!sptSet[v] && graph[u][v] && dist[u] != INFINITY
				&& dist[u] + graph[u][v] < dist[v])
				dist[v] = dist[u] + graph[u][v];
	}

	printSolution(dist, V);
}

int main()
{
	FILE* city = fopen("city2.txt", "r");

    char c;
    int n, num = 0, i = 0, j = 0;
    int graph[3][3];

    while(!feof(city)) 
    {
        c = getc(city);

        if(isdigit(c))
        {
            num = num * 10 + int(c) - 48;
        }

        else if(c == ' ')
        {
            graph[i][j] = num;
            num = 0;
            j++;
        }

        else if(int(c) == 10 || int(c) == -1)
        {
            graph[i][j] = num;
            n = j;
            num = 0;
            j = 0;
            i++;
        }
    }

    fclose(city);

    auto start = high_resolution_clock::now();
	
	dijkstra(graph, 0);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
  
    cout << "Running time: "
         << duration.count() << " ms\n";

	return 0;
}
