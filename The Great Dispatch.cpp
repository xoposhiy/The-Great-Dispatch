#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <unordered_set>
#include <algorithm>

#define population 100

using namespace std;

default_random_engine re(chrono::system_clock::now().time_since_epoch().count());

struct timer
{
    chrono::high_resolution_clock::time_point t1;

    void start()
    {
        t1=chrono::high_resolution_clock::now();
    }

    int timeIs()
    {
        chrono::duration<double,std::milli> time_span=chrono::high_resolution_clock::now()-t1;

        return (int)time_span.count();
    }
};

timer t;

struct box
{
   float weight;
   float volume;
};

struct solution
{
    int score;
    int boxes[3300];
    float trucksWeight[100];
    float trucksVolume[100];
    
    solution()
    {
        score=-1;
        
        for(int i=0;i<100;i++)
        {
            trucksWeight[i]=0;
            trucksVolume[i]=0;
        }
    }
};

bool sortByScore(solution a,solution b)
{
    return a.score>b.score;
}

struct carPark
{
    int boxCount;
    unordered_set<int> selectedSolutions;
    vector<solution> solutions;
    box boxes[3300];
    
    carPark()
    {
        cin>>boxCount;cin.ignore();
        
        t.start();
    
        for(int i=0;i<boxCount;i++)
        {
            cin>>boxes[i].weight>>boxes[i].volume;
            cin.ignore();
        }
    }
    
    void populate()
    {        
        uniform_int_distribution<int> distrib{0,99};
        
        for(int i=0;i<population;i++)
        {
            solution s;
            solutions.push_back(s);
            
            for(int j=0;j<boxCount;j++)
            {
                int truck=distrib(re);
                
                while(solutions[i].trucksVolume[truck]+boxes[j].volume>100)
                {
                    truck=distrib(re);
                }

                solutions[i].boxes[j]=truck;
                
                solutions[i].trucksVolume[truck]+=boxes[j].volume;
                solutions[i].trucksWeight[truck]+=boxes[j].weight;
            }
        }
    }
    
    int fitness(int solutionId)
    {
        float max=0;
        float min=100;
        
        for(int i=0;i<100;i++)
        {
            if(solutions[solutionId].trucksWeight[i]<min)
            {
                min=solutions[solutionId].trucksWeight[i];
            }
            
            if(solutions[solutionId].trucksWeight[i]>max)
            {
                max=solutions[solutionId].trucksWeight[i];
            }
        }
        
        return round(100000.0/(max-min));
    }
    
    int sumScores()
    {
        int sum=0;
        
        for(int i=0;i<population;i++)
        {
            sum+=solutions[i].score;
        }
        
        return sum;
    }
    
    int wichSolution(int randomScore)
    {
        int count=0;
        
        for(int i=0;i<population;i++)
        {
            count+=solutions[i].score;
            
            if(count>=randomScore)
            {
                return i;
            }
        }
    }
    
    void selection()
    {
         uniform_int_distribution<int> distrib{0,sumScores()};
         
         while(selectedSolutions.size()<population/2)
         {
             selectedSolutions.insert(wichSolution(distrib(re)));
         }
    }
    
    void mutation()
    {
        unordered_set<int>::iterator it=selectedSolutions.begin();
        
        for(it;it!=selectedSolutions.end();it++)
        {
            solutions.push_back(solutions[*it]);
            
            float max=0;
            int maxId;
            
            for(int i=0;i<100;i++)
            {
                if(solutions.back().trucksWeight[i]>max)
                {
                    max=solutions.back().trucksWeight[i];
                    maxId=i;
                }
            }
            
            float min=max;
            int minId;
            
            for(int i=0;i<100;i++)
            {
                if(solutions.back().trucksWeight[i]<min)
                {
                    min=solutions.back().trucksWeight[i];
                    minId=i;
                }
            }
            
            vector<int> maxBoxes;
            vector<int> minBoxes;
            
            for(int i=0;i<boxCount;i++)
            {
                if(solutions.back().boxes[i]==maxId)
                {
                    maxBoxes.push_back(i);
                }
                
                if(solutions.back().boxes[i]==minId)
                {
                    minBoxes.push_back(i);
                }
            }
            
            for(int i=0;i<maxBoxes.size();i++)
            {
                if(solutions.back().trucksWeight[minId]<solutions.back().trucksWeight[maxId] && solutions.back().trucksVolume[minId]+boxes[maxBoxes[i]].volume<100)
                {
                    solutions.back().boxes[maxBoxes[i]]=minId;
                        
                    solutions.back().trucksWeight[maxId]-=boxes[maxBoxes[i]].weight;
                    solutions.back().trucksWeight[minId]+=boxes[maxBoxes[i]].weight;
                        
                    solutions.back().trucksVolume[maxId]-=boxes[maxBoxes[i]].volume;
                    solutions.back().trucksVolume[minId]+=boxes[maxBoxes[i]].volume;
                    
                    minBoxes.push_back(maxBoxes[i]);
                    maxBoxes[i]=-1;
                }
            }
            
            bool swapped=false;
            
            for(int i=0;i<maxBoxes.size();i++)
            {
                if(swapped)
                {
                    break;
                }
                
                if(maxBoxes[i]!=-1)
                {                    
                    for(int j=0;j<minBoxes.size();j++)
                    {
                        float newMinWeight=solutions.back().trucksWeight[minId]+boxes[maxBoxes[i]].weight-boxes[minBoxes[j]].weight;
                        float newMaxWeight=solutions.back().trucksWeight[maxId]-boxes[maxBoxes[i]].weight+boxes[minBoxes[j]].weight;
                        
                        float newMinVolume=solutions.back().trucksVolume[minId]+boxes[maxBoxes[i]].volume-boxes[minBoxes[j]].volume;
                        float newMaxVolume=solutions.back().trucksVolume[maxId]-boxes[maxBoxes[i]].volume+boxes[minBoxes[j]].volume;
                        
                        if(newMinVolume<100 && newMaxVolume<100)
                        {
                            solutions.back().boxes[maxBoxes[i]]=minId;
                            solutions.back().boxes[minBoxes[j]]=maxId;
                            
                            solutions.back().trucksWeight[maxId]=newMaxWeight;
                            solutions.back().trucksWeight[minId]=newMinWeight;
                            
                            solutions.back().trucksVolume[maxId]=newMaxVolume;
                            solutions.back().trucksVolume[minId]=newMinVolume;
                            
                            swapped=true;
                            break;
                        }
                    }
                }
            }
            
        }
    }
    
    void load()
    {
        populate();

        for(int i=0;i<population;i++)
        {
            solutions[i].score=fitness(i);
        }
        
        while(t.timeIs()<49000)
        {
            selection();
            
            mutation();
            
            for(int i=0;i<solutions.size();i++)
            {
                if(solutions[i].score=-1)
                {
                    solutions[i].score=fitness(i);
                }
            }
            
            sort(solutions.begin(),solutions.end(),sortByScore);
            
            while(solutions.size()!=population)
            {
                solutions.pop_back();
            }
        }
    }
    
    void start()
    {
        for(int i=0;i<boxCount;i++)
        {
            cout<<solutions[0].boxes[i]<<" ";
        }
        
        cout<<endl;
    }
};

int main()
{
    carPark convoy;
    
    convoy.load();
    
    convoy.start();
}