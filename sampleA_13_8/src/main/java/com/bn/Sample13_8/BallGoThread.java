package com.bn.Sample13_8;
/*
 * 控制球运动的线程
 */
public class BallGoThread extends Thread {
	BallForControl ballForControl;//声明AllBalls的引用
	int timeSpan=12;
	private boolean flag=false;//循环标志位
	
	public BallGoThread(BallForControl ballForControl)//构造器
	{
		this.ballForControl=ballForControl;//成员变量赋值
	}
	
	@Override
	public void run()//重写run方法
	{
		while(flag)//while循环
		{	
			ballForControl.go();//调用使所有球运动的方法
			
			try{
				Thread.sleep(timeSpan);//一段时间后再运动
			}
			catch(Exception e){
				e.printStackTrace();//打印异常
			}
		}		
	}
	public void setFlag(boolean flag) {
		this.flag = flag;
	}
}