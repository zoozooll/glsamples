package com.bn.Sample10_3;//声明包

//用于记录触控点坐标、及绘制触控点的类
public class BNPoint
{
	//触控点x、y坐标
	float x;
	float y;
	public BNPoint(float x,float y){//有参构造器
		//初始化触控点坐标
		this.x=x;
		this.y=y;
	}
}