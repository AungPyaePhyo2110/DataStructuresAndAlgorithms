#include<stdio.h>
#include<stdlib.h>

int main(void){
	int a;
	printf("ml' nob:\n");
	if (scanf("%d",&a) !=1 || a<1 || a>9){
		printf("luj\n");
		return EXIT_FAILURE;
	}

	if (a==1){
		printf("Qapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n");
	}
	else if (a==2){
		printf("Qapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n");
	}
	else if (a==3){
		printf("Qapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n");
	}
	else if (a==4){
		printf("Qapla'\nbISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n");
	}
	else if (a==5){
		printf("Qapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
	}
	else if (a==6){
		printf("Qapla'\nSuvlu'taHvIS yapbe' HoS neH.\n");
	}
	else if (a==7){
		printf("Qapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
	}
	else if (a==8){
		printf("Qapla'\nHeghlu'meH QaQ jajvam.\n");
	}
	else{
		printf("Qapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
	}

}



