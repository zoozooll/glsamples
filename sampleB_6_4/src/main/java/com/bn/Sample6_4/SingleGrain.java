package com.bn.Sample6_4;
//存储粒子系统中的某个粒子物理信息的类
public class SingleGrain {
	float vx;//x轴速度分量
	float vy;//y轴速度分量
	float vz;//z轴速度分量
	float timeSpan=0;//粒子的累计运动时间
	
	public SingleGrain(float vx,float vy,float vz)
	{//初始化粒子在各个坐标轴上的速度
		this.vx=vx;
		this.vy=vy;
		this.vz=vz;		
	}
	
	public void drawSelf()
	{
		MatrixState.pushMatrix();	
		//根据当前时间计算出粒子在X轴、Y轴、Z轴上的坐标
		float x=vx*timeSpan;
		float z=vz*timeSpan;
		float y=vy*timeSpan-0.5f*timeSpan*timeSpan*1.5f+3.0f;		
		MatrixState.translate(x, y, z);
		//绘制粒子
		GrainGroup.gfd.drawSelf();
		MatrixState.popMatrix();
	}
}