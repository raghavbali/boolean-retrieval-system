#include<stdio.h>


void exchange(int i, int j,double *numbers)
{
    int temp = numbers[i];
    numbers[i] = numbers[j];
    numbers[j] = temp;
}


void quicksort(int low, int high,double *numbers)
{
    int i = low, j = high;
    double pivot = numbers[low + (high-low)/2];

    // Divide into two lists
    while (i <= j)
    {

        while (numbers[i] < pivot)
        {
            i++;
        }

        while (numbers[j] > pivot)
        {
            j--;
        }

        if (i <= j)
        {
  //          printf("\n quick i=%d j=%d ni=%lf nj=%lf",i,j,numbers[i],numbers[j]);
            exchange(i, j,numbers);
            i++;
            j--;
        }
    }
    if (low < j)
        quicksort(low, j,numbers);
    if (i < high)
        quicksort(i, high,numbers);
}

/*
int main()
{
    int size=0;
    int difference=0;
    int i=0;
    int counter=0;
    double numbers[1000];
    scanf("%d %d",&size,&difference);
    for(i=0;i<size;i++)
    {
        scanf("%lf", &numbers[i]);
    }
    quicksort(0,size,numbers);
    for(i=1;i<size+1;i++)
    {
        printf("%lf ",numbers[i]);

    }
    return 0;
}
*/
