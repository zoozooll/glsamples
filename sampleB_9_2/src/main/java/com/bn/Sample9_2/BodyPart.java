package com.bn.Sample9_2;
import java.util.ArrayList;
import android.opengl.Matrix;
//身体部件
public class BodyPart 
{	//绘制者
	LoadedObjectVertexNormalTexture lovnt;	
	//部件索引
	int index;  
	//对应的GLSurfaceView
	MySurfaceView msv;
	//此骨骼在父骨骼坐标系中的实时变换矩阵
	float[] mFather=new float[16];
	//此骨骼在世界坐标系中的实时变换矩阵
	float[] mWorld=new float[16];	
	//此骨骼在父骨骼坐标系中的初始变换矩阵
    float[] mFatherInit=new float[16];
	//此骨骼初始情况下在世界坐标系中的变换矩阵的逆矩阵
	float[] mWorldInitInver=new float[16];	
	//最终变换矩阵
	float[] finalMatrix=new float[16];
	//此骨骼不动点在世界坐标系中的原始坐标
	float fx,fy,fz;
    //此骨骼自己的子骨骼列表
    ArrayList<BodyPart> childs=new ArrayList<BodyPart>();
    //指向父骨骼的引用
    BodyPart father;
    //是否有最低控制点
    boolean lowestFlag=false;
    //最低控制点序列
    float[][] lowestDots;
    Robot robot;
    //构造器的入口参数
    //fx,fy,fz为子骨骼不动点在父坐标系中的坐标    
    public BodyPart(float fx,float fy,float fz,LoadedObjectVertexNormalTexture lovnt,int index,MySurfaceView msv,Robot robot)
    {    	
    	this.index=index;
    	this.msv=msv;    	
    	this.lovnt=lovnt;
    	this.fx=fx;
    	this.fy=fy;
    	this.fz=fz;    	    
    	this.robot=robot;
    }
    public BodyPart(float fx,float fy,float fz,LoadedObjectVertexNormalTexture lovnt,int index,
    		boolean lowestFlag,float[][] lowestDots,MySurfaceView msv,Robot robot)
    {    	
    	this.index=index;
    	this.msv=msv;    	
    	this.lovnt=lovnt;
    	this.fx=fx;
    	this.fy=fy;
    	this.fz=fz;    	 
    	this.lowestFlag=lowestFlag;
    	this.lowestDots=lowestDots;
    	this.robot=robot;
    }       
      //级联计算最低控制点
    public void calLowest()
    {
    	//先计算自己控制点的最低点
    	if(lowestFlag)
    	{
    		for(float[] p:lowestDots)
    		{
    			//计算变化后的坐标
    			float[] pqc={p[0],p[1],p[2],1};
    			float[] resultP={0,0,0,1};
    			Matrix.multiplyMV(resultP, 0, finalMatrix, 0, pqc, 0);
    			if(resultP[1]<robot.lowest)
    			{
    				robot.lowest=resultP[1];
    			}
    		}
    	}
    	
    	//计算所有的孩子
    	for(BodyPart bp:childs)
    	{
    		bp.calLowest();
    	}   
    }
     //将最终矩阵内容拷贝进绘制用的最终矩阵
    public void copyMatrixForDraw()
    {
    	for(int i=0;i<16;i++)
    	{
    		
    		robot.fianlMatrixForDrawArray[index][i]=finalMatrix[i];
    	}
    }
    
    public void drawSelf(float[][] tempMatrixArray)
    {
    	//如果自己的绘制者不为空，则绘制自己
    	if(lovnt!=null)
    	{
    		MatrixState.pushMatrix();
    		//插入新矩阵
        	MatrixState.setMatrix(tempMatrixArray[index]);        	
        	lovnt.drawSelf();
        	MatrixState.popMatrix();   
    	}
    	//绘制所有的孩子
    	for(BodyPart bp:childs)
    	{
    		bp.drawSelf(tempMatrixArray);
    	}    	
    }
     //级联计算每个子骨骼在父骨骼坐标系中的原始坐标
    public void initFatherMatrix()
    {
    	float tx=fx;
    	float ty=fy;
    	float tz=fz;    	
    	
    	if(father!=null)
    	{//若父骨骼不为空，则子骨骼在父骨骼坐标系中的原始坐标
    	 //为子骨骼在世界坐标系中的原始坐标减去父骨骼在在世界
    	 //坐标系中的原始坐标
    		tx=fx-father.fx;
    		ty=fy-father.fy;
    		tz=fz-father.fz;
    	}    	
    	//生成初始的此骨骼在父骨骼坐标系中的初始变换矩阵
    	Matrix.setIdentityM(mFather, 0);
    	Matrix.translateM(mFather, 0, tx, ty, tz);   
    	for(int i=0;i<16;i++)
    	{
    		mFatherInit[i]=mFather[i];
    	}
    	
    	//然后更新自己的所有孩子
    	for(BodyPart bc:childs)
    	{
    		bc.initFatherMatrix();
    	}
    }
       //层次级联计算子骨骼初始情况下在世界坐标系中的变换矩阵的逆矩阵
    public void calMWorldInitInver()
    {
    	Matrix.invertM(mWorldInitInver, 0, mWorld, 0);
    	//然后更新自己的所有孩子
    	for(BodyPart bc:childs)
    	{
    		bc.calMWorldInitInver();
    	}
    }
    public void updateBone(){//层次级联更新骨骼矩阵的方法
    	//首先根据父矩阵更新自己在世界坐标系中的变换矩阵
    	if(father!=null){
    	 //若父骨骼不为空则此骨骼在世界坐标系中的变换矩阵
         //为父骨骼在世界坐标系中的变换矩阵乘以自己在父骨骼
         //坐标系中的变换矩阵
    		Matrix.multiplyMM(mWorld, 0, father.mWorld, 0, mFather, 0);
    	}
    	else{
    		//若父骨骼为空，则此骨骼在世界坐标系中的变换矩阵
    		//为自己在父骨骼坐标系中的变换矩阵
    		for(int i=0;i<16;i++)
    		{
    			mWorld[i]=mFather[i];
    		}
    	}
    	calFinalMatrix();
    	
    	//然后更新自己的所有孩子
    	for(BodyPart bc:childs)
    	{
    		bc.updateBone();
    	}
    }
       //获取最终的绑定到此骨骼的顶点受此骨骼影响的从Mesh坐标系到骨骼动画后位置的最终变换矩阵
    public void calFinalMatrix()
    {
    	Matrix.multiplyMM(finalMatrix, 0, mWorld, 0, mWorldInitInver, 0);
    }
       //清除状态
    public void backToIInit()
    {
    	for(int i=0;i<16;i++)
    	{
    		mFather[i]=mFatherInit[i];
    	}
    	
    	//然后更新自己的所有孩子
    	for(BodyPart bc:childs)
    	{
    		bc.backToIInit();
    	}
    } 
        //在父坐标系中平移自己
    public void transtate(float x,float y,float z)//设置沿xyz轴移动
    {
    	Matrix.translateM(mFather, 0, x, y, z);
    }
        //在父坐标系中旋转自己
    public void rotate(float angle,float x,float y,float z)//设置绕xyz轴转动
    {
    	Matrix.rotateM(mFather,0,angle,x,y,z);
    }    
    //添加子骨骼
    public void addChild(BodyPart child)
    {
    	childs.add(child);
    	child.father=this;
    }
}