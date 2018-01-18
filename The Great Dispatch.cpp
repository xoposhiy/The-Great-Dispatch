#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

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
    int id;
    float weight;
    float volume;
    int truckId;
};

struct truck
{
    float weight;
    float volume;
    
    truck()
    {
        weight=0;
        volume=0;
    }
};

bool sortByWeight(box a,box b)
{
    return a.weight>b.weight;
}

bool sortById(box a,box b)
{
    return a.id<b.id;
}

int main()
{
    int boxCount;
    float targetWeight=0;
    vector<box> boxes;
    truck trucks[100];
    
    cin>>boxCount;cin.ignore();
    
    t.start();
    
    for(int i=0;i<boxCount;i++)
    {
        box b;
        
        b.id=i;
        
        cin>>b.weight>>b.volume;cin.ignore();
        
        targetWeight+=b.weight;
        
        boxes.push_back(b);
    }
    
    targetWeight/=(float)boxCount;
    
    sort(boxes.begin(),boxes.end(),sortByWeight);
    
    for(int i=0;i<boxCount;i++)
    {
        float minDelta=10000.0;
        int minDeltaId;
        
        for(int j=0;j<100;j++)
        {
            float delta=abs(targetWeight-(trucks[j].weight+boxes[i].weight));
            
            if(delta<minDelta && trucks[j].volume+boxes[i].volume<=100)
            {
                minDelta=delta;
                minDeltaId=j;
            }
        }
        
        boxes[i].truckId=minDeltaId;
        
        trucks[minDeltaId].volume+=boxes[i].volume;
        trucks[minDeltaId].weight+=boxes[i].weight;
    }
    
    sort(boxes.begin(),boxes.end(),sortById);
    
    while(t.timeIs()<5000)
    {
        float maxTruckWeight=0;
        int maxTruckId;
        
        for(int i=0;i<100;i++)
        {
            if(trucks[i].weight>maxTruckWeight)
            {
                maxTruckWeight=trucks[i].weight;
                maxTruckId=i;
            }
        }
        
        float minTruckWeight=maxTruckWeight;
        int minTruckId;
        
        for(int i=0;i<100;i++)
        {
            if(trucks[i].weight<minTruckWeight)
            {
                minTruckWeight=trucks[i].weight;
                minTruckId=i;
            }
        }
        
        vector<int> minTruckBoxes;
        vector<int> maxTruckBoxes;
        
        for(int i=0;i<boxCount;i++)
        {
            if(boxes[i].truckId==minTruckId)
            {
                minTruckBoxes.push_back(i);
            }
            
            if(boxes[i].truckId==maxTruckId)
            {
                maxTruckBoxes.push_back(i);
            }
        }
        
        float minDelta=abs(maxTruckWeight-minTruckWeight);
        int minTruckBoxId=-1;
        int maxTruckBoxId=-1;
        
        for(int i=0;i<minTruckBoxes.size();i++)
        {
            for(int j=0;j<maxTruckBoxes.size();j++)
            {
                float newMinTruckVolume=trucks[minTruckId].volume+boxes[maxTruckBoxes[j]].volume-boxes[minTruckBoxes[i]].volume;
                float newMaxTruckVolume=trucks[maxTruckId].volume-boxes[maxTruckBoxes[j]].volume+boxes[minTruckBoxes[i]].volume;
                
                float newMinTruckWeight=trucks[minTruckId].weight+boxes[maxTruckBoxes[j]].weight-boxes[minTruckBoxes[i]].weight;
                float newMaxTruckWeight=trucks[maxTruckId].weight-boxes[maxTruckBoxes[j]].weight+boxes[minTruckBoxes[i]].weight;
                
                float newDelta=abs(newMaxTruckWeight-newMinTruckWeight);
                
                if(newDelta<minDelta && newMinTruckVolume<=100 && newMaxTruckVolume<=100)
                {
                    minDelta=newDelta;
                    minTruckBoxId=minTruckBoxes[i];
                    maxTruckBoxId=maxTruckBoxes[j];
                }
            }
        }
        
        if(minTruckBoxId!=-1)
        {
            boxes[minTruckBoxId].truckId=maxTruckId;
            boxes[maxTruckBoxId].truckId=minTruckId;
            
            trucks[minTruckId].volume=trucks[minTruckId].volume+boxes[maxTruckBoxId].volume-boxes[minTruckBoxId].volume;
            trucks[maxTruckId].volume=trucks[maxTruckId].volume-boxes[maxTruckBoxId].volume+boxes[minTruckBoxId].volume;

            trucks[minTruckId].weight=trucks[minTruckId].weight+boxes[maxTruckBoxId].weight-boxes[minTruckBoxId].weight;
            trucks[maxTruckId].weight=trucks[maxTruckId].weight-boxes[maxTruckBoxId].weight+boxes[minTruckBoxId].weight;
        }
        else
        {
            break;
        }
    }

    for(int i=0;i<boxCount;i++)
    {
        cout<<boxes[i].truckId<<" ";
    }

    cout<<endl;
}