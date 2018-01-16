#include <iostream>
#include <vector>

using namespace std;

struct box
{
   float weight;
   float volume;
   int truckId;
   
   box()
   {
       truckId=-1;
   }
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

int nextTruck(int truckId)
{
    return (truckId+1)%100;
}

int main()
{
    int boxCount;
    int currentTruck=0;
    float targetWeight=0;
    truck trucks[100];
    vector<box> boxes;

    cin>>boxCount;cin.ignore();
    
    for(int i=0;i<boxCount;i++)
    {
        box b;
        
        cin>>b.weight>>b.volume;cin.ignore();
        
        targetWeight+=b.weight;
        
        boxes.push_back(b);
    }

    targetWeight/=100.0;
    
    for(int i=0;i<boxCount;i++)
    {
        int bestTruck=0;
        float bestWeightRatio=1000;
        
        for(int j=currentTruck;j<100;j++)
        {
            float weightRatio=abs((trucks[j].weight+boxes[i].weight)-targetWeight);
            
            if(trucks[j].volume+boxes[i].volume<=100 && weightRatio<bestWeightRatio)
            {
                bestTruck=j;
                bestWeightRatio=weightRatio;
            }
            
            if(bestTruck==0 && j==99 && currentTruck!=0)
            {
                j=0;
            }
        }
        
        boxes[i].truckId=bestTruck;
        trucks[bestTruck].volume+=boxes[i].volume;
        trucks[bestTruck].weight+=boxes[i].weight;
        
        currentTruck=nextTruck(currentTruck);
    }

    for(int i=0;i<boxCount;i++)
    {
        cout<<boxes[i].truckId<<" ";
    }
    
    cout<<endl;
}