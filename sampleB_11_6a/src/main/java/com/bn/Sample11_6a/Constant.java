package com.bn.Sample11_6a;

import android.opengl.Matrix;

public class Constant 
{
	public static final int WATER_WIDTH=127;//水面横向格子数
	public static final int WATER_HEIGHT=127;//水面纵向格子数
	public static final float WATER_UNIT_SIZE=1.5f;//水面格子尺寸
	
	//摄像机观察目标点，这里为水面大矩形的中心点
	public static final float tx=WATER_WIDTH*WATER_UNIT_SIZE/2.0f;
	public static final float ty=0;
	public static final float tz=WATER_HEIGHT*WATER_UNIT_SIZE/2.0f;
	//摄像机位置
	public static float cx,cy,cz;	
	//摄像机up向量
	public static float upx,upy,upz;
	//摄像机朝向
	public static float direction=0;
	//摄像机仰角
	public static float yj=45;
	//摄像机离目标点的距离
	public static float fsjl=(float)Math.sqrt((WATER_WIDTH*WATER_UNIT_SIZE)*(WATER_WIDTH*WATER_UNIT_SIZE)/4+
			                    (WATER_HEIGHT*WATER_UNIT_SIZE)*(WATER_HEIGHT*WATER_UNIT_SIZE)/4)*0.7f;
	
	//根据摄像机朝向、仰角等计算摄像机9参数
	public static void calCamera()
	{
		//虚拟摄像机位置点
		float cameraV[]={0,0,fsjl,1};
		//摄像机向量变化矩阵
		float[] m=new float[16];
		Matrix.setIdentityM(m,0);
		//方位角旋转
		Matrix.rotateM(m,0,direction,0,1,0);
		//仰角旋转
		Matrix.rotateM(m,0,-yj,1,0,0);
		//计算出虚拟摄像机观察向量点结果位置
		float[] cameraVResult=new float[4];
		Matrix.multiplyMV(cameraVResult,0,m,0,cameraV,0);
	    //计算出up向量
		float up[]={0,1,0,1};
		float[] upResult=new float[4];
		Matrix.multiplyMV(upResult,0,m,0,up,0);
		//设置结果摄像机
		cx=tx+cameraVResult[0];
		cy=ty+cameraVResult[1];
		cz=tz+cameraVResult[2];
		upx=upResult[0];
		upy=upResult[1];
		upz=upResult[2];		
	}
	
	//计算一个波对指定点的高度扰动
	public static float calHdr(
		float[] bx,	//波心
		float bc,	//波长
		float zf,	//振幅
		float qsj,	//起始角
		float[] ddxz	//被扰动的顶点xz坐标
	){
	    //计算与波心的距离
	    float dis=distance(ddxz,bx);
	    //算出角度跨度
	    float angleSpan=dis*2*3.1415926f/bc;
	    //计算此波对此顶点的振幅扰动
	    float hrd=(float) (Math.sin(angleSpan+qsj)*zf);
	    //返回高度扰动值
	    return hrd;
	}
	
	public static float distance(float[] ddxz,float[] bx)
	{
		float result=(float) Math.sqrt((ddxz[0]-bx[0])*(ddxz[0]-bx[0])+(ddxz[1]-bx[1])*(ddxz[1]-bx[1]));
		return result;
	}
	
	//求两个向量的叉积
	public static float[] getCrossProduct(float x1,float y1,float z1,float x2,float y2,float z2)
	{		
		//求出两个矢量叉积矢量在XYZ轴的分量ABC
	    float A=y1*z2-y2*z1;
	    float B=z1*x2-z2*x1;
	    float C=x1*y2-x2*y1;
			
	    return new float[]{A,B,C};
	}
		
	//向量规格化
	public static float[] vectorNormal(float[] vector)
	{
		//求向量的模
		float module=(float)Math.sqrt(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
		return new float[]{vector[0]/module,vector[1]/module,vector[2]/module};
	}
}