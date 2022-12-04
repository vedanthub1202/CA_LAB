/*
g++ -pg quicksort.cpp -o quicksort
./quicksort
gprof quicksort > output.txt
*/


#include <bits/stdc++.h>
using namespace std;

void printArray(int num[], int n) {
  int i;
  for (i = 0; i < n; i++)
    cout << num[i] << " ";
  cout << endl;
}


void swap(int *num,int x,int y){
    int temp = num[x];
    num[x] = num[y];
    num[y] = temp;
}


int partition(int num[], int low, int high)
{
    int pivot = num[low];

    int cnt = 0;
    for (int i = low + 1; i <= high; i++)
    {
        if (num[i] <= pivot)
        {
            cnt++;
        }
    }
    int pivotIndex = low + cnt;
    swap(num,pivotIndex,low);
    int i = low, j = high;
    while (i < pivotIndex && j > pivotIndex)
    {
        while (num[i] <= pivot)
        {
            i++;
        }
        while (num[j] > pivot)
        {
            j--;
        }
        if (i < pivotIndex && j > pivotIndex)
        {
            swap(num,i++,j--);
        }
    }
    return pivotIndex;
}

void quickSort(int num[], int low, int high)
{

    if (low >= high)
        return;
    int p = partition(num, low, high);
    quickSort(num, low, p - 1);
    quickSort(num, p + 1, high);
}


int main(int argc, char const *argv[])
{
    int n;
    cout<<"Enter the value of N: ";
    cin>>n;
    int num[n];

    for(int i=0;i<n;i++)
    {
        num[i] = n-i;
    }
    quickSort(num, 0, n-1);
    
    printArray(num,n);
    cout << endl;

    return 0;
}