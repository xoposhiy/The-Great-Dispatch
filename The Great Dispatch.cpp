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
    
    while(t.timeIs()<49000)
    {
        for(int aTruckId=0;aTruckId<100;aTruckId++)
        {
            for(int bTruckId=0;bTruckId<100;bTruckId++)
            {
                if(aTruckId!=bTruckId)
                {
                    vector<int> aTruckBoxes;
                    vector<int> bTruckBoxes;
                    
                    for(int i=0;i<boxCount;i++)
                    {
                        if(boxes[i].truckId==aTruckId)
                        {
                            aTruckBoxes.push_back(i);
                        }
                        
                        if(boxes[i].truckId==bTruckId)
                        {
                            bTruckBoxes.push_back(i);
                        }
                    }
                    
                    float minDelta=abs(trucks[aTruckId].weight-trucks[bTruckId].weight);
                    int aTruckBoxId=-1;
                    int bTruckBoxId=-1;
                    
                    for(int i=0;i<aTruckBoxes.size();i++)
                    {
                        for(int j=0;j<bTruckBoxes.size();j++)
                        {
                            float newATruckVolume=trucks[aTruckId].volume-boxes[aTruckBoxes[i]].volume+boxes[bTruckBoxes[j]].volume;
                            float newBTruckVolume=trucks[bTruckId].volume+boxes[aTruckBoxes[i]].volume-boxes[bTruckBoxes[j]].volume;
                            
                            float newATruckWeight=trucks[aTruckId].weight-boxes[aTruckBoxes[i]].weight+boxes[bTruckBoxes[j]].weight;
                            float newBTruckWeight=trucks[bTruckId].weight+boxes[aTruckBoxes[i]].weight-boxes[bTruckBoxes[j]].weight;
                            
                            float newDelta=abs(newATruckWeight-newBTruckWeight);
                            
                            if(newDelta<minDelta && newATruckVolume<=100 && newBTruckVolume<=100)
                            {
                                minDelta=newDelta;
                                aTruckBoxId=aTruckBoxes[i];
                                bTruckBoxId=bTruckBoxes[j];
                            }
                        }
                    }
                    
                    if(aTruckBoxId!=-1)
                    {
                        boxes[aTruckBoxId].truckId=bTruckId;
                        boxes[bTruckBoxId].truckId=aTruckId;
                        
                        trucks[aTruckId].volume=trucks[aTruckId].volume-boxes[aTruckBoxId].volume+boxes[bTruckBoxId].volume;
                        trucks[bTruckId].volume=trucks[bTruckId].volume+boxes[aTruckBoxId].volume-boxes[bTruckBoxId].volume;
            
                        trucks[aTruckId].weight=trucks[aTruckId].weight-boxes[aTruckBoxId].weight+boxes[bTruckBoxId].weight;
                        trucks[bTruckId].weight=trucks[bTruckId].weight+boxes[aTruckBoxId].weight-boxes[bTruckBoxId].weight;
                    }
                }
            }
        }
    }

    for(int i=0;i<boxCount;i++)
    {
        cout<<boxes[i].truckId<<" ";
    }

    cout<<endl;
}