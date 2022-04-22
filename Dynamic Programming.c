#include<stdio.h>
#include<math.h> 
#include<stdlib.h>

typedef struct {    // reklam bilgilerinin tutuldu�u struct yap�s�
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
	int kacAdet; //ka� adet reklam oldu�unun tutuldu�u de�i�ken
	int i,j,secim,index;
	int *profit; //profit bilgileri farkl� bir dizide tutuluyor bu diziyi g�steren pointer
	
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
	if(secim == 2){	//input y�ntemine g�re inputlar al�n�r.
		for(i=1;i<kacAdet+1;i++){
			printf("%d inci reklam icin sirasiyla StartTime, Duration, Value degerlerini giriniz: ",i);
			scanf("%d %d %d", &Reklam[i].StartTime, &Reklam[i].Duration, &Reklam[i].Value);
			Reklam[i].FinishTime = Reklam[i].StartTime + Reklam[i].Duration;//finish time hesaplan�r
			Reklam[i].ReklamNo  = index;
			index++;
		}
	}else{
		for(i=1;i<kacAdet+1;i++){
			fscanf(fp,"%d %d %d", &Reklam[i].StartTime, &Reklam[i].Duration, &Reklam[i].Value);
			Reklam[i].FinishTime = Reklam[i].StartTime + Reklam[i].Duration;//finish time hesaplan�r
			Reklam[i].ReklamNo  = index;
			index++;
		}
	}
	printf("\n");
	printf("Reklamlar: \n\n");
	printf(" Reklam No |StartTime|Duration|Value|FinishTime|Profit\n");
	for(i=1;i<kacAdet+1;i++){//reklamlar ve profitleri yazd�r�l�r.
		printf("  %d        |    %d,   |    %d,  | %d,   |    %d   |   %d   \n",Reklam[i].ReklamNo,Reklam[i].StartTime,Reklam[i].Duration, Reklam[i].Value, Reklam[i].FinishTime,profit[i]);
	}
	bubbleSort(kacAdet+1, Reklam);//reklam listesi finish time'a g�re s�ralan�r.
	
	profit[0] = 0;//profit[0] = 0 a e�itlenir
	profit[1] = Reklam[1].Value;//profit[1] ilk eleman�n value's�ne e�it olaca�� i�in direkt olarak atan�r.
	for (i=2;i<kacAdet+1;i++){//reklamlar listesinde 2. elemandan ba�lanarak profitler hesaplan�r.
		j=i-2;
		if(Reklam[i-1].FinishTime>Reklam[i].StartTime){//e�er reklam listesinde s�rada olan reklam bir �ncekiyle �ak���yorsa
			while(j!=0 && Reklam[j].FinishTime>Reklam[i].StartTime){//bir �nceki �ak��mayan reklam bulunana kadar geriye gider.
				j--;
			}
			if(j==0){//e�er �ak��mayan bulamazsa
				profit[i] = maximum(Reklam[i].Value,profit[i-1]);//�nceki ad�m�n profiti ve bu ad�m�n value's�nden hangisi y�ksekse onu profit olarak al�r.	
			} 
			else {
				profit[i] = maximum(profit[j] + Reklam[i].Value,profit[i-1]);//e�er �ak��mayan �nceki varsa �nceki ad�m�n profiti ile �nceki �ak��mayan reklam ad�m�n�n profiti+bu ad�m�n value's� kar��la�t�r�l�r.
			}
		}else{
			profit[i]= profit[i-1]+Reklam[i].Value;//reklam listesinde bu ad�mdaki reklam bir �nceki reklam ile �ak��m�yorsa
		}
	}
	
	printf("\n");
	printf("Siralanmis Reklamlar: \n\n");
	printf(" Reklam No |StartTime|Duration|Value|FinishTime|Profit\n");
	for(i=1;i<kacAdet+1;i++){//reklamlar ve profitleri yazd�r�l�r.
		printf("  %d        |    %d,   |    %d,  | %d,   |    %d   |   %d   \n",Reklam[i].ReklamNo,Reklam[i].StartTime,Reklam[i].Duration, Reklam[i].Value, Reklam[i].FinishTime,profit[i]);
	}
	
	i=kacAdet;
	while(profit[i] == profit[i-1]){// en son de�i�im olan reklam ad�m� bulunur.
		i--;
	}
	
	j=i-1;
	printf("\n Yayinlanacak reklamlar(Reklamlar Siralanmadan Onceki Numaralari): \n %d, ",Reklam[i].ReklamNo);
	while(j>0){	//en son de�i�im olan reklamdan bir �ncekinden ba�lanarak son �ak��mayan reklam bulunur, geriye do�ru liste bitene kadar �ak��mayan reklamlar bulunur, b�ylelikle hangi reklamlar�n se�ildi�i belirlenir.
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

int maximum(int a, int b){//iki de�erden b�y�k olan� d�nd�ren fonksiyon
	if(a>b) return a;
	else return b;
}

void bubbleSort(int adet,reklam Reklam[adet]){//reklamlar� finish time'lar�na g�re k���kten b�y�ye s�ralayan fonksiyon.
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
