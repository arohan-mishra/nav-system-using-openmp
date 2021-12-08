#include <iostream>
#include <omp.h>
#include <algorithm>
#include <ctype.h>
#include <string>
#include <fstream>
 
using namespace std;
 
#define INFINITY 100000
int V,E;
int arr[9] = { 0 };
int graph[9][9];
 
//Structure for vertex
typedef struct
{
    int label;
    bool visited;  
 
} Vertex;
 
//Structure for directed edge from u to v
typedef struct
{
    int u;
    int v;
 
} Edge;
 
//Printing Shortest Path Length
void printShortestPathLength(int *path_length)
{
    printf("\nVERTEX \tSHORTEST PATH LENGTH \n");
    int i;
    for(i = 0; i < V; i++)
    {
        printf("%d \t",i);
        if (path_length[i]<INFINITY)
            printf("%d\n",path_length[i]);
        else
            printf("Infinity\n");
    }
}
 
//Finds weight of the edge that connects Vertex u with Vertex v
int findEdgeWeight(Vertex u, Vertex v, Edge *edges, int *weights)
{
 
    int i;
    for(i = 0; i < E; i++)
    {
 
        if(edges[i].u == u.label && edges[i].v == v.label)
        {
            return weights[i];
        }
    }
    // If no edge exists, weight is infinity
    return INFINITY;
}
 
//Get the minimum path length among the paths 
int minimimPathLength(int *path_length, Vertex *vertices)
{
    int i;
    int min_path_length = INFINITY;
    for(i = 0; i < V; i++)
    {
        if(vertices[i].visited == true)
        {
            continue;
        }
       
        else if(vertices[i].visited == false && path_length[i] < min_path_length)
        {
            min_path_length = path_length[i];
           
        }
       
    }
    return min_path_length;
}
 
int minimimPathVertex(Vertex *vertices, int *path_length)
{
    int i;
    int min_path_length = minimimPathLength(path_length, vertices);
   
    //Get the vertex with the minimum path length
    //Mark it as visited
    for(i = 0; i < V; i++)
    {
        if(vertices[i].visited == false && path_length[vertices[i].label] == min_path_length)
        {
            vertices[i].visited = true;
            return i;
        }
    }
    return 0;
}
 
// Dijkstra Algorithm 
void Dijkstra_Parallel(Vertex *vertices, Edge *edges, int *weights, Vertex *root)
{  
    double parallel_start, parallel_end;
    int path_length[V];
   
    // Mark first vertex as visited, shortest path = 0
    root->visited = true;
    path_length[root->label] = 0;
    int i, j;
    // Compute distance to other vertices
   
    for(i = 0; i < V;i++)
    {
 
        if(vertices[i].label != root->label)
        {
            path_length[vertices[i].label] = findEdgeWeight(*root, vertices[i], edges, weights);
        }
        else
        {
            vertices[i].visited = true;
        }
    }
   
    parallel_start = omp_get_wtime();
    // External For Loop
    for(j = 0; j < V; j++)
    {
        Vertex u;
        // Obtain the vertex which has shortest distance and mark it as visited
        int h = minimimPathVertex(vertices, path_length);
        u = vertices[h];    
 
        //Update shortest path wrt new 0
        //Internal For Loop, Parallelising the computation
        #pragma omp parallel for schedule(static) private(i)
        for(i = 0; i < V; i++)
        {
            if(vertices[i].visited == false)
            {  
                int c = findEdgeWeight( u, vertices[i], edges, weights);
                path_length[vertices[i].label] = std::min(path_length[vertices[i].label], path_length[u.label] + c);
            }
        }
    }
   
    parallel_end = omp_get_wtime();
    printShortestPathLength(path_length);
    printf("\nRunning time: %lf ms\n", (parallel_end - parallel_start)*1000);
}
 
void convertStrtoArr(string str)
{
    int str_length = str.length();
 
    int j = 0, i;
 
    for (i = 0; i < 3; i++)
    {
        arr[i] = 0;
    }
   
    // Traverse the string
    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ' ')
        {
            j++;
        }
        else
        {
            arr[j] = arr[j] * 10 + (str[i] - 48);
        }
    }
 
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            //cout << arr[j] << " ";
            graph[i][j] = arr[j];
        }
    }
}

int main()
{
    FILE* city = fopen("city2.txt", "r");

    char c;
    int n, num = 0, i = 0, j = 0;
    int graph[100][100];
    E = 0;

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
            if(num != 0)
                E++;
            num = 0;
            j++;
        }

        else if(int(c) == 10 || int(c) == -1)
        {
            graph[i][j] = num;
            if(num != 0)
                E++;
            n = j;
            num = 0;
            j = 0;
            i++;
        }
    }

    n++;
    fclose(city);
 
    V = i;
    int k = 0;
 
    Vertex vertices[V];
    Edge edges[E];
    int weights[E];
 
    for(i = 0; i < V; i++)
    {
        Vertex a = { .label =i , .visited=false};
        vertices[i] = a;
    }

    for(i = 0; i < V; i++)
    {
        for(j = 0; j < V; j++)
        {
            if(graph[i][j] != 0)
            {
                E++;
                Edge e = {.u = i , .v = j};
                edges[k] = e;
                weights[k] = graph[i][j];
                k++;
            }
        }
    }
    Vertex root = {0, false};
    Dijkstra_Parallel(vertices, edges, weights, &root);
    return 0;
}
