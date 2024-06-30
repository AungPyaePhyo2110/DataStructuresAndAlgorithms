#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAX 200

typedef struct 
{   
    double x_coordinate;
    double y_coordinate;
    char name[MAX]; 
}airplane;

airplane * push_array (airplane * input ,size_t * size, size_t * capacity,
                         double * cor_x ,  double * cor_y ){
        if((*size)>=*capacity){
            if(*capacity==0){
                *capacity= 1;
            }
            *capacity=(*capacity)*2;
            input = (airplane *)realloc(input,(*capacity)*(sizeof(*input)));
        }  
        input[*size].x_coordinate= *cor_x;
        input[*size].y_coordinate= *cor_y;
        (*size)++;
        return input;
}



double find_distance(double x1 , double y1 , double x2, double y2){
    double sum = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    return sqrt(sum);
}


int main(){
    double cor_x,cor_y;
    size_t size=0 , capacity = 0;
    char string[MAX];
    airplane * input = NULL;
    printf("Plane coordinates:\n");
    while(scanf("%lf,%lf: %199s",&cor_x,&cor_y,string)==3){
        input = push_array(input,&size,&capacity,&cor_x,&cor_y);
        strncpy((char*)input[size-1].name,string,sizeof(input[0].name));
    }
    if(!feof(stdin)){
        printf("Invalid input.\n");
        free(input);
        return EXIT_FAILURE;
    }
    if(size<=1){
        printf("Invalid input.\n");
        free(input);
        return EXIT_FAILURE;
    }


    //finding minimum distance
    double min = find_distance(input[0].x_coordinate,input[0].y_coordinate,
                                        input[1].x_coordinate,input[1].y_coordinate);
    for(unsigned int i = 0 ; i < size ; ++i){
        for (unsigned int j = i+1 ; j<size ; ++j){
            double distance = find_distance(input[i].x_coordinate,input[i].y_coordinate,
                                        input[j].x_coordinate,input[j].y_coordinate);
            if(distance<min){
                min = distance;
            }
        }
    }    
    printf("Minimum airplane distance: %lf\n",min);


    //finding pairs
    int pairs=0;
    for(unsigned int k = 0 ; k < size ; ++k){
        for (unsigned int m = k+1 ; m<size ; ++m){
            double check=find_distance(input[k].x_coordinate,input[k].y_coordinate,
                                        input[m].x_coordinate,input[m].y_coordinate);
            if(check-min<=__DBL_EPSILON__*check*min*1000){
                pairs++;
            }

        }
    }
    printf("Pairs found: %d\n",pairs);
    

    //printing the names of pairs
    for(unsigned int k = 0 ; k < size ; ++k){
        for (unsigned int m = k+1 ; m<size ; ++m){
            double check=find_distance(input[k].x_coordinate,input[k].y_coordinate,
                                        input[m].x_coordinate,input[m].y_coordinate);
            if(check-min<=__DBL_EPSILON__*check*min*1000){
                printf("%s - %s\n",input[k].name , input[m].name);
            }

        }
    }
    
    free(input);
    return 0;
}

