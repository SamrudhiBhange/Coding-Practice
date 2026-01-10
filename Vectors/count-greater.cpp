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

    cout<<"Enter x to count number of occurrence: "<<endl;
    int x;
    cin>>x;

    int count = 0;
    for(int i=0; i<v.size(); i++)
    {
        if(v[i] > x)
        {
            count++;
        }
    }
    cout<<"Number of elements greater than x is: "<<count<<endl;
    return 0;
}