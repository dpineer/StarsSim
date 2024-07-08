#include "graphics.h" //��Ҫ�������ͼ�ο�  D:\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include
#include "stdio.h"
//#include <iostream>
#include "gdiplus.h"
#include "conio.h"
#include "time.h"
#include <string>
#include <algorithm> //C++17 �����DevCpp�����������ں˰汾 
#include "math.h" 
#include "GraPGameLib-Y/GPGL.h" /////�����غ����������� 

using namespace Gdiplus;
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
//ʹ��ͼ�λ�ѡ���Խ���CMD���� _������VSC  ����ѧϰ������������ĩ 

#define Left_Click 0
#define Right_Click 1

#define GH2 1.4142135623731


bool Develop = 0;      /////////////////////////////////������Ϣ//////////////////////////////

struct PhysicSX               //�������� 
{
	unsigned int IdetifyNum;  //ͬһ������ʹ��ͬһ��ID�롪��AUΪ��λʱ�ĵ�ģ�� 
	unsigned int ton;		  //��λ���� 
	int hard;				  //��λ��Ӳ��
	int Completeness;         //�����ȣ�Ѫ����--Ҫ��ͼƬ�� ��ͬʱҲ������Ϊ�׿�����Դָʾ�� 
    float Heat;				  //�����¶� ��λ ������ 
};

struct Pytloc
{
	int IdentifyNum;
	double lx,ly,lz;				//�����ṹ�塪�����ڸ���һ������״̬�����������ϵ 
	double vx,vy,vz;				//�ٶ�
	double VmixXY,VmixXZ;
};

struct SolarsObjectRail     ////RKKV�͹�׶ 
{
	unsigned int From[20],To[20];////��Ӧ����ϵ��� 
	float vpc[20];			////��Թ��� 
	unsigned int type[20];	///���� 
	float Per[20];			///�ִ���� 
};

struct ColorInt
{
	int Red,Green,Blue;	
};

struct ColorInt ALS = {////////////////////////////����һ��ȫ�ֽṹ�����ڴ���RGB����//////////////////////////////// 
	0,0,0
};

struct Pytloc PLD = {/// 
	0,0.0,0.0,0.0,0.0,0.0,0.0
};

void BlackBody_Translate_RGB(float kelvin) ///����תRGB -ʹ����ȫ�ֱ��� 
{
	float red;
	float green;
	float blue;
	int RI,GI,BI;
	float x,t;
	
	t = kelvin/100.0;
	
	if (t <= 66)
	{
    	red = 1.0;
	}
	else
	{
    	x = t - 55.0;
    	red = clamp(
        	1.38030159086 + 0.00044786845 * x - 0.15785750233 * log(x),
        	0.0,
        	1.0);
	}
	if (t <= 10)
	{
    	green = 0.0;
	}
	else if (t <= 66)
	{
    	x = t - 2.0;
    	green = clamp(
        	-0.60884257109 - 0.00174890002 * x + 0.40977318429 * log(x),
        	0.0,
        	1.0);
	}
	else
	{
    	x = t - 50.0;
    	green = clamp(
        	1.27627220616 + 0.0003115081 * x - 0.11013841706 * log(x),
        	0.0,
        	1.0);
	}
	if (t <= 20)
	{
    	blue = 0.0;
	}
	else if (t <= 66)
	{
    	x = t - 10.0;
    	blue = clamp(
        	-0.99909549742 + 0.00324474355 * x + 0.45364683926 * log(x),
        	0.0,
        	1.0);
	}
	else
	{
    	blue = 1.0;
	}
///�¶�תRGB���ԣ� https://computergraphics.stackexchange.com/questions/9526/blackbody-curve-to-floating-rgb 
	RI = int(red*255.0f);
	GI = int(green*255.0f);
	BI = int(blue*255.0f);
	
	ALS.Red = RI;
	ALS.Blue = BI;
	ALS.Green = GI;//ע������ 
	//return red,green,blue;
}



int GSCalluate(int RDY,int GLP) ///������դ��������˹���� 
{
	int PSAs,i;
	PSAs = 0;
	i = 1;
	while(i)
	{
		PSAs ++;
		if(RDY <= PSAs*GLP)
		{
			i = 0;
		}
	}
	return PSAs;
 } 

