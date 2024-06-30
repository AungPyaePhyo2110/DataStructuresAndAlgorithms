#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct TItem
{
  struct TItem             * m_Next;
  char                     * m_Name;
  char                       m_Secret[24];
} TITEM;

#endif /* __PROGTEST__ */

TITEM            * newItem      ( const char      * name,
                                  TITEM           * next )
{
  TITEM * new_node = (TITEM *) malloc(sizeof(TITEM));
  size_t capacity = strlen(name);
  new_node->m_Name = (char *) malloc(capacity+1);
  strcpy(new_node->m_Name,name);
  if(next == NULL){
    new_node->m_Next = NULL;
    next = new_node;
    return next; 
  }
  new_node->m_Next=next;
  next=new_node;
  return next;
}



TITEM * divider (TITEM * l){
    TITEM * hare = l->m_Next;
    TITEM * tortoise = l;
    while(hare->m_Next){
        hare=hare->m_Next;
        if(hare->m_Next){
            hare=hare->m_Next;
            tortoise = tortoise->m_Next;
        }
    }
    return tortoise;
}

TITEM * merge(TITEM * a, TITEM *b, int ascending){
    TITEM * sorted = NULL;
    TITEM * head = NULL;
    if(ascending!=0){
        if(strcmp(a->m_Name,b->m_Name)<=0){
            sorted = a;
            a = a->m_Next;
        }
        else{
            sorted = b;
            b=b->m_Next;
        }
        
        head = sorted;

        while(a!=NULL && b!= NULL){
            if(strcmp(a->m_Name,b->m_Name)<=0){
            sorted->m_Next= a;
            a = a->m_Next;
            sorted = sorted->m_Next;
        }
        else{
            sorted->m_Next = b;
            b=b->m_Next;
            sorted = sorted->m_Next;
        }
        }
        if(a!=NULL){
            sorted->m_Next = a;
        }
        if(b!=NULL){
            sorted->m_Next = b;
        }
    }
    else{
        if(strcmp(a->m_Name,b->m_Name)>=0){
        sorted = a;
        a = a->m_Next;
        }
        else{
            sorted = b;
            b=b->m_Next;
        }
        
        head = sorted;

        while(a!=NULL && b!= NULL){
            if(strcmp(a->m_Name,b->m_Name)>=0){
            sorted->m_Next= a;
            a = a->m_Next;
            sorted = sorted->m_Next;
        }
        else{
            sorted->m_Next = b;
            b=b->m_Next;
            sorted = sorted->m_Next;
        }
        }
        if(a!=NULL){
            sorted->m_Next = a;
        }
        if(b!=NULL){
            sorted->m_Next = b;
        }
    }
    return head;
}

TITEM * recursion(TITEM *l , int ascending ){
    if(l==NULL){
        return NULL;
    }
    if(l->m_Next==NULL){
        return l;
    }
    TITEM * mid = l;
    mid = divider(l);
    TITEM * temp = mid;
    mid = mid->m_Next;
    temp->m_Next = NULL;
    TITEM * l1 =recursion(l,ascending);
    TITEM * l2 = recursion(mid,ascending);
    l = merge(l1,l2,ascending);
    return l;
}

TITEM            * sortList     ( TITEM           * l,
                                  int               ascending )
{ 
  if(l==NULL){
    return l;
  }
  if(l->m_Next==NULL){
    return l;
  }
  
  l = recursion(l , ascending);

  return l;
}
void               freeList     ( TITEM           * src )
{ 
  if(src == NULL){
    return;
  }
  if(src->m_Next == NULL){
    free(src->m_Name);
    free(src);
    src = NULL;
    return;
  }
  TITEM * current = src;
  while(current){
    TITEM * to_remove =current;
    free(current->m_Name);
    current = current->m_Next;
    free(to_remove);
  }
  src = NULL;
}


void print_list (TITEM * l){
  if(l==NULL){
    return;
  }
  TITEM * current = l;
  while(current){
    printf("%s -> ",current->m_Name);
    current=current->m_Next;
  }
  printf("\n");
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  TITEM * l;
  char tmp[50];

  assert ( sizeof ( TITEM ) == sizeof ( TITEM * ) + sizeof ( char * ) + 24 * sizeof ( char ) );
  l = NULL;
  l = newItem ( "PA1", l );
  l = newItem ( "PA2", l );
  l = newItem ( "UOS", l );
  l = newItem ( "LIN", l );
  l = newItem ( "AG1", l );
  print_list(l);
  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 1 );
  print_list(l);

  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = newItem ( "programming and algorithmic I", l );
  l = newItem ( "AAG", l );
  print_list(l);
  assert ( l
           && ! strcmp ( l -> m_Name, "AAG" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 0 );
  print_list(l);
  assert ( l
           && ! strcmp ( l -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AAG" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeList ( l );
  l = NULL;
  strncpy ( tmp, "PA1", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "PA2", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "UOS", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "LIN", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "AG1", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  print_list(l);
  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 1 );
  print_list(l);
  assert ( l
           && ! strcmp ( l -> m_Name, "AG1" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  strncpy ( tmp, "programming and algorithmic I", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  strncpy ( tmp, "AAG", sizeof ( tmp ) - 1 );
  tmp[sizeof ( tmp ) - 1 ] = '\0';
  l = newItem ( tmp, l );
  print_list(l);
  assert ( l
           && ! strcmp ( l -> m_Name, "AAG" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  l = sortList ( l, 0 );
  print_list(l);
  assert ( l
           && ! strcmp ( l -> m_Name, "programming and algorithmic I" ) );
  assert ( l -> m_Next
           && ! strcmp ( l -> m_Next -> m_Name, "UOS" ) );
  assert ( l -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Name, "PA2" ) );
  assert ( l -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Name, "PA1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "LIN" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AG1" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next
           && ! strcmp ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Name, "AAG" ) );
  assert ( l -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  freeList ( l );
  l=NULL;
//   // printf("%s",l->m_Name);
//   // assert(l==NULL);
  print_list(l);
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
