#include<fstream>
#include<iostream>
#include<ctype.h>

using namespace std;

void display(int a[][100], int m, int n)
{
    for(int i = 0; i<m; i++)
    {
        for(int j = 0; j < n; j++)
            cout << a[i][j] <<" ";
        cout << endl;
    }
}

int main()
{
    FILE* city = fopen("city1.txt", "r");

    char c;
    int n, num = 0, i = 0, j = 0;
    int graph[100][100];

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

        else if(c == '\n')
        {
            n = j;
            num = 0;
            j = 0;
            i++;
        }
    }

    fclose(city);

    display(graph,i,n);

    return 0;
}

