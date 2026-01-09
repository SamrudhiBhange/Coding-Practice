#include<iostream>
using namespace std;

int add(int a, int b=1, int c){
    return (a+b+c);
}

int main()
{
    cout<<add(2)<<endl;
    cout<<add(2,2)<<endl;
    cout<<add(2,2,3)<<endl;

    return 0;
}