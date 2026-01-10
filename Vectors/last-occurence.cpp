#include<iostream>
#include<vector>
using namespace std;

int main()
{
    vector<int> v(6);
    for(int i=0; i<v.size(); i++)
    {
        cin>>v[i];
    }

    cout<<"Enter x to find its last occurrence: "<<endl;
    int x;
    cin>>x;

    int occurrence = -1;
    // for loop from the start to find the last occurrence
    // for(int i=0; i<v.size(); i++)
    // {
    //     if(v[i] == x)
    //     {
    //         occurrence = i;
    //     }
    // }

    // for loop from the end to find the last occurrence 
    for(int i=v.size()-1; i>=0; i--)
    {
        if(v[i] == x)
        {
            occurrence = i;
            break;
        }
    }

    cout<<"Last occurrence of x is at index: "<<occurrence<<endl;

    return 0;
}