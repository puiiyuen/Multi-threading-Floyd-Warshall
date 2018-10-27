#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#define INF 99999

using namespace std;

typedef struct {
    int n;
    int i;
    int k;
}args;

int **dist;
int **graph;

bool checkNumber(int num)
{
    return (num>0)?true:false;
}

bool checkMatrix(int numOfVertex,int numOfEdge)
{
    if(checkNumber(numOfVertex)&&checkNumber(numOfEdge))
    {
        int maxSum=0;
        for(int i=0;i<numOfVertex;i++)
            maxSum+=i;
        return (maxSum<numOfEdge)?false:true;
    }
    else
        return false;
}

void initialMatrix(args *&argument)
{
    int numOfVertex=0,numOfEdge=0;
    string input,temp;

    do{
        cout<<"How many vertices and edges do you have:";
        getline(cin,input);
        istringstream content(input);
        getline(content,temp,' ');
        numOfVertex=atoi(temp.c_str());
        getline(content,temp,' ');
        numOfEdge=atoi(temp.c_str());
        if(!checkMatrix(numOfVertex,numOfEdge))
            cout<<"There is/are negetive number(s),or the number of edge exceed the maximum number"<<endl;
    }while (!checkMatrix(numOfVertex,numOfEdge));

    argument=(args *)malloc(numOfVertex * sizeof(args));
    graph=(int **)malloc(numOfVertex * sizeof(int *));
    dist=(int **)malloc(numOfVertex * sizeof(int *));
    for(int i=0;i<numOfVertex;i++)
    {
        argument[i].n=numOfVertex;
        graph[i]=(int *)malloc(numOfVertex * sizeof(int *));
        dist[i]=(int *)malloc(numOfVertex * sizeof(int *));
        for(int j=0;j<numOfVertex;j++)
        {
            if(i==j)
            {
                graph[i][j]=0;
                dist[i][j]=0;
            }
            else
            {
                graph[i][j]=INF;
                dist[i][j]=INF;
            }

        }
    }

    cout<<"Input 2 vertices and the weight of their edge:"<<endl;
    for(int i=0;i<numOfEdge;i++)
    {
        int j,k,edge;
        getline(cin,input);
        istringstream content(input);
        getline(content,temp,' ');
        j=atoi(temp.c_str())-1;
        getline(content,temp,' ');
        k=atoi(temp.c_str())-1;
        getline(content,temp,' ');
        edge=atoi(temp.c_str());
        if(!(checkNumber(j+1)&&checkNumber(k+1)&&checkNumber(edge))||j>=numOfVertex||k>=numOfVertex)
        {
            cout<<"This line contains illegal number(s)"<<endl;
            cout<<"Input again"<<endl;
            i--;
        }
        else if(graph[j][k]!=INF && j!=k)
        {
            cout<<"The weight of edge between "<<j<<" and "<<k<<"exists"<<endl;
            cout<<"Input again"<<endl;
            i--;
        }
        else
        {
            graph[j][k]=edge;
            graph[k][j]=edge;
            dist[i][k]=edge;
            dist[i][k]=edge;
        }
    }
}

void displayMatrix(args *argument)
{
    for(int i=0;i<argument[0].n;i++)
    {
        for(int j=0;j<argument[0].n;j++)
        {
            if(dist[i][j]==INF)
                cout<<"INF ";
            else
                cout<<dist[i][j]<<" ";
        }
        cout<<endl;
    }
}

void *worker(void *arg)
{
    args *argument=(args*)arg;

    int n=argument->n;
    int i=argument->i;
    int k=argument->k;

    for(int j=0;j<n;j++)
    {
        //acquire read lock
        if((dist[i][k]+dist[k][j])<dist[i][j])
        {
            //release read lock
            //acquire write lock
            dist[i][j]=dist[i][k]+dist[k][j];
            //release write lock
        }
    }
}

void shortestPath(args *argument)
{
    int numOfVex=argument[0].n;
    pthread_t *threads=(pthread_t *)malloc(numOfVex * sizeof(pthread_t *));
    for(int k=0;k<numOfVex;k++)
    {
        for(int i=0;i<numOfVex;i++)
        {
            argument[i].k=k;
            argument[i].i=i;
            pthread_create(threads+i,NULL,worker,(void *)&(argument[i]));
        }

        for(int i=0;i<numOfVex;i++)
        {
            pthread_join(*(threads+i),NULL);
        }
    }
}

int main()
{

    args *argument;
    initialMatrix(argument);

    shortestPath(argument);

    displayMatrix(argument);

    return 0;
}