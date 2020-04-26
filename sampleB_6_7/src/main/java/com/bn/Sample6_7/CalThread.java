package com.bn.Sample6_7;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import com.bn.util.Vector3f;

public class CalThread extends Thread
{
    boolean flag=true;
    ParticleControl pc;
    Vector3f ballP;
    FloatBuffer mVertexBuffer;
    float[] vdata;
    
    public CalThread(ParticleControl pc)
    {
    	this.pc=pc;
    }
    
    public void run()
    {
    	while(flag)
    	{
    		synchronized(Constant.lockB)
    		{
    		pc.stepSimulation(0.010f);//进行物理计算
    		}
    		vdata=pc.getVerties();//获得一帧数据
    		ballP = pc.getBall();
            ByteBuffer vbb = ByteBuffer.allocateDirect(vdata.length*4);
            vbb.order(ByteOrder.nativeOrder());//设置字节顺序
            mVertexBuffer = vbb.asFloatBuffer();//转换为int型缓冲
            mVertexBuffer.put(vdata);//向缓冲区中放入顶点坐标数据
            mVertexBuffer.position(0);//设置缓冲区起始位置
    		synchronized(Constant.lockA)
    		{
    			Constant.mVertexBufferForFlag=mVertexBuffer;//将顶点缓冲引用指向新的数据
    			Constant.ballP = ballP;
    		}
    	}
    }
}
