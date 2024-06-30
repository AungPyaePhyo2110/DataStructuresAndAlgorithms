#include<stdio.h>
#include<stdlib.h>
#define MAX_VALUE 2100

int find_sum(int start , int end , int sum[]){
    if(start>0)
    return sum[end]-sum[start-1];
    else
    return sum[end];
}

int find_range(int number){
    return (number)*(number-1)/2;
}



int * push_array (int * array , size_t * capacity , size_t * size , int number ){
    if(*size >= *capacity){
        if(*capacity==0) * capacity = 1;
        * capacity = ( * capacity ) * 2;
    }
    array = (int *)realloc(array, (*capacity)*sizeof(array));
    array[* size]=number;
    (* size)++;
    return array;
}

int intCmp( int *a , int * b){
    return (*a>*b)-(*b>*a);
}


int main(){
    int total=0;
    int * sum = NULL;
    size_t capacity = 0;
    size_t size = 0;
    int x;
    printf("Input sequence:\n");
    while (scanf("%d",&x)==1){
        total+=x;
       sum = push_array(sum,&capacity,&size,total);
        if(size>=2000){
             printf("Invalid input.\n");
            return EXIT_FAILURE;
        }
    }
    if(!feof(stdin)){
        printf("Invalid input.\n");
        free(sum);
        return EXIT_FAILURE;
    }

    if(size==0){
        printf("Invalid input.\n");
        free(sum);
        return EXIT_FAILURE;
    }
    if(size == 1) {
        free(sum);
        printf("Total pairs: 0\n");
        return EXIT_SUCCESS;
    }


    int range = find_range(size);


    int * test = NULL;
    size_t test_capacity =0;
    size_t test_size=0;
    for(unsigned int k=0 ; k<=size-2 ; ++k){
        for(unsigned int n=k+1 ; n<=size-1 ; ++n){
            test = push_array(test,&test_capacity,&test_size,find_sum(k,n,sum));
        }
    }

    free(sum);

    qsort ( test, range, sizeof ( * test ), (int (*)(const void*, const void*)) intCmp );
    int pairs=0,count=0;
    for(int n=0 ; n<range ; ++n){
        if(test[n]==test[n+1]){
            count++;
        }
        else{
            pairs += count * (count+1) / 2;
            count = 0;
        }
    }

    free(test);
    
    printf("Total pairs: %d\n",pairs);

    
    return EXIT_SUCCESS;
}
