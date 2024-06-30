#include<stdio.h>
#include<stdlib.h>
#include<math.h>


double roundingthevalue(double a){
	int b= a * 100;
	double c = double(b)/100;
	return c;
}

int main(){
	double credit_interest,debit_interest;
	int day[10000],amount[10000],n=1,previous_day=-1;
	double balance=0.000000;
	char comma;
	printf("Enter credit interest rate %s:\n","[%]");
	if(scanf(" %lf",&credit_interest) !=1 || credit_interest <= 0){
		printf("Invalid input.\n");
		return EXIT_FAILURE;
	}
	printf("Enter debet interest rate %s:\n","[%]");
	if(scanf(" %lf",&debit_interest) !=1 || debit_interest <= 0){
		printf("Invalid input.\n");
		return EXIT_FAILURE;

	}
	printf("Enter transactions:\n");
	for(int i=1;i<=n;i++){
		if(scanf(" %d %c %d",&day[i],&comma,&amount[i])!=3 || day[i] <0 || day[i]<=previous_day|| comma !=','){
			printf("Invalid input.\n");
			return EXIT_FAILURE;
		}
		if(i==1)
		balance=balance+amount[i];
		if(i>=2){
			int circles=day[i]-day[i-1];
			for(int j=1;j<=circles;j++){
				if (balance>=0){
				balance=balance+(credit_interest*0.01*balance);
				balance=roundingthevalue(balance);
				}
				if(balance<0){
				balance=balance+(debit_interest*0.01*balance);
				balance=roundingthevalue(balance);
				}
		}
		balance=balance+amount[i];
		}
		if(amount[i]==0){
			break;
			return EXIT_FAILURE;

		}
		else{
			previous_day=day[i];
			n++;
		}

	}
	printf("Balance: %.2f\n",balance);




}

