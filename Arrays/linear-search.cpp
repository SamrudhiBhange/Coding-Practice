#include<iostream>
using namespace std;

int main()
{
    int arr[] = {12, 45, 7, 23, 89, 34};
    int key = 20; 

    int ans= -1; 

    for(int i=0; i<6; i++){
        if(arr[i] == key){
             ans = i; 
             break;
        }
    }
    cout << "Element found at index: " << ans << endl;
    return 0;
}
