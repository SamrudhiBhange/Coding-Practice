#include<iostream>
using namespace std;

int main()
{
    int a = 4;
    cout << sizeof(a) << endl;

    char c = 's';
    cout << sizeof(c) << endl;

    bool flag;
    a == c ? flag = true : flag = false;
    cout << flag << endl;

    cout << (&a) << endl;