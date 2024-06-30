#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(){
	int a,x1,y1,z1,x2,y2,z2,x3,z3,piping_length;
	double hose;

	printf("Room dimension:\n");
	if (scanf("%d",&a)!=1||a<=0){
		printf("Invalid input.\n");
		return EXIT_FAILURE;
	}
	printf("Point #1:\n");
	if (scanf("%d %d %d",&x1,&y1,&z1)!=3||x1<0||x1>a||y1<0||y1>a||
		((x1<20||x1>a-20)&&(y1<20||y1>a-20))||z1<20||z1>a-20||(x1!=0&&x1!=a&&y1!=0&&y1!=a)){
		printf("Invalid input.\n");
		return EXIT_FAILURE;
	}

	printf("Point #2:\n");
	if (scanf("%d %d %d",&x2,&y2,&z2)!=3||x2<0||x2>a||y2<0||y2>a||
		((x2<20||x2>a-20)&&(y2<20||y2>a-20))||z2<20||z2>a-20||(x2!=0&&x2!=a&&y2!=0&&y2!=a)){
		printf("Invalid input.\n");
		return EXIT_FAILURE;
	}
	z3=abs(z1-z2);
	if(x1!=a&&y1==a){x1=-x1;y1=-y1;}
	if(x1==0&&y1!=a){x1=-x1;y1=-y1;}
	if(x2!=a&&y2==a){x2=-x2;y2=-y2;}
	if(x2==0&&y2!=a){x2=-x2;y2=-y2;}
	x3=abs(x1-x2)+abs(y1-y2);
	if(x3>2*a){
		x3=4*a-(abs(x1-x2)+abs(y1-y2));
	}
	piping_length=x3+z3;
	hose=sqrt((x3*x3)+(z3*z3));
	printf("Piping length: %d\n",piping_length);
	printf("Hose length: %lf\n",hose);


	

}


