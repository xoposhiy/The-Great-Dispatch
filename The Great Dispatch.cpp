#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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

    for(int i=0;i<boxCount;i++)
    {
        cout<<boxes[i].truckId<<" ";
    }

    cout<<endl;
}