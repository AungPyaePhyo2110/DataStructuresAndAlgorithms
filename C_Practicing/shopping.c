/*
#0
a
#2
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define MAX 200


void remove_NL(char *s){
    int length = strlen(s);
    if(length > 0 && s[length-1]=='\n'){
        s[length-1]='\0';
    }
}

typedef struct 
{
    int items;
    char ** shelf_list;
}shelf;

typedef struct 
{   int items;
    int * found;
    int * found_shelf_line;
    char ** shopping_list;
}shopping;


shelf * allocate_shelves ( shelf * shelves , size_t *shelf_size , size_t *shelf_capacity){
    if(*shelf_size>=*shelf_capacity){
        if(*shelf_capacity==0) * shelf_capacity = 1;
        else *shelf_capacity = (*shelf_capacity)*2;
        shelves = (shelf *) realloc (shelves , (*shelf_capacity)*sizeof(*shelves));
    }

    (*shelf_size)++;
    return shelves;
}

shopping * allocate_lists ( shopping * lists , size_t *list_size, size_t *list_capacity){
    if(*list_size>=*list_capacity){
        if(*list_capacity==0) * list_capacity = 1;
        else *list_capacity = (*list_capacity)*2;
        lists = (shopping *) realloc (lists , (*list_capacity)*sizeof(*lists));
    }
    (*list_size)++;
    return lists;
}


void remove_newline(char *s){
    int length = strlen(s);
    if(length>0 && s[length-1]=='\n'){
        s[length-1]='\0';
    }
}

void change_lower(char *s ){
    int len = strlen(s);
    for(int i=0 ; i < len ; i++){
        s[i]=tolower(s[i]);
    }
}


int is_number(char *s){
    int len = strlen(s);
    for(int i=1 ; i < len ; i++){
        if(isdigit(s[i])==0) return 0;
    }
    return 1;
}



int main(){
   char * string = NULL;
   size_t count = 0 ;
   int shelf_list = 0 , shopping_list = 0 , string_size =0 , string_capacity = 0;
   size_t shelf_size = 0 , shelf_capacity = 0;
   size_t list_size = 0 , list_capacity = 0;
   shelf * shelves = NULL;
   shopping * lists = NULL;
   size_t str_size = 0;
   int input_no = 1;
   while(getline(&string,&str_size,stdin) != EOF){
        if(input_no==1 && string[0]!='#'){
            printf("Invalid input.\n");
            free(string);
            free(shelves);
            free(lists);
            return EXIT_FAILURE;
        }
        input_no++;
        remove_newline(string);
        if(string[0]=='#'){
            if(is_number(string)==0){
                if(shelf_size!=0){
                        for(unsigned int i=0 ; i< shelf_size ; i++){
                         // printf("shelevs[%u].items:%d\n",i,shelves[i].items);
                            for(int j=0 ; j < shelves[i].items; j++){
                                free(shelves[i].shelf_list[j]);
                            }
                            free(shelves[i].shelf_list);
                        }
                }
                // printf("freed\n");
                if(list_size !=0 ){
                    for(unsigned int i=0 ; i< list_size ; i++){
                        free(lists[i].found);
                        free(lists[i].found_shelf_line);
                        for(int j=0 ; j < lists[i].items ; j++){
                            free(lists[i].shopping_list[j]);
                        }
                        free(lists[i].shopping_list);
                    }
                }
                printf("Invalid input.\n");
                free(string);
                free(shelves);
                free(lists);
                return EXIT_FAILURE;
            }
            size_t num = atoi(&(string[1]));
            // 1
            if(num!=count){
                // printf("shelf_size: %zu",shelf_size);
                if(shelf_size!=0){
                    for(unsigned int i=0 ; i< shelf_size ; i++){
        // printf("shelevs[%u].items:%d\n",i,shelves[i].items);
                        for(int j=0 ; j < shelves[i].items; j++){
                            free(shelves[i].shelf_list[j]);
                         }
                            free(shelves[i].shelf_list);
                    }
                }
                // printf("freed\n");
                if(list_size !=0 ){
                    for(unsigned int i=0 ; i< list_size ; i++){
                        free(lists[i].found);
                        free(lists[i].found_shelf_line);
                        for(int j=0 ; j < lists[i].items ; j++){
                            free(lists[i].shopping_list[j]);
                        }
                        free(lists[i].shopping_list);
                    }
                }
                // printf("freed\n");
                free(string);
                free(lists);
                free(shelves);
                printf("Invalid input.\n");
                return EXIT_FAILURE;
            }
            shelf_list=1;
            shopping_list=0;
            shelves = allocate_shelves(shelves,&shelf_size,&shelf_capacity);
            shelves[shelf_size-1].items = 0;
            count++;
            string_size = 0;
            string_capacity = 0;
            shelves[shelf_size-1].shelf_list = NULL;
            continue;
        }
        if(strlen(string)==0){
            shopping_list = 1;
            shelf_list = 0;
            lists = allocate_lists(lists,&list_size,&list_capacity);
            lists[list_size-1].items=0;
            string_size = 0;
            string_capacity = 0;
            lists[list_size-1].shopping_list = NULL;
            continue;
        }

        

    // printf("string size : %d , string capacity : %d\n",string_size , string_capacity ) ;

    if(shelf_list==1){
        int length = strlen(string);
        if(string_size>=string_capacity){
            if(string_capacity==0) string_capacity=1;
            else string_capacity *=2;
            // printf("string_capacity: %d",string_capacity);
            shelves[shelf_size-1].shelf_list = (char **) 
            realloc(shelves[shelf_size-1].shelf_list,string_capacity*sizeof(char *));
        }
        shelves[shelf_size-1].shelf_list[string_size] =(char *) malloc ((length+1) * sizeof(char));
        strcpy(shelves[shelf_size-1].shelf_list[string_size],string);
        string_size++;
        shelves[shelf_size-1].items=string_size;
    }

    if(shopping_list==1){
        int length = strlen(string);
        if(string_size>=string_capacity){
            if(string_capacity==0) string_capacity=1;
            else string_capacity *=2;
            // printf("string_capacity: %d",string_capacity);
            lists[list_size-1].shopping_list = (char **) 
            realloc(lists[list_size-1].shopping_list,string_capacity*sizeof(char *));
        }
        lists[list_size-1].shopping_list[string_size] =(char *) malloc ((length+1) * sizeof(char));
        strcpy(lists[list_size-1].shopping_list[string_size],string);
        string_size++;
        lists[list_size-1].items=string_size;
    }
    // free(string);
    // string = NULL;
    }



    if(shelf_size==0 || list_size==0){
        if(shelf_size!=0){
            for(unsigned int i=0 ; i< shelf_size ; i++){
            // printf("shelevs[%u].items:%d\n",i,shelves[i].items);
                for(int j=0 ; j < shelves[i].items; j++){
                    free(shelves[i].shelf_list[j]);
                }
            free(shelves[i].shelf_list);
            }
        }
        printf("Invalid input.\n");
        free(shelves);
        free(lists);
        free(string);
        return EXIT_FAILURE;
    }
    
    free(string);

    for(unsigned int i=0 ; i < list_size ; i++){
        lists[i].found= (int *) malloc(lists[i].items*sizeof(int));
        for(int p=0 ; p < lists[i].items ; p++){
            lists[i].found[p]=-1;
        }
        lists[i].found_shelf_line = (int *) malloc(lists[i].items*sizeof(int));
        for(unsigned int k=0 ; k<shelf_size; k++){
            for(int j=0 ; j < lists[i].items ; j++){
                    for(int l=0 ; l < shelves[k].items ; l++){
                        char * test1;
                        test1 = (char *) malloc(1+strlen(lists[i].shopping_list[j]));
                        char * test2;
                        test2 = (char *) malloc(1+strlen(shelves[k].shelf_list[l]));
                        strcpy(test1,lists[i].shopping_list[j]);
                        strcpy(test2,shelves[k].shelf_list[l]);
                        change_lower(test1);
                        change_lower(test2);
                        if(strcmp(test1,test2)==0){
                            lists[i].found[j]=k;
                            lists[i].found_shelf_line[j]=l;
                        }
                        else if(strcasestr(test2,test1)!=NULL && lists[i].found[j]==-1){
                            lists[i].found[j]=k;
                            lists[i].found_shelf_line[j]=l;
                        }
                        free(test1);
                        free(test2);
                    }
            }
        }
    }

    for(unsigned int i=0 ; i<list_size ; i++){
        printf("Optimized list:\n");
        int p = 0;
        for(unsigned int k=0; k<shelf_size ; k++ ){
            for(int j=0 ; j<lists[i].items ; j++){
                if(lists[i].found[j]==(int)k){
                    printf(" %d. %s -> #%d %s\n",p,lists[i].shopping_list[j],k,shelves[k].shelf_list[lists[i].found_shelf_line[j]]);
                    p++;
                }
            }
        }
        for(int j=0 ; j<lists[i].items;j++){
            if(lists[i].found[j]==-1){
                printf(" %d. %s -> N/A\n",p,lists[i].shopping_list[j]);
                p++;
            }
        }
    }


    for(unsigned int i=0 ; i< shelf_size ; i++){
        // printf("shelevs[%u].items:%d\n",i,shelves[i].items);
        for(int j=0 ; j < shelves[i].items; j++){
            free(shelves[i].shelf_list[j]);
        }
        free(shelves[i].shelf_list);
    }
    for(unsigned int i=0 ; i< list_size ; i++){
        free(lists[i].found);
        free(lists[i].found_shelf_line);
        for(int j=0 ; j < lists[i].items ; j++){
            free(lists[i].shopping_list[j]);
        }
        free(lists[i].shopping_list);
    }
    free(shelves);
    free(lists);
    
    return 0;
}





