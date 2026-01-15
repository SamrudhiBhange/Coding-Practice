#include <iostream>
#include <climits>

using namespace std;

// int largestElementIndex(int array[], int size)
// {
//     int max = INT_MIN;
//     int maxIndex = -1;
//     for(int i=0;i<size;i++)
//     {
//         if(array[i]>max)
//         {
//             max = array[i];
//             maxIndex=i;
//         }
//     }
//     return maxIndex;
// }

int secondLargestElementIndex(int array[], int size)
{
    int max = INT_MIN;
    int second_max = INT_MIN;

    for(int i=0;i<size;i++)
    {
        if(array[i]>max)
        {
            max = array[i];
        }
    }
    for(int i=0;i<size;i++)
    {
        if(array[i]>second_max && array[i]!=max)
        {
            second_max = array[i];
        }
    }
    return second_max; 
}


int main()
{
    int array[] = {2,3,5,7,6,1,7};
    int size = 7;

    // int firstLargestIndex = largestElementIndex(array, size);
    // cout<<array[firstLargestIndex]<<endl;
    // //array[firstLargestIndex] = -1;
    // int largestElement = array[firstLargestIndex];
    // for(int i=0;i<size;i++)
    // {
    //     if(array[i]==largestElement)
    //     {
    //         array[i] = -1;
    //     }
    // }

    // int secondLargestIndex = largestElementIndex(array, size);
    // cout<<array[secondLargestIndex]<<endl;

    cout<<secondLargestElementIndex(array, size)<<endl;

    return 0;
}


// 2,3,5,7,6,1,7
// firstLargestIndex = 3
// 2,3,5,7,6,1,7
// secondLargestIndex = 4

// 2,3,5,7,6,1,7
// max = 7
// second_max = 6
// if(array[i]>max) { max = array[i]; }

// if(array[i]>second_max && array[i]!=max) { second_max = array[i]; }