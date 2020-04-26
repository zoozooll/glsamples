package com.bn.Sample9_1;
public class DoActionThread extends Thread{
    int currActionIndex=0;//当前动作编号
    int currStep=0;//当前的动作编号细节编号
    Action currAction;//当前动作数据
    Robot robot;
    MySurfaceView msv;    
    public DoActionThread(Robot robot,MySurfaceView msv){
    	this.robot=robot;
    	this.msv=msv;
    }    
    public void run(){
    	try{
			Thread.sleep(500);
		} 
    	catch (InterruptedException e){
			e.printStackTrace();
		}
    	currAction=ActionGenerator.acArray[currActionIndex];	//拿到当前的动作编号
    	while(true){
    		 robot.backToInit();//变成最原始的初始变化矩阵，
    		//如果此次动画播放完了，则进行下一组动画的播放
    		if(currStep>=currAction.totalStep){
    			//取余控制动作的编号在一定范围内
    			currActionIndex=(currActionIndex+1)%ActionGenerator.acArray.length;
    			currAction=ActionGenerator.acArray[currActionIndex];//重新获取当前的动作编号
    			currStep=0;//当前的动作编号细节编号，变为0;
    		}    
    		//将ActionGenerator中给出的动作数据进行分解
    		for(float[] ad:currAction.data){//修改数据
    			int partIndex=(int) ad[0];//部件索引
    			int aType=(int)ad[1]; //动作类型
    			if(aType==0){//若aType==0此部件动作为平移
    				float xStart=ad[2];//起始位置的x坐标
    				float yStart=ad[3];//起始位置的y坐标
    				float zStart=ad[4];//起始位置的z坐标
    				float xEnd=ad[5];//部件最终要到达位置的x坐标
    				float yEnd=ad[6];//部件最终要到达位置的y坐标
    				float zEnd=ad[7];//部件最终要到达位置的z坐标
    				//根据当前的动作编号细节编号，计算出当前的平移距离
    				float currX=xStart+(xEnd-xStart)*currStep/currAction.totalStep;
    				float currY=yStart+(yEnd-yStart)*currStep/currAction.totalStep;
    				float currZ=zStart+(zEnd-zStart)*currStep/currAction.totalStep;
    				//将当前部件的平移信息记录进变换矩阵
    				robot.bpArray[partIndex].transtate(currX, currY, currZ);
    			}
    			else if(aType==1){//若aType==0此部件动作为旋转
    				float startAngle=ad[2];//旋转的起始角度
    				float endAngle=ad[3];//旋转的介绍角度
    				//根据当前的动作编号细节编号出当前的旋转角度
    				float currAngle=startAngle+(endAngle-startAngle)*currStep/currAction.totalStep;
    				//此部件的旋转轴
    				float x=ad[4];
    				float y=ad[5];
    				float z=ad[6];
    				//将当前部件的旋转信息记录进变换矩阵
    				robot.bpArray[partIndex].rotate(currAngle, x, y, z);
    			}}    
    		robot.updateState();//层次级联更新骨骼矩阵的方法
    		//将最终矩阵内容拷贝进绘制用的最终矩阵
    		robot.flushDrawData();
    		currStep++;  //当前的动作编号细节编号+1  		
    		try{
				Thread.sleep(30);
			}
    		catch (InterruptedException e){
				e.printStackTrace();
    		}}}}