#include<stdio.h>
#include<math.h> 
#include<stdlib.h>

typedef struct {    // reklam bilgilerinin tutulduðu struct yapýsý
	int StartTime;
	int Duration;
	int Value;
	int FinishTime;	
	int ReklamNo;
}reklam;

int maximum(int a, int b);
void bubbleSort(int adet,reklam Reklam[adet]);

int main(){
	FILE *fp;
	fp = fopen("sample.txt","r");
	int kacAdet; //kaç adet reklam olduðunun tutulduðu deðiþken
	int i,j,secim,index;
	int *profit; //profit bilgileri farklý bir dizide tutuluyor bu diziyi gösteren pointer
	
	printf("Program icin girdiyi dosyadan okumak istiyorsaniz 1, manuel olarak girmek istiyorsaniz 2'yi seciniz: ");
	scanf("%d",&secim);
	
	printf("Kac adet reklam veren var?: ");
	scanf("%d",&kacAdet);
	
	profit = (int*)calloc(kacAdet+1, sizeof(int));
	if (profit == NULL) {
        printf("Memory not allocated.\n");
        exit(0);
    }
    
	reklam *Reklam = malloc((sizeof(reklam) * kacAdet));
	index = 1;
	if(secim == 2){	//input yöntemine göre inputlar alýnýr.
		for(i=1;i<kacAdet+1;i++){
			printf("%d inci reklam icin sirasiyla StartTime, Duration, Value degerlerini giriniz: ",i);
			scanf("%d %d %d", &Reklam[i].StartTime, &Reklam[i].Duration, &Reklam[i].Value);
			Reklam[i].FinishTime = Reklam[i].StartTime + Reklam[i].Duration;//finish time hesaplanýr
			Reklam[i].ReklamNo  = index;
			index++;
		}
	}else{
		for(i=1;i<kacAdet+1;i++){
			fscanf(fp,"%d %d %d", &Reklam[i].StartTime, &Reklam[i].Duration, &Reklam[i].Value);
			Reklam[i].FinishTime = Reklam[i].StartTime + Reklam[i].Duration;//finish time hesaplanýr
			Reklam[i].ReklamNo  = index;
			index++;
		}
	}
	printf("\n");
	printf("Reklamlar: \n\n");
	printf(" Reklam No |StartTime|Duration|Value|FinishTime|Profit\n");
	for(i=1;i<kacAdet+1;i++){//reklamlar ve profitleri yazdýrýlýr.
		printf("  %d        |    %d,   |    %d,  | %d,   |    %d   |   %d   \n",Reklam[i].ReklamNo,Reklam[i].StartTime,Reklam[i].Duration, Reklam[i].Value, Reklam[i].FinishTime,profit[i]);
	}
	bubbleSort(kacAdet+1, Reklam);//reklam listesi finish time'a göre sýralanýr.
	
	profit[0] = 0;//profit[0] = 0 a eþitlenir
	profit[1] = Reklam[1].Value;//profit[1] ilk elemanýn value'süne eþit olacaðý için direkt olarak atanýr.
	for (i=2;i<kacAdet+1;i++){//reklamlar listesinde 2. elemandan baþlanarak profitler hesaplanýr.
		j=i-2;
		if(Reklam[i-1].FinishTime>Reklam[i].StartTime){//eðer reklam listesinde sýrada olan reklam bir öncekiyle çakýþýyorsa
			while(j!=0 && Reklam[j].FinishTime>Reklam[i].StartTime){//bir önceki çakýþmayan reklam bulunana kadar geriye gider.
				j--;
			}
			if(j==0){//eðer çakýþmayan bulamazsa
				profit[i] = maximum(Reklam[i].Value,profit[i-1]);//önceki adýmýn profiti ve bu adýmýn value'sünden hangisi yüksekse onu profit olarak alýr.	
			} 
			else {
				profit[i] = maximum(profit[j] + Reklam[i].Value,profit[i-1]);//eðer çakýþmayan önceki varsa önceki adýmýn profiti ile önceki çakýþmayan reklam adýmýnýn profiti+bu adýmýn value'sü karþýlaþtýrýlýr.
			}
		}else{
			profit[i]= profit[i-1]+Reklam[i].Value;//reklam listesinde bu adýmdaki reklam bir önceki reklam ile çakýþmýyorsa
		}
	}
	
	printf("\n");
	printf("Siralanmis Reklamlar: \n\n");
	printf(" Reklam No |StartTime|Duration|Value|FinishTime|Profit\n");
	for(i=1;i<kacAdet+1;i++){//reklamlar ve profitleri yazdýrýlýr.
		printf("  %d        |    %d,   |    %d,  | %d,   |    %d   |   %d   \n",Reklam[i].ReklamNo,Reklam[i].StartTime,Reklam[i].Duration, Reklam[i].Value, Reklam[i].FinishTime,profit[i]);
	}
	
	i=kacAdet;
	while(profit[i] == profit[i-1]){// en son deðiþim olan reklam adýmý bulunur.
		i--;
	}
	
	j=i-1;
	printf("\n Yayinlanacak reklamlar(Reklamlar Siralanmadan Onceki Numaralari): \n %d, ",Reklam[i].ReklamNo);
	while(j>0){	//en son deðiþim olan reklamdan bir öncekinden baþlanarak son çakýþmayan reklam bulunur, geriye doðru liste bitene kadar çakýþmayan reklamlar bulunur, böylelikle hangi reklamlarýn seçildiði belirlenir.
		while(Reklam[i].StartTime<Reklam[j].FinishTime && j>0){
			j--;
		}
		printf("%d, ",Reklam[j].ReklamNo);
		i=j;
		j--;
	}
	
	printf("\n Toplam Kazanc: %d",profit[kacAdet]);
	
	free(Reklam);
	fclose(fp);
	free(profit);
	return 0;
}

int maximum(int a, int b){//iki deðerden büyük olaný döndüren fonksiyon
	if(a>b) return a;
	else return b;
}

void bubbleSort(int adet,reklam Reklam[adet]){//reklamlarý finish time'larýna göre küçükten büyüye sýralayan fonksiyon.
	int i,j;
	reklam tmp;
	for(i=1;i<adet;i++){
		for(j=1;j<adet-i;j++){
			if(Reklam[j].FinishTime>Reklam[j+1].FinishTime){
				tmp=Reklam[j];
				Reklam[j] = Reklam[j+1];
				Reklam[j+1] = tmp;
			}
		}
	}
}
