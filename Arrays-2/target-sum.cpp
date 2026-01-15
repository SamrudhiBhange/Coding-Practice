#include<iostream>

using namespace std;

int main()
{
    int array[] = {3,4,6,7,1};
    int target = 5;

    int pairs = 0;

    for(int i=0;i<5;i++)
    {
        for(int j=i+1;j<5;j++)
        {
            if(array[i]+array[j]==target)
            {
                pairs++;
            }
        }
    }
    cout<<"Total pairs: "<<pairs<<endl;
    return 0;
}