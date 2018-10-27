#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <semaphore.h>
#include <sys/time.h>

#define INF 99999
#define NONUM "-1"

using namespace std;

typedef struct {
    int n;
    int i;
    int k;
}args;


int **dist;
int **graph;

//sem_t sem_distLock;

pthread_mutex_t distLock;

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
        temp=NONUM;
        getline(content,temp,' ');
        numOfVertex=atoi(temp.c_str());
        temp=NONUM;
        getline(content,temp,' ');
        numOfEdge=atoi(temp.c_str());
        temp=NONUM;
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
        temp=NONUM;
        getline(content,temp,' ');
        j=atoi(temp.c_str())-1;
        temp=NONUM;
        getline(content,temp,' ');
        k=atoi(temp.c_str())-1;
        temp=NONUM;
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
            cout<<"The weight of edge between "<<j+1<<" and "<<k+1<<" exists"<<endl;
            cout<<"Input again"<<endl;
            i--;
        }
        else
        {
            graph[j][k]=edge;
            graph[k][j]=edge;
            dist[j][k]=edge;
            dist[k][j]=edge;
        }
    }
}

void displayMatrix(args *argument)
{
    int n=argument[0].n;
    if(n>50)
    {
        cout<<"There are "<< n <<" vertices"<<endl;
        cout<<"The matrix is too large to display"<<endl;
    }
    else
    {
        cout<<"The shortest path of original matrix is"<<endl;
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(dist[i][j]==INF)
                    cout<<"INF ";
                else
                    cout<<dist[i][j]<<" ";
            }
            cout<<endl;
        }
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
        pthread_mutex_lock(&distLock);
        if((dist[i][k]+dist[k][j])<dist[i][j])
        {
            //release write lock
            pthread_mutex_unlock(&distLock);
            //acquire write lock
            pthread_mutex_lock(&distLock);
            dist[i][j]=dist[i][k]+dist[k][j];
            //release write lock
            pthread_mutex_unlock(&distLock);
        }
        else
        {
            //release read lock
            pthread_mutex_unlock(&distLock);
        }
    }



    pthread_exit(NULL);
}

void shortestPath(args *argument)
{

//    sem_init(&sem_distLock,0,1);

    pthread_mutex_init(&distLock,NULL);

    double totalTime=0.0;

    timeval startTime,endTime;

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

        gettimeofday(&startTime,0);

        for(int i=0;i<numOfVex;i++)
        {
            pthread_join(*(threads+i),NULL);
        }

        gettimeofday(&endTime,0);

        double timeuse =1000000*(endTime.tv_sec - startTime.tv_sec)+ endTime.tv_usec - startTime.tv_usec;
        timeuse/=1000000;
        totalTime+=timeuse;
        printf("Finished running %d time(s) in %f s\n",k+1,timeuse);
    }
    printf("Finished running in %f s\n\n",totalTime);
}

int main()
{

    args *argument;

    initialMatrix(argument);

    shortestPath(argument);

    displayMatrix(argument);

    return 0;
}