#define LEN 10

void quicksort(int numbers[], int left, int right)
{
    int i, j, aux, pivot = left;

    for(i = left+1; i <= right ; i++)
    {
        j = i;

        aux = numbers[j];
        if(numbers[j] < numbers[pivot])
        {
         while(j > pivot)
         {    
            numbers[j] = numbers[j-1];
            j--;
         }
         numbers[j] = aux;
         pivot++;        
        }  
    }
    if(pivot - 1 >= left){
        quicksort(numbers, left, pivot-1);
    }
    if(pivot + 1 <= right){
        quicksort(numbers, pivot+1, right);
    }
}
