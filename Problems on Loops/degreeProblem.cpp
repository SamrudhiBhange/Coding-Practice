#include<iostream>
using namespace std;

int main()
{
    int n,m;
    cin>>n>>m;

    int result=1;
    for(int i=1;i<=m;i++){
        result *= n;
    }
    cout<<result<<endl;
    return 0;
}