void PrtCal(double Xq,double Yq,double Zq,int IdentifyNum,double BS) //�ӽ�ת������ -ʹ����ȫ�ֱ��� 
{
	 PLD.IdentifyNum = IdentifyNum;
	 PLD.lx = Xq*-BS;
	 PLD.ly = Yq*BS;
	 PLD.lz = Zq*-BS;
	 
	 PLD.ly = PLD.ly / GH2;
	 PLD.lx += PLD.ly;
	 
	 PLD.ly += PLD.lz; 
	 
	 PLD.lx += 500.0;
	 PLD.ly += 400.0;
}

void FillCircle(float x,float y,float R,double BS) ///���һ��������Ч����Բ�Σ���Ϊ�Դ�����Ҫ�ƶ���ɫ��Ϊ�߿�  
{
	bool KSA;
	float FSS;
	unsigned int RdS,GdS,BdS;
	float Count;
	RdS = ALS.Red;
	GdS = ALS.Green;
	BdS = ALS.Blue;
	Count = 14;
	FSS = R;
	KSA = 1;
	setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
	while(KSA)
	{
		Count -= 1;
		RdS -= 7;
		GdS -= 7;
		BdS -= 7;
		setcolor(EGERGB(RdS,GdS,BdS));
		line((x-R-Count),y,(x+R+Count),y);
		line(x,(y-R-Count),x,(y+R+Count));
		if(Count <= 3)
		{
			KSA = 0;
		}
	}
	KSA = 1;
	setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
	while(KSA)
	{ 
		circle(x,y,R); 
		R -= 0.7;
		if(R <= 8.0)
		{
			R -= 0.3;
		}
		if(R <= 3.0)
		{
			KSA = 0; 
		}
	 } 
	 KSA = 1;
	 R = FSS;
	 FSS = FSS*BS;
	 
	 ALS.Red -= 55;
	 ALS.Green -= 55;
	 ALS.Blue -= 55;
	 
	 while(KSA)
	 {	
	 ALS.Red -= 9;
	 ALS.Green -= 9;
	 ALS.Blue -= 9;
	 if(ALS.Blue <= 3)
	 {
	 	ALS.Blue = 0;
	 	KSA = 0;
	 }
	 if(ALS.Red <= 3)
	 {
	 	ALS.Red = 0;
	 	KSA = 0;
	 }
	 if(ALS.Green <= 3)
	 {
	 	ALS.Green = 0;
	 	KSA = 0;
	 }
	setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
		R += 0.4;
		circle(x,y,R); 
		if(R >= FSS)
		{
			KSA = 0;
		}
	}
	setcolor(LIGHTGRAY);
}

void DrawTmpRGB(int x,int y,float LwK,float HwK,float CZX) ///���Դ��룬��һ���¶��� 
{
	int count,Tcount;
	float Ntk;
	bool ESOX;
	ESOX = 1;
	count = 0;
	Ntk = LwK;
	Tcount = 0;//CZX:��ֵ����ϸ�ȣ�Խ��Խ��ϸ 
	while(ESOX)
	{
		BlackBody_Translate_RGB(Ntk);
		setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
		line(x+count,y,x+count,y+10);
		count += 1;
		if(count % 10 == 0)
		{
			Tcount += 1; 
			line(x+count,y,x+count,y+100);
		}
		Ntk += CZX;
		if(Ntk >= HwK)
		{
			ESOX = 0; 
		}
		
	}
 } 
 
