#include<iostream>
using namespace std;

int main()
{
    int arr[] = {12, 45, 7, 23, 89, 34};
    int size = sizeof(arr)/sizeof(arr[0]);
    int maxNum = arr[0];

    //for loop to find maximum number
    for(int idx = 1; idx < size; idx++){
        if(arr[idx] > maxNum){
            maxNum = arr[idx];
        }
    }

    cout << "Maximum number in the array: " << maxNum << endl;
    return 0;
}