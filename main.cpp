#include "graphics.h" //需要引用这个图形库  D:\Dev-Cpp\MinGW64\lib\gcc\x86_64-w64-mingw32\4.9.2\include
#include "stdio.h"
//#include <iostream>
#include "gdiplus.h"
#include "conio.h"
#include "time.h"
#include <string>
#include <algorithm> //C++17 如果是DevCpp，必须升级内核版本 
#include "math.h" 
#include "GraPGameLib-Y/GPGL.h" /////后继相关函数打包到这里！ 

using namespace Gdiplus;
using namespace std;

#define _CRT_SECURE_NO_WARNINGS
//使用图形化选项以禁用CMD窗口 _适用于VSC  所有学习代码已贴在文末 

#define Left_Click 0
#define Right_Click 1

#define GH2 1.4142135623731


bool Develop = 0;      /////////////////////////////////调试信息//////////////////////////////

struct PhysicSX               //物体性质 
{
	unsigned int IdetifyNum;  //同一个物体使用同一个ID码——AU为单位时的点模拟 
	unsigned int ton;		  //单位：吨 
	int hard;				  //单位：硬度
	int Completeness;         //完整度（血量）--要与图片绑定 （同时也可以作为易开采资源指示） 
    float Heat;				  //表面温度 单位 开尔文 
};

struct Pytloc
{
	int IdentifyNum;
	double lx,ly,lz;				//地理点结构体——用于更进一步描述状态，比如多体星系 
	double vx,vy,vz;				//速度
	double VmixXY,VmixXZ;
};

struct SolarsObjectRail     ////RKKV和光锥 
{
	unsigned int From[20],To[20];////对应恒星系编号 
	float vpc[20];			////相对光速 
	unsigned int type[20];	///类型 
	float Per[20];			///抵达进度 
};

struct ColorInt
{
	int Red,Green,Blue;	
};

struct ColorInt ALS = {////////////////////////////定义一个全局结构体用于传递RGB参数//////////////////////////////// 
	0,0,0
};

struct Pytloc PLD = {/// 
	0,0.0,0.0,0.0,0.0,0.0,0.0
};

void BlackBody_Translate_RGB(float kelvin) ///黑体转RGB -使用了全局变量 
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
///温度转RGB来自： https://computergraphics.stackexchange.com/questions/9526/blackbody-curve-to-floating-rgb 
	RI = int(red*255.0f);
	GI = int(green*255.0f);
	BI = int(blue*255.0f);
	
	ALS.Red = RI;
	ALS.Blue = BI;
	ALS.Green = GI;//注意别填错！ 
	//return red,green,blue;
}



int GSCalluate(int RDY,int GLP) ///操作格栅化——高斯函数 
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

void PrtCal(double Xq,double Yq,double Zq,int IdentifyNum,double BS) //视角转换计算 -使用了全局变量 
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

void FillCircle(float x,float y,float R,double BS) ///填充一个带渐变效果的圆形，因为自带的需要制定颜色作为边框  
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

