#include<iostream>
#include<vector>
using namespace std;

int main()
{
    int arr[] = {1, 2, 1, 2, 1, 2};

    int ans_sum = 0;
    for(int i=0; i<6; i++)
    {
        if(i % 2 == 0)
        {
            ans_sum += arr[i];
        }
        else
        {
            ans_sum -= arr[i];
        }
    }
    cout<<ans_sum<<endl;
    return 0;


}