package com.bn.Sample9_2;

public class DoActionThread extends Thread
{
    int currActionIndex=0;
    int currStep=0;
    Action currAction;
    Robot robot;
    MySurfaceView msv;
    
    public DoActionThread(Robot robot,MySurfaceView msv)
    {
    	this.robot=robot;
    	this.msv=msv;
    }
    
    public void run()
    {
    	try 
		{
			Thread.sleep(500);
		} catch (InterruptedException e) 
		{
			e.printStackTrace();
		}
    	//拿到当前的动作编号
    	currAction=ActionGenerator.acArray[currActionIndex];
    	while(true)
    	{
    		//变成最原始的初始变化矩阵，
    		 robot.backToInit();
    		//如果此次动画播放完了，则进行下一组动画的播放
    		if(currStep>=currAction.totalStep)
    		{
    			//取余控制动作的编号在一定范围内
    			currActionIndex=(currActionIndex+1)%ActionGenerator.acArray.length;
    			//重新获取当前的动作编号
    			currAction=ActionGenerator.acArray[currActionIndex];
    			//当前的动作编号细节编号，变为0;
    			currStep=0;
    		}    		
    		//修改数据
    		for(float[] ad:currAction.data)
    		{
    			//部件索引
    			int partIndex=(int) ad[0];
    			//动作类型
    			int aType=(int)ad[1]; 
    			//当前步骤值
    			
    			if(aType==0)
    			{//平移
    				float xStart=ad[2];
    				float yStart=ad[3];
    				float zStart=ad[4];
    				
    				float xEnd=ad[5];
    				float yEnd=ad[6];
    				float zEnd=ad[7];
    				
    				float currX=xStart+(xEnd-xStart)*currStep/currAction.totalStep;
    				float currY=yStart+(yEnd-yStart)*currStep/currAction.totalStep;
    				float currZ=zStart+(zEnd-zStart)*currStep/currAction.totalStep;
    				
    				robot.bpArray[partIndex].transtate(currX, currY, currZ);
    			}
    			else if(aType==1)
    			{//旋转
    				float startAngle=ad[2];
    				float endAngle=ad[3];
    				float currAngle=startAngle+(endAngle-startAngle)*currStep/currAction.totalStep;
    				float x=ad[4];
    				float y=ad[5];
    				float z=ad[6];
    				robot.bpArray[partIndex].rotate(currAngle, x, y, z);
    			}
    		}    
    		//层次级联更新骨骼矩阵的方法
    		robot.updateState();
    		//级联计算最低控制点
    		robot.calLowest();
    		//将最终矩阵内容拷贝进绘制用的最终矩阵并且lowestForDraw重新赋值，赋值上述计算出来的 值
    		robot.flushDrawData();
    		//一帧动作完成换下一帧的动作
    		currStep++;    		
    		try 
    		{
				Thread.sleep(30);
			} catch (InterruptedException e) 
			{
				e.printStackTrace();
			}
    	}
    }
}