void DrawTmpRGB(int x,int y,float LwK,float HwK,float CZX) ///测试代码，画一个温度条 
{
	int count,Tcount;
	float Ntk;
	bool ESOX;
	ESOX = 1;
	count = 0;
	Ntk = LwK;
	Tcount = 0;//CZX:插值，精细度，越低越精细 
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
 
int main(int argc, char* argv[])//int argc, char* argv[]) //启用11标准需要入口函数 
{ 
/******************************DEFINE START*******************************/ 
	char WelCome[] = "Loading";
	key_msg k = {0};
	MOUSEMSG m = {0};
	bool HWtestmode = 0;
	char Version[] = "0.0.2.dev";
	char Mode[] = "请等待几秒钟…… "; 
    char Title[30];
    char DevLog[20];
	struct ColorInt CIKS = {
		0,0,0
	};
	char filename[] = "So.csv";
    char table[20][15][30];//这个有大坑……必须注意第二个值必须和csv文件严格对应的横坐标范围！ 
    char KKR[200][6][5];//须注意第二个值必须和csv文件严格对应的横坐标范围！ 
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
		//1,0.0,0.0,0.0,1.0,0.0,1.0,1.0,0.0,0.0	//通过光度，半径，计算等效温度Teff  这是测试范例 
	};
	
	struct StarsSelf Stars = {//公用 
	//	1,0.0,0.0,0.0,1.0,0.0,1.0,1.0,0.0,0.0	//颜色现算 
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
	
	setinitmode(0); // 关闭ege启动画面
    initgraph(1080, 960,INIT_RENDERMANUAL); // 初始化，显示一个窗口    INIT_NOBORDER ：没有窗口    INIT_RENDERMANUAL：手动模式（自动模式会写爆屏幕） 
    sprintf(Title,"%s %s",Mode,Version);
	setcaption(Title);	
/*******************************SETTING END*******************************/ 
	cleardevice(); 
    fp = NULL;
	fp = fopen(filename,"r");     // fp指向文件头部
    for(int i = 0 ; i < 25 ; i++)
        for(int j = 0 ; j < 2000 ; j++){//数值尽可能小会令速度加快 
            fscanf(fp,"%[^,\n]",table[i][j]);  // 读到逗号或\n为止
            fgetc(fp); // 读取一个字符(逗号或\n)
        }
    fclose(fp);
    rps = NULL;
     outtextxy(300,0,"文件 Solars.csv 加载完毕！"); 
	//fseek(fp,0,SEEK_SET);  
	///特别注意，不能先创个xlsx再改为csv得要先用txt，再改xlsx 
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
		if(atoi(KKR[SCount][5]) == 10)///不是，为什么要是【6】的参数啊？ 
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
    NumofSolor = atoi(table[0][9]);//你得注意会不会超范围……
	for(PCount=0;PCount<=NumofTag;PCount++)
	{
		Objs.From[PCount] = atoi(KKR[PCount][0]);
		Objs.To[PCount] = atoi(KKR[PCount][1]);
		Objs.vpc[PCount] = atof(KKR[PCount][2]);
		Objs.type[PCount] = atoi(KKR[PCount][3]);
		Objs.Per[PCount] = atof(KKR[PCount][4]) ;
	} 
	delay_fps(50);
   outtextxy(300,0,"加载完毕，使用滚轮即可进入游戏");
   outtextxy(20,400,"按下C键或单击鼠标切换焦点，按下F键转换攻击模式，鼠标滚轮控制大小，开发者模式按K");
   outtextxy(10,600,"如果程序无响应,按shift取消输入法，如果无法切换至目标星系，缩小一定程度后左键单击");
	sprintf(Title,"测试版本 %s",Version);
	setcaption(Title);	
   delay_fps(50);
    STP = clock();
    ///////其实这里要做成"从存档读取" ，但是可以通过继承存档实现目的 
   for (; k.key != key_esc; ) //; is_run(); delay_fps(10))//; k.key != key_esc; ) // key_esc是ege定义的 Esc 键的键值
   {
   		if((clock()-STP) >= 1000)//定时器,单位ms 
   		{
   			STP = clock();
   			if(ClockMode)
   			{
   			Years += 1;
   			sprintf(Title,"运行年：%d",Years);
   			outtextxy(0,0,Title);
   			
			for(PCount=0;PCount<=NumofTag;PCount++)////重载数据 
			{
				Objs.Per[PCount] -= 1;
				if(Objs.Per[PCount] <= 0.0)
				{
					sprintf(s,"任务：%d 已经完成！",PCount);
					outtextxy(300,0,s);
				}
   			delay_ms(1);
			} 
			
			}
			
		   }
       if ( kbmsg() )// 获取按键信息，若没有按键会继续，如果使用纯GetKey则会一直等待 
	   {
	   		 k = getkey();//打印键盘按下和抬起过程中 key_msg结构体的值
	    	if(keystate('C')) //切换目标 
            {
            	PageChanged = 1;
            	OrbitSN += 1;
            	if(OrbitSN == NumofSolor)
            	{
            		OrbitSN = 1;
				}
            } 
            if(keystate('K')) ///开发者模式 
			{
				//TDcall(100,100,900,900,10);//就算放到开发者模式里这个格子还是很膈应人 
				DrawTmpRGB(0,900,1000.0,30000.0,30.0);
				Develop = 1;
			} 
                   
	    	if(keystate('D')) ///切换页面 
            {
            	PageChanged = 1;
            	Page += 1;
            	if(Page == 3)
            	{
            		Page = 1;
				}
            	delay_ms(200);////键盘刷新速度太快了 -125hz 
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
		 
		if (mousemsg()) //鼠标读取  
       {
       	m = GetMouseMsg();//这在文档中是不推荐的用法，原因是会等…… 
		switch (m.uMsg) ///鼠标数据处理 
		{
		case WM_LBUTTONDOWN:
		if(Page == 1)
		{
		SCount = 0; ////左键切换代码 
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
				sprintf(n,"我方星系，就绪！");
				ClockMode = 1; 
				break;
			case 1 :
				sprintf(n,"敌方星系，我方无法利用");
				break;
			case 2:
				sprintf(n,"中立星系，我方无法利用");
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
			////char【】类型不同的不可以转 
		}
		SCount = 0;
			break;
		case WM_RBUTTONDOWN:
			if(Page == 2)////////////////////////新建攻击/复制航线 
			{
				ClockMode = 0;
		Stars.ally = atoi(table[OrbitSN][8]);
			switch(Stars.ally)
		{
			case 0 :
				sprintf(n,"我方星系，拒绝攻击指令");
				break;
			case 1 :
				sprintf(n,"敌方星系，我方实施进攻");
				ClockMode = 1;
				break;
			case 2:
				sprintf(n,"中立星系，我方派出工厂");
				ClockMode = 2;
				break;
		} 
		outtextxy(0,80,n);
			delay_ms(2);
		if(ClockMode != 0)
		{
			sprintf(NewKkePrj[0][1],"%d",FTCount);////字符串不能单独分离来用？ 
			sprintf(NewKkePrj[0][2],"%d",OrbitSN);
		if(FTCount <= NumofSolor)
		{
				FILE *flsa;
			// 打开CSV文件进行写入
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
			////恒星之间使用光年为单位，这里是直接算出匀速状态下需要多少光年 
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
	 ////似乎1和2 写反了，但是渲染上看不出差别 
 	if(Develop)
 	{
 	outtextxy(0,200,s);
	 }
    // 写入数据到文件
    fprintf(flsa, s);
    // 关闭文件
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
		if(atoi(KKR[SCount][5]) == 10)///不是，为什么要是【6】的参数啊？ 
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
	
	for(PCount=0;PCount<=NumofTag;PCount++)////重载数据 
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
			if(Page == 1)////切换页面代码 
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
			
		case WM_MOUSEWHEEL:////缩放代码 
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
		if(PageChanged)/////页面改变对应函数 
		{
			PageChanged = 0;	
			cleardevice(); 
			  if(Page == 1)
						  {
	  	ClockMode = 1;
	    SCount = 0;
		for(;SCount <= NumofSolor;SCount++) ///绘制图片 
    	{
    	Stars.IdentifyNum = atoi(table[SCount][0]);
    	Stars.lox = atof(table[SCount][1])-atof(table[OrbitSN][1]);//////这一部分有问题 
    	Stars.loy = atof(table[SCount][2])-atof(table[OrbitSN][2]);
    	Stars.loz = atof(table[SCount][3])-atof(table[OrbitSN][3]);
   	 	//Stars.radius = atof(table[SCount][4]);//0:注释行  1：太阳  
    	Stars.mass = atof(table[SCount][4]); ///注意要有Mass才能算 
   	 	Stars.lum = Starslum(Stars.mass);   //使用分段函数拟合 
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
				sprintf(n,"我方");
				break;
			case 1 :
				sprintf(n,"敌方");
				break;
			case 2:
				sprintf(n,"中立");
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
		for(;SCount <= NumofSolor;SCount++) ///绘制图片 
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
					case 0://////////事件广播，玩家无法控制 ，光锥模拟，以后再来做 
						break;
					case 1://////////攻击轨迹 
						setcolor(RED);
						break;
					/*
					case 2://////////复制体 
						setcolor(BLUE);
						break;
						
					case 3://////////运输链 
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
				///进度条我还是没搞懂，以后再来搞 
			}
    	}
	 	}
		if(Develop)/////显示三色坐标轴 
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
   	 	Stars.lum = Starslum(Stars.mass);   //使用分段函数拟合 
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
		
    	if(Stars.mass != 0.0) ////双星支持 
		{
    	Stars.mass = atof(table[OrbitSN][13]);
   	 	Stars.lum = Starslum(Stars.mass); 
		Stars.radius = StarsRadius(Stars.mass);
		Stars.Teff = StarsTemp(Stars);
		BlackBody_Translate_RGB(Stars.Teff);
		setcolor(EGERGB(ALS.Red,ALS.Green,ALS.Blue));
		FillCircle(600,500,Stars.radius*5,100); 
		 } 
		 
   	 	
		switch(Stars.ally) ///信息条 
		{
			case 0 :
				setcolor(GREEN); 
				sprintf(n,"·用电设施质量:%f 发电设施质量:%f 未利用量:%f  我方",Stars.Facmass,Stars.Powmass,Stars.Unusemass);
				break;
			case 1 :
				setcolor(RED); 
				sprintf(n,"·用电设施质量:%f 发电设施质量:%f 未利用量:%f  敌方",Stars.Facmass,Stars.Powmass,Stars.Unusemass);
				break;
			case 2:
				setcolor(LIGHTGRAY); 
				sprintf(n,"·用电设施质量:%f 发电设施质量:%f 未利用量:%f  中立",Stars.Facmass,Stars.Powmass,Stars.Unusemass);
				break;
		} 
		outtextxy(0,30,n); 
		setcolor(LIGHTGRAY); 
		sprintf(s,"·%s 质量：%f 半径：%f 光度：%f 温度：%f 重元素丰度：%f",table[OrbitSN][7],Stars.mass,Stars.radius,Stars.lum,Stars.Teff,Stars.HeavyElement);
		outtextxy(0,0,s); 
		switch(ArvMode)
		{
			case 0://////戴森光束 
			sprintf(s,"·将会向敌方发射戴森光束，干扰其发电设施，压制其工业产能");
				break;
			case 1:////RKKV--快速相对论动能武器 
			sprintf(s,"·将会向敌方发射RKKV，凭借动能造成损害，但容易被具有一定工业的星系拦截");
				break;
			case 2:////慢舰队
			sprintf(s,"·将会向敌方发射冯诺依曼机（子个体）,它们是工厂，亦是战舰");
				break;
		}
		outtextxy(300,100,s);
	  	outtextxy(0,800,"定时终止 ，按D键退出继续模拟");
	  	outtextxy(0,830,"左键选定我方执行星系  右键攻击/发送复制体的目标星系（自动识别）");
				Stars.mass = atof(table[OrbitSN][4]);
				if(Stars.mass == 0.00)////回调
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
		delay_fps(50);/// 有且仅有在进行此类Delay代码的时候才能刷新 ————大幅提高软件的帧率
		////因为仅在这种情况下才可以让帧率稳定  因此，之前的那个“ottextxy”不能换的根源反而是它拖累了程序. 
		}
   }
    closegraph(); // 关闭图形界面 
	exit(-1); //闪退更快 
} 

//计算公式
 

//学习资料 
//https://blog.csdn.net/cnds123/article/details/109982100 
//https://zhuanlan.zhihu.com/p/29818711
//https://blog.csdn.net/TYPYDEF/article/details/77926605
//https://blog.csdn.net/qq_39151563/article/details/104342349
//https://blog.csdn.net/agoniangel/article/details/104631806 
//https://blog.csdn.net/qq_39151563/article/details/104342730(鼠标） 
