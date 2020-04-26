package com.bn.Sample6_4;
import java.util.ArrayList;

//代表粒子系统的类
public class GrainGroup {
	static GrainForDraw gfd;						//用于绘制粒子的绘制者
	ArrayList<SingleGrain> al=new ArrayList<SingleGrain>();			//所有粒子的列表
 	static final float SPEED_SPAN= (float) (1.5f+1.5f*Math.random());	//粒子初速度
	static final float SPEED=0.02f;		//粒子移动每一步的模拟时延，也就是时间间隔
	public GrainGroup(MySurfaceView mv){
    		gfd=new GrainForDraw(2,1,1,1,mv);		//创建粒子的绘制者
    		for(int i=0;i<400;i++){			//随机向列表中添加不同初速度的粒子
    			double elevation=0.35f*Math.random()*Math.PI+Math.PI*0.15f;	//仰角
    			double direction=Math.random()*Math.PI*2;		//方位角
    			float vy=(float)(SPEED_SPAN*Math.sin(elevation));//分解出3个轴的初速度
    			float vx=(float)(SPEED_SPAN*Math.cos(elevation)*Math.cos(direction));
    			float vz=(float)(SPEED_SPAN*Math.cos(elevation)*Math.sin(direction));
    			al.add(new SingleGrain(vx,vy,vz));	//创建粒子对像并添加进粒子列表
    		} 
	}
	
	long timeStamp=0;							//用于计算的时间戳
	public void drawSelf(){
		long currTimeStamp=System.nanoTime()/1000000;	//获取当前系统时间
		if(currTimeStamp-timeStamp>10){	//若时间间隔大于10ms，则各个粒子前进一步
			for(SingleGrain sp:al){		//扫描粒子列表，并修改粒子的累计运动时间
				sp.timeSpan=sp.timeSpan+SPEED;
				if(sp.timeSpan>10){		//判断粒子的累计运动时间是否大于10
					sp.timeSpan=0;		//将粒子的累计运动时间归零
				} 
			}
			timeStamp=currTimeStamp;		//更新用于计算的时间戳
		}
		int size=al.size();
		for(int i=0;i<size;i++){			//循环扫描所有粒子的列表并绘制各个粒子
			try{ al.get(i).drawSelf();}catch(Exception e){} //绘制粒子
		}
	}
}