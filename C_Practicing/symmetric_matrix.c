#include<stdio.h>
#include<stdlib.h>
#define MAX_VALUE 102

int matrix[MAX_VALUE][MAX_VALUE];

int horizontal_symmtery(int matrix[][MAX_VALUE],int rows , int columns){
    for(int i=0,j=rows-1 ;i<rows/2;++i,--j){
        for(int k=0; k<columns-1; ++k){
            if(matrix[i][k]!=matrix[j][k]) return 0;
        }
    }

    return 1;
}

int vertical_symmtery(int matrix[][MAX_VALUE],int rows , int columns){
    for(int i=0,j=columns-1 ;i<columns/2;++i,--j){
        for(int k=0; k<rows-1; ++k){
            if(matrix[k][i]!=matrix[k][j]) return 0;
        }
    }
    return 1;
}

int Center_symmtery(int matrix[][MAX_VALUE],int rows , int columns){
    for(int i=0,j=rows-1 ;i<=rows/2;++i,--j){
        for(int k=0,l=columns-1; k<=columns/2; ++k,--l){
            if(matrix[i][k]!=matrix[j][l]) return 0;
        }
    }

    return 1;
}

int isInt(double n){
    int a = n;
    if((n-a)==0) return 1;
    else return 0;
}



int main(){
    printf("Matrix:\n");
    int rows=0,columns=0,i=0,j=0,check=0;
    char c = ' ';
    double x;
    

    while (scanf("%lf",&x) == 1)
    {   
        if(!isInt(x)){
            printf("Invalid input.\n");
            return EXIT_FAILURE;
        }

        matrix[i][j]= x;

        c=getchar();
        if(c=='\n'){
            if(i==0)check=j;
            if(j!=check){
                printf("Invalid input.\n");
                return EXIT_FAILURE;
            }
            i++;
            if(i>100){
                printf("Invalid input.\n");
                return EXIT_FAILURE;
            }
            j=0;
        }
        else{
        j++;
        if(j>99){
            printf("Invalid input.\n");
            return EXIT_FAILURE;
        }
        }
    }
    if(!feof(stdin)){
        printf("Invalid input.\n");
        return EXIT_FAILURE;
    }

    rows=i;
    columns=check+1;
    
    if(i==0){
        printf("Invalid input.\n");
        return EXIT_FAILURE;
    }

    if(horizontal_symmtery(matrix,rows,columns)){
        printf("Horizontal symmetry: yes\n");
    }
    else{
        printf("Horizontal symmetry: no\n");
    }

    if(vertical_symmtery(matrix,rows,columns)){
        printf("Vertical symmetry: yes\n");
    }
    else{
        printf("Vertical symmetry: no\n");
    }


    if(Center_symmtery(matrix,rows,columns)){
        printf("Center symmetry: yes\n");
    }
    else{
        printf("Center symmetry: no\n");
    }
    

    }





   




