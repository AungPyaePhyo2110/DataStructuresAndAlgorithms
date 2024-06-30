#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MAX_VALUE 5000001
int data[MAX_VALUE];
int main(){
    for(int i=0 ; i<MAX_VALUE-1 ; ++i ){
        data[i]=0;
    }

    char c;
    int a,b;
    printf("Intervals:\n");
    while(scanf(" %c %d %d",&c, &a , &b)==3){
        int total=0;
        if((c!='?'&& c!='#') || a>b || a<=0 || b<=0){
            printf("Invalid input.\n");
            return EXIT_FAILURE;
        }

        for(int i=a;i<=b;++i){
            int sum=1,d=0;
            int n=i;

            if(data[i]==1){
                ++total;
                if(c=='?'){
                    printf("%d\n",i);
                }
            }
            else{
            for(int j=2;j<=sqrt(n);++j){
                int current_power=1,current_term=1;
                while(n%j==0 && i!=j){
                    n/=j;
                    current_power*=j;
                    current_term+=current_power;
                    
                }
                sum*=current_term;
            }
            
            if(n>=2) sum*=(1+n);
            sum=sum-i;
            if(sum<2 || sum%2==0) d=1;
            else{
                int max = (int)sqrt(sum);
                for(int k=3;k<=max;k+=2){
                    if(sum%k==0){
                        d=1;
                        break;
                    }
                }
            }
            if(d==0){
                data[i]=1;
                ++total;
                if(c=='?'){
                    printf("%d\n",i);
                }
            }
            }

            
        }
        printf("Total: %d\n",total);

    }

    if ( ! feof ( stdin ) ) {
        printf ( "Invalid input.\n" );
        return EXIT_FAILURE;
    }



        
        


}




