package com.bn.Sample10_2;//声明包

//用于记录触控点坐标、及绘制触控点的类
public class BNPoint   
{
	//上一次此触控点的x、y坐标
   float oldX;
   float oldY;	
 //是否已经存在上一次触控位置的标志位
   boolean hasOld=false;
 //触控点当前x、y坐标
   float x;   
   float y;    
   
   public BNPoint(float x,float y){//构造器
	 //初始化x、y坐标
	   this.x=x;
	   this.y=y;
   }
 //设置触控点新位置的方法
   public void setLocation(float x,float y)
   {
	 //把原来位置记录为旧位置
	   oldX=this.x;
	   oldY=this.y;
	 //设置是否已经存在上一次触控位置的标志位
	   hasOld=true;
	 //设置新位置
	   this.x=x;
	   this.y=y;
   }
   
 //计算两个触控点距离的方法
   public static float calDistance(BNPoint a,BNPoint b)
   {
	   float result=0;
	 //计算两个触控点的直线距离
	   result=(float)Math.sqrt(
	     (a.x-b.x)*(a.x-b.x)+
	     (a.y-b.y)*(a.y-b.y)
	   );	   
	   return result;
   }
}