int main(int argc, char* argv[])//int argc, char* argv[]) //����11��׼��Ҫ��ں��� 
{ 
/******************************DEFINE START*******************************/ 
	char WelCome[] = "Loading";
	key_msg k = {0};
	MOUSEMSG m = {0};
	bool HWtestmode = 0;
	char Version[] = "0.0.2.dev";
	char Mode[] = "��ȴ������ӡ��� "; 
    char Title[30];
    char DevLog[20];
	struct ColorInt CIKS = {
		0,0,0
	};
	char filename[] = "So.csv";
    char table[20][15][30];//����д�ӡ�������ע��ڶ���ֵ�����csv�ļ��ϸ��Ӧ�ĺ����귶Χ�� 
    char KKR[200][6][5];//��ע��ڶ���ֵ�����csv�ļ��ϸ��Ӧ�ĺ����귶Χ�� 
    char NewKkePrj[10][4][5];
    
    unsigned int SCount = 0;
	bool PageChanged = 0;
	float PXD = 100.0;
	unsigned int NumofSolor = 0;
	unsigned int NumofTag = 0;
	unsigned int OrbitSN = 1;
	unsigned int Page = 1;
	char s[20];
	char n[20];
	char h[5];
	char buf[255];
	float StarsTableX[20];
	float StarsTableY[20];
	float StarsTableR[20];
	float CaluateTranslateStarsXYR = 0.0;
	unsigned int Years;
	clock_t STP; 
	bool ClockMode; 
	unsigned int PCount;
	unsigned int FTCount; 
	unsigned int ArvMode;
	int retLen;
	double CaluateSignOfSP1;
	double CaluateSignOfSP2;
	double CaluateSignOfSP3;
	
/*******************************SOLAR Def*************************/
	struct StarsSelf Sun = {
		//1,0.0,0.0,0.0,1.0,0.0,1.0,1.0,0.0,0.0	//ͨ����ȣ��뾶�������Ч�¶�Teff  ���ǲ��Է��� 
	};
	
	struct StarsSelf Stars = {//���� 
	//	1,0.0,0.0,0.0,1.0,0.0,1.0,1.0,0.0,0.0	//��ɫ���� 
	};
	
	struct SolarsObjectRail Objs = {
	};
	FILE* fp,* rps;// = NULL;
/*******************************SETTING START*****************************/	
    Years = 0;
    ClockMode = 0;
	ArvMode = 0;
	PCount = 0;
	CaluateSignOfSP1 = 0.0;
	CaluateSignOfSP2 = 0.0;
	CaluateSignOfSP3 = 0.0;
	
	setinitmode(0); // �ر�ege��������
    initgraph(1080, 960,INIT_RENDERMANUAL); // ��ʼ������ʾһ������    INIT_NOBORDER ��û�д���    INIT_RENDERMANUAL���ֶ�ģʽ���Զ�ģʽ��д����Ļ�� 
    sprintf(Title,"%s %s",Mode,Version);
	setcaption(Title);	
/*******************************SETTING END*******************************/ 
	cleardevice(); 
    fp = NULL;
	fp = fopen(filename,"r");     // fpָ���ļ�ͷ��
    for(int i = 0 ; i < 25 ; i++)
        for(int j = 0 ; j < 2000 ; j++){//��ֵ������С�����ٶȼӿ� 
            fscanf(fp,"%[^,\n]",table[i][j]);  // �������Ż�\nΪֹ
            fgetc(fp); // ��ȡһ���ַ�(���Ż�\n)
        }
    fclose(fp);
    rps = NULL;
     outtextxy(300,0,"�ļ� Solars.csv ������ϣ�"); 
	//fseek(fp,0,SEEK_SET);  
	///�ر�ע�⣬�����ȴ���xlsx�ٸ�Ϊcsv��Ҫ����txt���ٸ�xlsx 
	rps = fopen("Ob.csv","r");    
    for(int i = 0 ; i < 6 ; i++)
        for(int j = 0 ; j < 2000 ; j++){
            fscanf(rps,"%[^,\n]",KKR[i][j]); 
            fgetc(rps); 
        }
    fclose(rps);
	ClockMode = 1;
	SCount = 0;
	while(ClockMode)
	{
		if(Develop)
		{
		sprintf(s,"%d %s",SCount,KKR[SCount][5]);
		outtextxy(0,80+SCount*10,s);
		}
		if(atoi(KKR[SCount][5]) == 10)///���ǣ�ΪʲôҪ�ǡ�6���Ĳ������� 
		{
			SCount ++;
		}
		else
		{
			NumofTag = SCount;
			ClockMode = 0;	
		}
	}
	SCount = 0;
	//outtextxy(10,10,KKR[0][5]);
    NumofSolor = atoi(table[0][9]);//���ע��᲻�ᳬ��Χ����
	for(PCount=0;PCount<=NumofTag;PCount++)
	{
		Objs.From[PCount] = atoi(KKR[PCount][0]);
		Objs.To[PCount] = atoi(KKR[PCount][1]);
		Objs.vpc[PCount] = atof(KKR[PCount][2]);
		Objs.type[PCount] = atoi(KKR[PCount][3]);
		Objs.Per[PCount] = atof(KKR[PCount][4]) ;
	} 
	delay_fps(50);
   outtextxy(300,0,"������ϣ�ʹ�ù��ּ��ɽ�����Ϸ");
   outtextxy(20,400,"����C���򵥻�����л����㣬����F��ת������ģʽ�������ֿ��ƴ�С��������ģʽ��K");
   outtextxy(10,600,"�����������Ӧ,��shiftȡ�����뷨������޷��л���Ŀ����ϵ����Сһ���̶Ⱥ��������");
	sprintf(Title,"���԰汾 %s",Version);
	setcaption(Title);	
   delay_fps(50);
    STP = clock();
    ///////��ʵ����Ҫ����"�Ӵ浵��ȡ" �����ǿ���ͨ���̳д浵ʵ��Ŀ�� 
   for (; k.key != key_esc; ) //; is_run(); delay_fps(10))//; k.key != key_esc; ) // key_esc��ege����� Esc ���ļ�ֵ
   {
   		if((clock()-STP) >= 1000)//��ʱ��,��λms 
   		{
   			STP = clock();
   			if(ClockMode)
   			{
   			Years += 1;
   			sprintf(Title,"�����꣺%d",Years);
   			outtextxy(0,0,Title);
   			
			for(PCount=0;PCount<=NumofTag;PCount++)////�������� 
			{
				Objs.Per[PCount] -= 1;
				if(Objs.Per[PCount] <= 0.0)
				{
					sprintf(s,"����%d �Ѿ���ɣ�",PCount);
					outtextxy(300,0,s);
				}
   			delay_ms(1);
			} 
			
			}
			
		   }
       if ( kbmsg() )// ��ȡ������Ϣ����û�а�������������ʹ�ô�GetKey���һֱ�ȴ� 
	   {
	   		 k = getkey();//��ӡ���̰��º�̧������� key_msg�ṹ���ֵ
	    	if(keystate('C')) //�л�Ŀ�� 
            {
            	PageChanged = 1;
            	OrbitSN += 1;
            	if(OrbitSN == NumofSolor)
            	{
            		OrbitSN = 1;
				}
            } 
            if(keystate('K')) ///������ģʽ 
			{
				//TDcall(100,100,900,900,10);//����ŵ�������ģʽ��������ӻ��Ǻ���Ӧ�� 
				DrawTmpRGB(0,900,1000.0,30000.0,30.0);
				Develop = 1;
			} 
                   
	    	if(keystate('D')) ///�л�ҳ�� 
            {
            	PageChanged = 1;
            	Page += 1;
            	if(Page == 3)
            	{
            		Page = 1;
				}
            	delay_ms(200);////����ˢ���ٶ�̫���� -125hz 
            } 
            if(keystate('F'))
			{
				ArvMode += 1;
				if(ArvMode == 3)
				{
					ArvMode = 0;
				}
				PageChanged = 1;
				delay_ms(200);
			 } 
		}
		 
		if (mousemsg()) //����ȡ  
       {
       	m = GetMouseMsg();//�����ĵ����ǲ��Ƽ����÷���ԭ���ǻ�ȡ��� 
		switch (m.uMsg) ///������ݴ��� 
		{
		case WM_LBUTTONDOWN:
		if(Page == 1)
		{
		SCount = 0; ////����л����� 
		for(;SCount <= NumofSolor;SCount++)
    	{	CaluateTranslateStarsXYR = StarsTableR[SCount]*2;
    	CaluateTranslateStarsXYR += 10.0;
			if(m.x >= StarsTableX[SCount] - CaluateTranslateStarsXYR)
			{
				if(m.x <= StarsTableX[SCount] + CaluateTranslateStarsXYR)
				{
					if(Develop)
					{
					sprintf(s,"X->%d",SCount);
					outtextxy(0,20,s); 
					delay_fps(50);
					}
					if(m.y >= StarsTableY[SCount] - CaluateTranslateStarsXYR)
					{
						if(m.y <= StarsTableY[SCount] + CaluateTranslateStarsXYR)
						{
							
					if(Develop)
					{
					sprintf(s,"Y(X-passed)->%d",SCount);
					outtextxy(0,20,s);
					delay_fps(50); 
					}
							OrbitSN = SCount;
							PageChanged = 1;
						}
					}
				}
			}
		}
		}
		if(Page == 2)
		{
			ClockMode = 0;
		Stars.ally = atoi(table[OrbitSN][8]);
			switch(Stars.ally)
		{
			case 0 :
				sprintf(n,"�ҷ���ϵ��������");
				ClockMode = 1; 
				break;
			case 1 :
				sprintf(n,"�з���ϵ���ҷ��޷�����");
				break;
			case 2:
				sprintf(n,"������ϵ���ҷ��޷�����");
				break;
		} 
			outtextxy(0,100,n);
			delay_ms(2);
		if(ClockMode)
		{
			FTCount = OrbitSN;
		}
		//	outtextxy(0,20,c);
		ClockMode = 0;
			delay_ms(20);
			////char�������Ͳ�ͬ�Ĳ�����ת 
		}
		SCount = 0;
			break;
		case WM_RBUTTONDOWN:
			if(Page == 2)////////////////////////�½�����/���ƺ��� 
			{
				ClockMode = 0;
		Stars.ally = atoi(table[OrbitSN][8]);
			switch(Stars.ally)
		{
			case 0 :
				sprintf(n,"�ҷ���ϵ���ܾ�����ָ��");
				break;
			case 1 :
				sprintf(n,"�з���ϵ���ҷ�ʵʩ����");
				ClockMode = 1;
				break;
			case 2:
				sprintf(n,"������ϵ���ҷ��ɳ�����");
				ClockMode = 2;
				break;
		} 
		outtextxy(0,80,n);
			delay_ms(2);
		if(ClockMode != 0)
		{
			sprintf(NewKkePrj[0][1],"%d",FTCount);////�ַ������ܵ����������ã� 
			sprintf(NewKkePrj[0][2],"%d",OrbitSN);
		if(FTCount <= NumofSolor)
		{
				FILE *flsa;
			// ��CSV�ļ�����д��
    flsa = fopen("Ob.csv","a+");
    if (flsa == NULL) 
	{
        outtextxy(0,180,"Error opening file.");
        //return 1;
    }
    CaluateSignOfSP2 = atof(table[FTCount][1]); 
    CaluateSignOfSP1 = pow(CaluateSignOfSP2,2);
    CaluateSignOfSP3 = atof(table[FTCount][2]);
    CaluateSignOfSP2 = pow(CaluateSignOfSP3,2);
    CaluateSignOfSP3 = CaluateSignOfSP1 + CaluateSignOfSP2;
    CaluateSignOfSP2 = atof(table[FTCount][3]);
    CaluateSignOfSP1 = pow(CaluateSignOfSP2,2);
    CaluateSignOfSP3 += CaluateSignOfSP1;
    CaluateSignOfSP1 = pow(CaluateSignOfSP3,0.5);
    switch(ArvMode)
    {
    	case 0:
    		sprintf(NewKkePrj[0][3],"%f",1);
    		CaluateSignOfSP2 = CaluateSignOfSP1 / 1;
    		break;
    	case 1:
    		sprintf(NewKkePrj[0][3],"%f",0.8);
    		CaluateSignOfSP2 = CaluateSignOfSP1 / 0.8;
    		break;
    	case 2:
    		sprintf(NewKkePrj[0][3],"%f",0.1);
			////����֮��ʹ�ù���Ϊ��λ��������ֱ���������״̬����Ҫ���ٹ��� 
    		CaluateSignOfSP2 = CaluateSignOfSP1 / 0.1;
    		break;
	}
    sprintf(NewKkePrj[0][5],"%f",CaluateSignOfSP2);
	switch(ClockMode)
	{
		case 1:
		sprintf(NewKkePrj[0][4],"%d",1);
			break;
		case 2:
		sprintf(NewKkePrj[0][4],"%d",2);
			break;
	}
	////1-our  2-target
 	sprintf(s,"%s,%s,%s,%s,%s,10\n",NewKkePrj[0][2],NewKkePrj[0][1],NewKkePrj[0][3],NewKkePrj[0][4],NewKkePrj[0][5]);
	 ////�ƺ�1��2 д���ˣ�������Ⱦ�Ͽ�������� 
 	if(Develop)
 	{
 	outtextxy(0,200,s);
	 }
    // д�����ݵ��ļ�
    fprintf(flsa, s);
    // �ر��ļ�
    fclose(flsa);
    rewind(flsa);
	flsa = NULL;
	flsa = fopen("Ob.csv","r");    
    for(int i = 0 ; i < 6 ; i++)
        for(int j = 0 ; j < 2000 ; j++){
            fscanf(flsa,"%[^,\n]",KKR[i][j]); 
            fgetc(flsa); 
        }
    fclose(flsa);
    
	ClockMode = 1;
	SCount = 0;
	while(ClockMode)
	{
		if(Develop)
		{
		sprintf(s,"%d %s",SCount,KKR[SCount][5]);
		outtextxy(0,80+SCount*10,s);
		}
		if(atoi(KKR[SCount][5]) == 10)///���ǣ�ΪʲôҪ�ǡ�6���Ĳ������� 
		{
			SCount ++;
		}
		else
		{
			NumofTag = SCount;
			ClockMode = 0;	
		}
	}
	SCount = 0;
	
	for(PCount=0;PCount<=NumofTag;PCount++)////�������� 
	{
		Objs.From[PCount] = atoi(KKR[PCount][0]);
		Objs.To[PCount] = atoi(KKR[PCount][1]);
		Objs.vpc[PCount] = atof(KKR[PCount][2]);
		Objs.type[PCount] = atoi(KKR[PCount][3]);
		Objs.Per[PCount] = atof(KKR[PCount][4]) ;
	} 
	PCount = 0;
		}
		else
		{
 	sprintf(s,"ERROR:   %s,%s,0.2,1,0.0\n",NewKkePrj[0][1],NewKkePrj[0][2]);
 	outtextxy(0,200,s); 
		}
		
		}
		ClockMode = 0;
			delay_ms(2);
			
			}
			if(Page == 1)////�л�ҳ����� 
			{
			SCount = 0;
		for(;SCount <= NumofSolor;SCount++)
    	{	CaluateTranslateStarsXYR = StarsTableR[SCount]*2;
    	CaluateTranslateStarsXYR += 10.0;
			if(m.x >= StarsTableX[SCount] - CaluateTranslateStarsXYR)
			{
				if(m.x <= StarsTableX[SCount] + CaluateTranslateStarsXYR)
				{
					if(Develop)
					{
					sprintf(s,"X->%d",SCount);
					outtextxy(0,20,s); 
					delay_fps(50);
					}
					if(m.y >= StarsTableY[SCount] - CaluateTranslateStarsXYR)
					{
						if(m.y <= StarsTableY[SCount] + CaluateTranslateStarsXYR)
						{
							
							if(Develop)
							{
							sprintf(s,"Y(X-passed)->%d",SCount);
							outtextxy(0,20,s);
							delay_fps(50); 
							}
							OrbitSN = SCount;
							Page = 2;
							PageChanged = 1;
						}
					}
				}
			}
		}
	}
			break;
		case WM_MOUSEMOVE:
			break;
			
		case WM_MOUSEWHEEL:////���Ŵ��� 
			PageChanged = 1;
		    if(m.wheel >= 1)
		    {
		    	PXD += 5.0;
		    	if(PXD>=130.0)
		    	{
		    		PXD += 1.0;
				}
			}
			else if(m.wheel <= -1)
			{
				PXD -= 5.0;
				if(PXD <= 0.0)
				{
					PXD = 0.0;
				}
				
		    	if(PXD>=130.0)
		    	{
		    		PXD -= 3.0;
				}
			}
			else
			{
				PXD = PXD;
			}
		 	m.wheel = 0;
			break;
		}
		}
		if(PageChanged)/////ҳ��ı��Ӧ���� 
		{
			PageChanged = 0;	
			cleardevice(); 
			  if(Page == 1)
						  {
	  	ClockMode = 1;
	    SCount = 0;
		for(;SCount <= NumofSolor;SCount++) ///����ͼƬ 
    	{
    	Stars.IdentifyNum = atoi(table[SCount][0]);
    	Stars.lox = atof(table[SCount][1])-atof(table[OrbitSN][1]);//////��һ���������� 
    	Stars.loy = atof(table[SCount][2])-atof(table[OrbitSN][2]);
    	Stars.loz = atof(table[SCount][3])-atof(table[OrbitSN][3]);
   	 	//Stars.radius = atof(table[SCount][4]);//0:ע����  1��̫��  
    	Stars.mass = atof(table[SCount][4]); ///ע��Ҫ��Mass������ 
   	 	Stars.lum = Starslum(Stars.mass);   //ʹ�÷ֶκ������ 
    	Stars.radius = StarsRadius(Stars.mass);
		Stars.ally = atoi(table[SCount][8]);
		Stars.Teff = StarsTemp(Stars);
		BlackBody_Translate_RGB(Stars.Teff);
		setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
		PrtCal(Stars.lox,Stars.loy,Stars.loz,Stars.IdentifyNum,PXD);
		FillCircle(PLD.lx,PLD.ly,Stars.radius*PXD/30,PXD); 
		
		StarsTableX[SCount]=PLD.lx;
		StarsTableY[SCount]=PLD.ly;
		StarsTableR[SCount]=Stars.radius*PXD/30;
		setcolor(LIGHTGRAY); 
		if(PXD>=50.0)
		{ 
		switch(Stars.ally)
		{
			case 0 :
				sprintf(n,"�ҷ�");
				break;
			case 1 :
				sprintf(n,"�з�");
				break;
			case 2:
				sprintf(n,"����");
				break;
		} 
		sprintf(s,"%s %s",table[SCount][7],n);
		outtextxy(PLD.lx+2,PLD.ly+2,s); 
		}
		Stars.lum = PLD.ly;
		PrtCal(Stars.lox,Stars.loy,-atof(table[OrbitSN][3]),Stars.IdentifyNum,PXD);
		line(PLD.lx,Stars.lum,PLD.lx,PLD.ly);
		}
				PCount = 0;
		for(;PCount <= NumofSolor;PCount++)
		{
			
	    SCount = 0;
		for(;SCount <= NumofSolor;SCount++) ///����ͼƬ 
    	{
    		Stars.IdentifyNum = atoi(table[SCount][0]);
			if(Objs.From[PCount] == Stars.IdentifyNum)
			{
				setcolor(LIGHTGRAY);
				PrtCal(atof(table[Objs.From[PCount]][1])-atof(table[OrbitSN][1]),atof(table[Objs.From[PCount]][2])-atof(table[OrbitSN][2]),atof(table[Objs.From[PCount]][3])-atof(table[OrbitSN][3]),0,PXD);
				Stars.radius = PLD.lx;
				Stars.lum = PLD.ly;
				PrtCal(atof(table[Objs.To[PCount]][1])-atof(table[OrbitSN][1]),atof(table[Objs.To[PCount]][2])-atof(table[OrbitSN][2]),atof(table[Objs.To[PCount]][3])-atof(table[OrbitSN][3]),0,PXD);	
				switch(Objs.type[PCount])
				{
					case 0://////////�¼��㲥������޷����� ����׶ģ�⣬�Ժ������� 
						break;
					case 1://////////�����켣 
						setcolor(RED);
						break;
					/*
					case 2://////////������ 
						setcolor(BLUE);
						break;
						
					case 3://////////������ 
						setcolor(YELLOW);
						break;*/
				}
				line(Stars.radius,Stars.lum,PLD.lx,PLD.ly);
				/*
				PrtCal((atof(table[Objs.To[PCount]][1])-atof(table[OrbitSN][1]))*Objs.Per[PCount],(atof(table[Objs.To[PCount]][2])-atof(table[OrbitSN][2]))*Objs.Per[PCount],(atof(table[Objs.To[PCount]][3])-atof(table[OrbitSN][3]))*Objs.Per[PCount],0,PXD);
				line(Stars.radius,Stars.lum,PLD.lx,PLD.ly);
				line(Stars.radius,Stars.lum,PLD.lx-3,PLD.ly-3);
				line(Stars.radius,Stars.lum,PLD.lx+3,PLD.ly+3);
				*/
				///�������һ���û�㶮���Ժ������� 
			}
    	}
	 	}
		if(Develop)/////��ʾ��ɫ������ 
		{
		setcolor(RED);
		PrtCal(50-atof(table[OrbitSN][1]),-atof(table[OrbitSN][2]),-atof(table[OrbitSN][3]),0,PXD);
		Stars.radius = PLD.lx;
		Stars.lum = PLD.ly;
		PrtCal(-atof(table[OrbitSN][1]),-atof(table[OrbitSN][2]),-atof(table[OrbitSN][3]),0,PXD);
		line(Stars.radius,Stars.lum,PLD.lx,PLD.ly);
		setcolor(BLUE);
		PrtCal(-atof(table[OrbitSN][1]),50-atof(table[OrbitSN][2]),-atof(table[OrbitSN][3]),0,PXD);
		Stars.radius = PLD.lx;
		Stars.lum = PLD.ly;
		PrtCal(-atof(table[OrbitSN][1]),-atof(table[OrbitSN][2]),-atof(table[OrbitSN][3]),0,PXD);
		line(Stars.radius,Stars.lum,PLD.lx,PLD.ly);
		setcolor(YELLOW);
		PrtCal(-atof(table[OrbitSN][1]),-atof(table[OrbitSN][2]),50-atof(table[OrbitSN][3]),0,PXD);
		Stars.radius = PLD.lx;
		Stars.lum = PLD.ly;
		PrtCal(-atof(table[OrbitSN][1]),-atof(table[OrbitSN][2]),-atof(table[OrbitSN][3]),0,PXD);
		line(Stars.radius,Stars.lum,PLD.lx,PLD.ly);
		setcolor(LIGHTGRAY);
		}
		
	  
	}
		if(Page == 2)
	   	{
	   		ClockMode = 0;
    	Stars.IdentifyNum = atoi(table[OrbitSN][0]);
    	Stars.mass = atof(table[OrbitSN][4]);
    	Stars.HeavyElement = atof(table[OrbitSN][6]);
   	 	Stars.lum = Starslum(Stars.mass);   //ʹ�÷ֶκ������ 
    	Stars.radius = StarsRadius(Stars.mass);
		Stars.ally = atoi(table[OrbitSN][8]);
		Stars.Teff = StarsTemp(Stars);
		Stars.Facmass = atof(table[OrbitSN][10]);
		Stars.Powmass = atof(table[OrbitSN][11]);		
		Stars.Unusemass = atof(table[OrbitSN][12]);
		BlackBody_Translate_RGB(Stars.Teff);
		setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
		//PrtCal(Stars.lox,Stars.loy,Stars.loz,Stars.IdentifyNum,1);
		FillCircle(400,500,Stars.radius*5,100); 
		
    	if(Stars.mass != 0.0) ////˫��֧�� 
		{
    	Stars.mass = atof(table[OrbitSN][13]);
   	 	Stars.lum = Starslum(Stars.mass); 
		Stars.radius = StarsRadius(Stars.mass);
		Stars.Teff = StarsTemp(Stars);
		BlackBody_Translate_RGB(Stars.Teff);
		setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
		FillCircle(600,500,Stars.radius*5,100); 
		 } 
		 
   	 	
		switch(Stars.ally) ///��Ϣ�� 
		{
			case 0 :
				setcolor(GREEN); 
				sprintf(n,"���õ���ʩ����:%f ������ʩ����:%f δ������:%f  �ҷ�",Stars.Facmass,Stars.Powmass,Stars.Unusemass);
				break;
			case 1 :
				setcolor(RED); 
				sprintf(n,"���õ���ʩ����:%f ������ʩ����:%f δ������:%f  �з�",Stars.Facmass,Stars.Powmass,Stars.Unusemass);
				break;
			case 2:
				setcolor(LIGHTGRAY); 
				sprintf(n,"���õ���ʩ����:%f ������ʩ����:%f δ������:%f  ����",Stars.Facmass,Stars.Powmass,Stars.Unusemass);
				break;
		} 
		outtextxy(0,30,n); 
		setcolor(LIGHTGRAY); 
		sprintf(s,"��%s ������%f �뾶��%f ��ȣ�%f �¶ȣ�%f ��Ԫ�ط�ȣ�%f",table[OrbitSN][7],Stars.mass,Stars.radius,Stars.lum,Stars.Teff,Stars.HeavyElement);
		outtextxy(0,0,s); 
		switch(ArvMode)
		{
			case 0://////��ɭ���� 
			sprintf(s,"��������з������ɭ�����������䷢����ʩ��ѹ���乤ҵ����");
				break;
			case 1:////RKKV--��������۶������� 
			sprintf(s,"��������з�����RKKV��ƾ�趯������𺦣������ױ�����һ����ҵ����ϵ����");
				break;
			case 2:////������
			sprintf(s,"��������з������ŵ���������Ӹ��壩,�����ǹ���������ս��");
				break;
		}
		outtextxy(300,100,s);
	  	outtextxy(0,800,"��ʱ��ֹ ����D���˳�����ģ��");
	  	outtextxy(0,830,"���ѡ���ҷ�ִ����ϵ  �Ҽ�����/���͸������Ŀ����ϵ���Զ�ʶ��");
				Stars.mass = atof(table[OrbitSN][4]);
				if(Stars.mass == 0.00)////�ص�
			{
				PageChanged = 1;
				OrbitSN = 1;
			}
	  }
	  if(Page == 3)
	  {
	  	Page = 1;	
	  	PageChanged = 1; 
	  }
	  
	  if(Develop)
	  {
	  sprintf(n,"Page:%d",Page);
		outtextxy(0,30,n); 
	  }
		delay_fps(50);/// ���ҽ����ڽ��д���Delay�����ʱ�����ˢ�� �������������������֡��
		////��Ϊ������������²ſ�����֡���ȶ�  ��ˣ�֮ǰ���Ǹ���ottextxy�����ܻ��ĸ�Դ�������������˳���. 
		}
   }
    closegraph(); // �ر�ͼ�ν��� 
	exit(-1); //���˸��� 
} 

//���㹫ʽ
 

//ѧϰ���� 
//https://blog.csdn.net/cnds123/article/details/109982100 
//https://zhuanlan.zhihu.com/p/29818711
//https://blog.csdn.net/TYPYDEF/article/details/77926605
//https://blog.csdn.net/qq_39151563/article/details/104342349
//https://blog.csdn.net/agoniangel/article/details/104631806 
//https://blog.csdn.net/qq_39151563/article/details/104342730(��꣩ 
