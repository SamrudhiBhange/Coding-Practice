#include<iostream>

using namespace std;

int main()
{
    int array[] = {3,1,2,4,0,6};
    int target = 5;
    int triplets = 0;

    for(int i=0; i<6; i++)
    {
        for(int j=i+1; j<6; j++){
            for(int k=j+1; k<6; k++)
            {
                if(array[i]+array[j]+array[k]==target)
                {
                    triplets++;
                }
            }
        }
    }
    cout<<"Total triplets: "<<triplets<<endl;
    return 0;
}