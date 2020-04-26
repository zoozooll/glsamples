package com.bn.Sample10_3;//声明包

import static com.bn.Sample10_3.MySurfaceView.*;
import static com.bn.Sample10_3.Sample10_3Activity.*;

public class KeyThread extends Thread
{
	//键盘状态  1-up 2-down 4-left 8-right
    int keyState=0;//存储各比特位的int型变量
	boolean flag=true;//线程循环标志
	@Override
	public void run()
	{
		while(flag)//线程不断循环定时读取各比特位值
		{
			if((keyState&0x1)!=0) 
			{//茶壶左移动作对应比特
				xOffset-=0.3f;
			}
			else if((keyState&0x2)!=0)
			{//茶壶右移动作对应比特
				xOffset+=0.3f;
			}
			if((keyState&0x4)!=0)
			{//逆时针旋转动作对应比特
				yAngle+=2.5f;
			}
			else if((keyState&0x8)!=0)
			{//顺时针旋转动作对应比特
				yAngle-=2.5f;
			}
			if(yAngle>=360||yAngle<=-360)
			{//如果旋转角度绝对值大于360，则置零
				yAngle=0;
			}
			try
			{
				Thread.sleep(30);
			}
			catch (InterruptedException e)
			{
				e.printStackTrace();
			}
		}
	}
	//检测按下触控点所在区域并置对应比特位值为1
	public void keyPress(float x,float y)
	{
		if(x>=0&&x<screenWidth/2&&y>=0&&y<screenHeight/2)
		{//向前
			keyState=keyState|0x1;//触控点在茶壶左移区域
		}
		else if(x>=screenWidth/2&&x<screenWidth&&y>=0&&y<screenHeight/2)
		{//向后
			keyState=keyState|0x2;//触控点在茶壶右移区域
		}
		else if(x>=0&&x<screenWidth/2&&y>=screenHeight/2&&y<screenHeight)
		{//向左
			keyState=keyState|0x4;//触控点在逆时针旋转区域
		}
		else if(x>=screenWidth/2&&x<=screenWidth&&y>=screenHeight/2&&y<=screenHeight)
		{//向右
			keyState=keyState|0x8;//触控点在顺时针旋转区域
		}
	}
	//检测抬起触控点所在区域并置对应比特位值为0
	public void keyUp(float x,float y)
	{
		if(x>=0&&x<screenWidth/2&&y>=0&&y<screenHeight/2)
		{//向前
			keyState=keyState&0xE;//触控点在茶壶左移区域
		}
		else if(x>=screenWidth/2&&x<screenWidth&&y>=0&&y<screenHeight/2)
		{//向后
			keyState=keyState&0xD;//触控点在茶壶右移区域
		}
		else if(x>=0&&x<screenWidth/2&&y>=screenHeight/2&&y<screenHeight)
		{//向左
			keyState=keyState&0xB;//触控点在逆时针旋转区域
		}
		else if(x>=screenWidth/2&&x<=screenWidth&&y>=screenHeight/2&&y<=screenHeight)
		{//向右
			keyState=keyState&0x7;//触控点在顺时针旋转区域
		}
	}
	//所有触控点均抬起时调用的方法，即将keyState值置为0
	public void clearKeyState()
	{
		keyState=keyState&0x0;
	}
}