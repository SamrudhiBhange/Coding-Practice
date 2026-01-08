#include<iostream>
using namespace std;

int main()
{
    int n;
    cin>>n;

    int digits = 0;
    while(n>0){
        digits++;   //1
        n=n/10;     //61
    }
    cout<<digits<<endl;
    return 0;
}