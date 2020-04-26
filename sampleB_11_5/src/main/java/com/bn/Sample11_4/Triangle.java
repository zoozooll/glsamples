package com.bn.Sample11_4;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.GLES30;
import android.opengl.Matrix;

//三角形
public class Triangle
{
	public static float[] mProjMatrix = new float[16];//4x4矩阵 投影用
    public static float[] mVMatrix = new float[16];//摄像机位置朝向9参数矩阵
    public static float[] mMVPMatrix;//最后起作用的总变换矩阵
	
	int mProgram;//自定义渲染管线程序id
    int muMVPMatrixHandle;//总变换矩阵引用id
    int maPositionHandle; //顶点位置属性引用id  
    int maColorHandle; //顶点颜色属性引用id  
    static float[] mMMatrix = new float[16];//具体物体的移动旋转矩阵，旋转、平移
	
	FloatBuffer   mVertexBuffer;//顶点坐标数据缓冲
	FloatBuffer   mColorBuffer;//顶点着色数据缓冲
    int vCount=0;	
    float xAngle=0;//绕x轴旋转的角度
    public Triangle(MyTDView mv)
    {    	
    	//初始化顶点坐标与着色数据
    	initVertexData();
    	//初始化shader
    	initShader(mv);
    }
   
    public void initVertexData()
    {
    	//顶点坐标数据的初始化
        vCount=3;  
        final float UNIT_SIZE=0.2f;
        float vertices[]=new float[]
        {
        	-4*UNIT_SIZE,0,
        	0,0,-4*UNIT_SIZE,
        	0,4*UNIT_SIZE,0,0
        };
		
        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);
        vbb.order(ByteOrder.nativeOrder());
        mVertexBuffer = vbb.asFloatBuffer();
        mVertexBuffer.put(vertices);
        mVertexBuffer.position(0);
        
        float colors[]=new float[]
        {
        		1,1,0,0,
        		0,1,1,0,
        		1,0,1,0
        };
        
        ByteBuffer cbb = ByteBuffer.allocateDirect(colors.length*4);
        cbb.order(ByteOrder.nativeOrder());
        mColorBuffer = cbb.asFloatBuffer();
        mColorBuffer.put(colors);
        mColorBuffer.position(0);
    }

    //初始化shader
    public void initShader(MyTDView mv)
    {
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil.createProgram("shader.bin",mv.getResources());
        //获取程序中顶点位置属性引用id  
        maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点颜色属性引用id  
        maColorHandle= GLES30.glGetAttribLocation(mProgram, "aColor");
        //获取程序中总变换矩阵引用id
        muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");  
    }
    
    public void drawSelf()
    {        
    	 //制定使用某套shader程序
    	 GLES30.glUseProgram(mProgram);        
    	 //初始化变换矩阵
         Matrix.setRotateM(mMMatrix,0,0,0,1,0);
         //设置沿Z轴正向位移1
         Matrix.translateM(mMMatrix,0,0,0,1);
         //设置绕x轴旋转
         Matrix.rotateM(mMMatrix,0,xAngle,1,0,0);
         //
         GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, Triangle.getFianlMatrix(mMMatrix), 0); 
         //为画笔指定顶点位置数据
         GLES30.glVertexAttribPointer(
         		maPositionHandle,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer
         );
         GLES30.glVertexAttribPointer  
         (
        		maColorHandle,
         		4,
         		GLES30.GL_FLOAT,
         		false,
                4*4,
                mColorBuffer
         );
         //允许顶点位置数据数组
         GLES30.glEnableVertexAttribArray(maPositionHandle);  
         GLES30.glEnableVertexAttribArray(maColorHandle);  
         //绘制三角形
         GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount); 
    }
    public static float[] getFianlMatrix(float[] spec)
    {
    	mMVPMatrix=new float[16];
    	Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, spec, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);        
        return mMVPMatrix;
    }
}