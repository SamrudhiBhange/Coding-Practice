#include <iostream>

using namespace std;

int main()
{
    int array[] = {1,2,3,4,5};
    int size = 5;
    int k = 7; // number of rotations

    // Normalize k if it's greater than size
    k = k % size;

    // Create a temporary array to hold the rotated values
    int ansArray[5];
    int j=0;

    // Fill the temporary array with rotated values
    for(int i=size-k; i<size; i++)
    {
        ansArray[j++] = array[i];
    }

    // Copy the temporary array back to the original array
    for(int i=0; i<=k; i++)
    {
        ansArray[j++] = array[i];
    }

    for(int i=0; i<size; i++)
    {
        cout << ansArray[i] << " ";
    }
    cout << endl;

    return 0;
}