#include<iostream>
using namespace std;

int addFunc(int num1, int num2)
{
    int sum = num1 + num2;
    return sum;
}

int addFunc(int num1, int num2, int num3)
{
    int sum = num1 + num2 + num3;
    return sum;
}

float addFunc(float num1, float num2)
{
    float sum = num1 + num2;
    return sum;
}

int main()
{
    float a=3.4;
    float b=4.4;
    cout<<addFunc(a,b);
    //cout<<addFunc(4,5);
    //cout<<addFunc(4,4,3);

    return 0;
}