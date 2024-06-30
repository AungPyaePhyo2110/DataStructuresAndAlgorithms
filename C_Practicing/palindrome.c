#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <assert.h>
#endif /* __PROGTEST__ */


unsigned long long isPalindrome(int data[], unsigned long long from,int radix){
  int n=0;
  unsigned long long number = from;
  while(number!=0){
    data[n]=number%radix;
    number/=radix;
    ++n;
  }
  for(int i = n-1 , j = 0 ; j<=n/2 ; --i,++j){
    if(data[i]!=data[j]) return 0;
  }
  return from;
}

int nextPalindrome ( unsigned long long from, 
                     int radix,
                     unsigned long long * next )
{
   unsigned long long result=0 , from2 ;
    if(radix <2 || radix > 36 || from > ULLONG_MAX-1) return 0;
    int data[100];
    result = isPalindrome(data,from,radix);
    * next = result;
    result=0;
    from2=from+1;
    while ((result==0) && (from!=ULLONG_MAX)){
      result = isPalindrome(data,from2,radix);
      ++from2;
    }
    if(result!=0) {
      * next = result;
      return 1;
    }

    return 0;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  unsigned long long next;

  assert ( nextPalindrome ( 123, 10, &next ) == 1
           && next == 131 );
  assert ( nextPalindrome ( 188, 10, &next ) == 1
           && next == 191 );
  assert ( nextPalindrome ( 1441, 10, &next ) == 1
           && next == 1551 );
  assert ( nextPalindrome ( 95, 15, &next ) == 1
           && next == 96 ); /* 96 = 66 (radix 15) */
  assert ( nextPalindrome ( 45395, 36, &next ) == 1
           && next == 45431 ); /* 45431 = z1z (radix 36) */
  assert ( nextPalindrome ( 1027, 2, &next ) == 1
           && next == 1057 ); /* 1057 = 10000100001 (radix 2) */
  assert ( nextPalindrome ( 1000, 100, &next ) == 0
           && next == 1057 );
  assert ( nextPalindrome ( 18446744073709551614LLu, 2, &next ) == 1
           && next == 18446744073709551615LLu ); /* 18446744073709551615 = 1111111111111111111111111111111111111111111111111111111111111111 (radix 2) */
  assert ( nextPalindrome ( 18446744073709551615LLu, 2, &next ) == 0
           && next == 18446744073709551615LLu );
  return 0;
}
#endif /* __PROGTEST__ */
