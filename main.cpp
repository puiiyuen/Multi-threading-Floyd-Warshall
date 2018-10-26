#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

#define INF 9999

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

void createMatrix(args *&argument)
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
            cout<<"There is/are illegal number(s)"<<endl;
    }while (!checkMatrix(numOfVertex,numOfEdge));

    argument=(args *)malloc(numOfVertex * sizeof(args));
    dist=(int **)malloc(numOfVertex * sizeof(int *));
    for(int i=0;i<numOfVertex;i++)
    {
        argument->n=numOfVertex;
        dist[i]=(int *)malloc(numOfVertex * sizeof(int *));
        for(int j=0;j<numOfVertex;j++)
        {
            if(i==j)
                dist[i][j]=0;
            else
                dist[i][j]=INF;
        }
    }

//
//    for(int i=0;i<numOfVertex;i++)
//        for(int j=0;j<numOfVertex;j++)
//            dist[i][j]=INF;

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
        if(!(checkNumber(j+1)&&checkNumber(k+1)&&checkNumber(edge)))
        {
            cout<<"This line contains illegal number(s)"<<endl;
            cout<<"Input again"<<endl;
            i--;
        }
        else if(dist[j][k]!=INF && j!=k)
        {
            cout<<"The weight of edge between "<<j<<" and "<<k<<"exists"<<endl;
            cout<<"Input again"<<endl;
            i--;
        }
        else
        {
            dist[j][k]=edge;
            dist[k][j]=edge;
        }
    }
}

int main()
{

    args *argument;
    createMatrix(argument);
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
    return 0;
}