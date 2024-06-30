#include<stdio.h>
#include<stdlib.h>

int * push_array(int * array , size_t * capacity , size_t * size , int n){
    if(*size>=*capacity){
        if(*capacity==0){
            *capacity=1;
        }
        *capacity=(*capacity)*2;
        array = (int *) realloc(array , (*capacity)*sizeof(*array));
    }
    array[*size]=n;
    (*size)++;
    return array;
}

int main(){
    printf("Points:\n");
    char start;
    char end;
    int i;
    if(scanf("%c",&start)!=1||start!='{'){
        printf("Invalid input.\n");
        // printf("1");
        return EXIT_FAILURE;
    }
    int * marks = NULL;
    size_t size=0 , capacity = 0;
    while(1){
        if(scanf(" %d %c",&i , &end)!=2||i>1000000000||i<0){
            printf("Invalid input.\n");
            free(marks);
            return EXIT_FAILURE;
        }
        if(end!=','&& end!='}'){
            printf("Invalid input.\n");
            // printf("2");
            free(marks);
            return EXIT_FAILURE;
        }
        marks = push_array(marks,&capacity,&size,i);
        if(end=='}')break;
    }
    
    

    printf("Intervals:\n");
    int a , b;
    char e , f;
    while(scanf(" %c%d;%d%c",&e,&a,&b,&f)==4){
        if(e!='(' && e!='<' && f!=')' && f!='>'){
            printf("Invalid input.\n");
            // printf("4");
            free(marks);
            return EXIT_FAILURE;
        }

        if(e=='(') a=a+1;
        if(f==')') b=b-1;
        int count=0;
        for(unsigned int i = 0 ; i < size ; i++){
            if(marks[i]>=a && marks[i]<=b){
                count++;
            }
        }
        printf(" -> %d\n",count);
    }
    if(!feof(stdin)){
        printf("Invalid input.\n");
        // printf("5");
        free(marks);
        return EXIT_FAILURE;
    }

    free(marks);


    return 0;
}

