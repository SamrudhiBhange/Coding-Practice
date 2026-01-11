#include<iostream>
using namespace std;

int main()
{
    int arr[] = {2,4,6,8,10};
    int size = sizeof(arr)/sizeof(arr[0]);
    int sum = 0;

    //for loop to calculate sum
    for(int idx = 0; idx < size; idx++){
        sum += arr[idx];
    }

    cout << "Sum of elements: " << sum << endl;
    return 0;
}