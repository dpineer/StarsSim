#include "math.h"
#include "GPGL.h" 

//��������뾶  
float StarsRadius(double mass)  ///��ϲ��� 
{
	double radius;
	double Xdss,Rdss,Kdss;
	Xdss = pow(1.4,0.9);
	Rdss = pow(1.4,0.6);
	Kdss = Xdss/Rdss;//
	Kdss = 1.10621;
	if(mass <= 1.4)/////ע�⣬0.2-30.0��̫����������Ĳ����ǣ�0.9���µ����Ч������
	{
		radius = pow(mass,0.9);
	}
	else
	{
		Xdss = pow(mass,0.6);
		radius = Kdss * Xdss;
	}
	return radius;
}

//������������ 
float Starslum(double mass) 
{
	double lum;
	double XdSs,RdSs;
	if(mass <= 0.43)
	{
		//���̫���������� 
		 XdSs = pow(mass,2.3);
		 RdSs = XdSs*0.23;
		 //̫������Ϊ1.0��ֱ����� 
		 lum = RdSs;
	}
	else if(mass <= 2.0)
	{
		XdSs = pow(mass,4);
		lum  = XdSs;
	}
	else if(mass <= 20.0)
	{
		XdSs = pow(mass,3.5);
		RdSs = 1.5*XdSs;
		lum = RdSs;
	}
	else
	{
		lum = 3200.0*mass;
	}
	return lum;
}

///�������̫�������͹�ȼ����Ч�¶�
float StarsTemp(struct StarsSelf SSCP)   
{
	double LC4PK,R2SB;	
	double P4xA;
	P4xA = 4.0 *Pai;
	LC4PK = SSCP.lum * SunLight *E26;
	LC4PK = LC4PK / P4xA;
	R2SB = SSCP.radius * SunRadius;
	R2SB = pow(R2SB,2);
	R2SB = R2SB * StefanBoltzmannConst;
	LC4PK = LC4PK / R2SB;
	P4xA = pow(LC4PK,0.25);
	R2SB = P4xA;
	return R2SB;
}

//���Ǽʹⷫ����RKKV1 -�����ٶ�(0.2.0) 
//�����ٶȼ��㲨��-����
float LaunchBlueShift(double PersentofC)  
{
	double UPOC,DWNOC,CALUMC;
	UPOC = PersentofC;
	DWNOC = PersentofC; 
	UPOC += 1.0;  
	DWNOC += 1.0;
	CALUMC = UPOC/DWNOC;
	UPOC = pow(CALUMC,0.5);
	return UPOC;
}
//����Ŀ������ĩ�ٶ�������Ч�ʹ�ϵ 
float LaserEfficiency(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0-PersentofC;
	DWNOC = 1.0+PersentofC;
	CALUMC = UPOC/DWNOC;
	UPOC = 1.0-CALUMC;
	return UPOC;	
}
//ȫ������¹��������ͱ��������嶯�ܱ�ֵ����Ч�ʣ� 
float TotalLaserEfficiency(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	CALUMC = pow(PersentofC,2.0);
	UPOC = 1.0-CALUMC;
	DWNOC = pow(UPOC,0.5);
	UPOC = 2.0 * PersentofC;
	DWNOC += 1.0 + PersentofC;
	CALUMC = UPOC/DWNOC;
	return CALUMC;
}
//�ٶ����ɭ�����蹦��
float TargetVDemandDysonPower(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0+PersentofC;
	DWNOC = 2.0 / UPOC;
	DWNOC -= 1.0;
	CALUMC = 1.0/DWNOC;
	return CALUMC;
}
//���ٻ��ѹ������������� 
float LandingCostEnergy(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	DWNOC = 1.0+PersentofC;
	UPOC = 2/DWNOC;
	UPOC -= 1.0;
	DWNOC = pow(UPOC,0.5);
	UPOC = 1.0-DWNOC;
	CALUMC = 0.5 *UPOC;
	return CALUMC;
}
//���Ǽʹⷫ����RKKV1 -�ⷫ�뺽��(0.2.1) 
//float 
