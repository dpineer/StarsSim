#include "math.h"
#include "GPGL.h" 

//质量计算半径  
float StarsRadius(double mass)  ///拟合参数 
{
	double radius;
	double Xdss,Rdss,Kdss;
	Xdss = pow(1.4,0.9);
	Rdss = pow(1.4,0.6);
	Kdss = Xdss/Rdss;//
	Kdss = 1.10621;
	if(mass <= 1.4)/////注意，0.2-30.0个太阳质量以外的不考虑，0.9以下的拟合效果更好
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

//质量计算亮度 
float Starslum(double mass) 
{
	double lum;
	double XdSs,RdSs;
	if(mass <= 0.43)
	{
		//相对太阳质量比例 
		 XdSs = pow(mass,2.3);
		 RdSs = XdSs*0.23;
		 //太阳亮度为1.0故直接输出 
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

///根据相对太阳质量和光度计算等效温度
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

//恒星际光帆船与RKKV1 -功与速度(0.2.0) 
//根据速度计算波长-蓝移
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
//计算目标物体末速度与能量效率关系 
float LaserEfficiency(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0-PersentofC;
	DWNOC = 1.0+PersentofC;
	CALUMC = UPOC/DWNOC;
	UPOC = 1.0-CALUMC;
	return UPOC;	
}
//全程情况下光子能量和被加速物体动能比值（总效率） 
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
//速度与戴森球所需功率
float TargetVDemandDysonPower(double PersentofC)
{
	double UPOC,DWNOC,CALUMC;
	UPOC = 1.0+PersentofC;
	DWNOC = 2.0 / UPOC;
	DWNOC -= 1.0;
	CALUMC = 1.0/DWNOC;
	return CALUMC;
}
//减速花费光束总能量计算 
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
//恒星际光帆船与RKKV1 -光帆与航行(0.2.1) 
//float 
