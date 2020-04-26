package com.bn.Sample9_2;
public class Robot 
{
	//机器人的各个部件
	BodyPart bRoot,bBody,bHead,bLeftTop,bLeftBottom,bRightTop,bRightBottom,
		bRightLegTop,bRightLegBottom,bLeftLegTop,bLeftLegBottom,bLeftFoot,bRightFoot;
	BodyPart[] bpArray;
		//最低点位置
	float lowest=Float.MAX_VALUE;	
	//用于绘制的最低点位置
	float lowestForDraw;		
	//用于绘制的最终矩阵数组
	float[][] fianlMatrixForDrawArray;
	//用于绘制的临时矩阵数组
	float[][] fianlMatrixForDrawArrayTemp;  
	//用于绘制的临时高度调整值
	float lowestForDrawTemp;
	//绘制数据锁
	Object lock=new Object();
	public void calLowest()
	{
		lowest=Float.MAX_VALUE;
		bRoot.calLowest();
	}
	public Robot(LoadedObjectVertexNormalTexture[] lovntArray,MySurfaceView msv)
	{
		bRoot=new BodyPart(0,0,0,null,0,msv,this);//
        bBody=new BodyPart(0.0f,0.938f,0.0f,lovntArray[0],1,msv,this);//不动点坐标（x,z,y）
        bHead=new BodyPart(0.0f,1.00f,0.0f,lovntArray[1],2,msv,this);
        bLeftTop=new BodyPart(0.107f,0.938f,0.0f,lovntArray[2],3,msv,this);
        bLeftBottom=new BodyPart(0.105f,0.707f,-0.033f,lovntArray[3],4,msv,this);
        bRightTop=new BodyPart(-0.107f,0.938f,0.0f,lovntArray[4],5,msv,this);
        bRightBottom=new BodyPart(-0.105f,0.707f,-0.033f,lovntArray[5],6,msv,this);
        bRightLegTop=new BodyPart(-0.068f,0.6f,0.02f,lovntArray[6],7,msv,this);//y，z改了
        bRightLegBottom=new BodyPart(-0.056f,0.312f,0f,lovntArray[7],8,msv,this);
        bLeftLegTop=new BodyPart(0.068f,0.6f,0.02f,lovntArray[8],9,msv,this);
        bLeftLegBottom=new BodyPart(0.056f,0.312f,0f,lovntArray[9],10,msv,this);
        bLeftFoot=new BodyPart
        (
        	0.068f,0.038f,0.033f,
        	lovntArray[10],
        	11,        	
        	true,
        	new float[][]{{0.068f,0.0f,0.113f},{0.068f,0f,-0.053f}},//注意3DMAX里的Y坐标是3D程序世界里的-Z坐标
        	msv,													//所以要注意正负问题
        	this
        );
        bRightFoot=new BodyPart
        (
        	-0.068f,0.038f,0.033f,
        	lovntArray[11],
        	12,
        	true,
        	new float[][]{{-0.068f,0.0f,0.113f},{-0.068f,0f,-0.053f}},
        	msv,
        	this
        );
        //所有的骨骼列表
        bpArray=new BodyPart[]
		{
        		bRoot,bBody,bHead,bLeftTop,bLeftBottom,bRightTop,bRightBottom,
        		bRightLegTop,bRightLegBottom,bLeftLegTop,bLeftLegBottom,bLeftFoot,bRightFoot
		};
        //每个骨骼一个矩阵
        fianlMatrixForDrawArray=new float[bpArray.length][16];    
        fianlMatrixForDrawArrayTemp=new float[bpArray.length][16];  
        bRoot.addChild(bBody);
        bBody.addChild(bHead);
        bBody.addChild(bLeftTop);
        bBody.addChild(bRightTop);
        bLeftTop.addChild(bLeftBottom);
        bRightTop.addChild(bRightBottom);
        bBody.addChild(bRightLegTop);
        bRightLegTop.addChild(bRightLegBottom);
        bBody.addChild(bLeftLegTop);
        bLeftLegTop.addChild(bLeftLegBottom);
        bLeftLegBottom.addChild(bLeftFoot);
        bRightLegBottom.addChild(bRightFoot);
        //级联计算每个子骨骼在父骨骼坐标系中的原始坐标，并且将平移信息记录进矩阵
        bRoot.initFatherMatrix();
        //层次级联更新骨骼矩阵的方法真实的平移信息，相对于世界坐标系
        bRoot.updateBone();
        //层次级联计算子骨骼初始情况下在世界坐标系中的变换矩阵的逆矩阵
        bRoot.calMWorldInitInver();
	}  
	//在线程中调用此方法  
	public void updateState()
	{
		bRoot.updateBone();
	}	
	//在线程中调用此方法
	public void backToInit()
	{
		bRoot.backToIInit();
	}	
	//在线程中调用此方法
	public void flushDrawData()
	{
		//加锁将主数据拷贝进绘制数据
		synchronized(lock)
		{
			for(BodyPart bp:bpArray)
			{
				bp.copyMatrixForDraw();
			}
			lowestForDraw=lowest;
		}
	}
	public void drawSelf()
	{	
		//绘制前将绘制数据拷贝进临时数据
		synchronized(lock)
		{
		     for(int i=0;i<bpArray.length;i++)
		     {
		    	 for(int j=0;j<16;j++)
		    	 {
		    		 fianlMatrixForDrawArrayTemp[i][j]=fianlMatrixForDrawArray[i][j];
		    	 }
		     }
		     lowestForDrawTemp=lowestForDraw;
		}
		MatrixState.pushMatrix();
		//进行落地调整
		MatrixState.translate(0, -lowestForDrawTemp, 0);
		//从根部件开始绘制
		bRoot.drawSelf(fianlMatrixForDrawArrayTemp);		
		MatrixState.popMatrix();
	}
}