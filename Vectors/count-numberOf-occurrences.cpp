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

    int occurrence = 0;

    for(int ele:v){
        if(ele == x){
            occurrence++;
        }
    }
    cout<<"Number of occurrences of x is: "<<occurrence<<endl;
}