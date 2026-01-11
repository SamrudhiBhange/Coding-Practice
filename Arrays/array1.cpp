#include<iostream>
using namespace std;

int main()
{
    int arr[] = {1,3,5,7,9};
    // cout<<sizeof(arr)<<endl;
    // cout<<sizeof(arr)/sizeof(arr[0])<<endl;
    int size = sizeof(arr)/sizeof(arr[0]);

    //for loop
    for(int idx = 0;idx<size;idx++){
        //cout<<arr[idx]<<endl;
    }

    //for each loop
    for(int ele:arr){
       // cout<<ele<<endl;
    }

    //while loop
    int idx = 0;
    while(idx < size){
      //  cout<<arr[idx]<<endl;
        idx++;
    }
    
    char vowels[5];
    //int size=sizeof(vowels);
    
    //for loop to take input
    // for(int idx=0; idx<size; idx++){
    //     cin>>vowels[idx];
    // }

    //for each loop to take input
    for(char &element:vowels){
        cin>>element;
    }

    for(int idx=0; idx<size; idx++){
        cout<<vowels[idx]<<" ";
    }
    return 0;
}