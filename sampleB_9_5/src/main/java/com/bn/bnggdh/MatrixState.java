package com.bn.bnggdh;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.Matrix;

//存储系统矩阵状态的类
public class MatrixState {
	private static float[] mProjMatrix = new float[16];//用于投影的 4*4矩阵
	private static float[] mVMatrix = new float[16];//摄像机位置朝向9参数矩阵
	private static float[] currMatrix;//当前变换矩阵
	public static float[] lightLocation = new float[]{0,0,0};//光源位置
	public static FloatBuffer cameraFB;
	public static FloatBuffer lightPositionFB;
	
	//保护变换矩阵的栈
	static float[][] mStack = new float[10][16];
	static int stackTop = -1;
	
	//获取不变换的初始矩阵
	public static void setInitStack()
	{
		currMatrix = new float[16];
		Matrix.setRotateM(currMatrix, 0, 0, 1, 0, 0);
	}
	
	//保护变换矩阵
	public static void pushMatrix()
	{
		stackTop++;
		for(int i = 0; i < 16; i++)
		{
			mStack[stackTop][i] = currMatrix[i];
		}
	}
	
	//恢复变换矩阵
	public static void popMatrix()
	{
		for(int i = 0; i < 16; i++)
		{
			currMatrix[i] = mStack[stackTop][i];
		}
		stackTop--;
	}
	
	//设置沿xyz轴移动
	public static void translate(float x, float y, float z)
	{
		Matrix.translateM(currMatrix, 0, x, y, z);
	}
	
	//设置旋转
	public static void rotate(float angle, float x, float y, float z)
	{
		Matrix.rotateM(currMatrix, 0, angle, x, y, z);
	}
	
	//设置缩放
	public static void scale(float x, float y, float z)
	{
		Matrix.scaleM(currMatrix, 0, x, y, z);
	}
	
	//插入自带矩阵
	public static void matrix(float[] self)
	{
		float[] result = new float[16];
		Matrix.multiplyMM(result, 0, currMatrix, 0, self, 0);//self矩阵插入currMatrix矩阵
		currMatrix = result;
	}
	
	//设置摄像机
	static ByteBuffer llbb = ByteBuffer.allocateDirect(3*4);//字节缓冲器
	static float[] cameraLocation = new float[3];//摄像机位置
	public static void setCamera
	(
		float cx,
		float cy,
		float cz,
		float tx,
		float ty,
		float tz,
		float upx,
		float upy,
		float upz
	)
	{
		Matrix.setLookAtM(mVMatrix, 0, cx, cy, cz, tx, ty, tz, upx, upy, upz);
		
		cameraLocation[0] = cx;
		cameraLocation[1] = cy;
		cameraLocation[2] = cz;
		
		llbb.clear();
		llbb.order(ByteOrder.nativeOrder());//设置字节顺序
		cameraFB = llbb.asFloatBuffer();//通过字节缓冲器 来 创建float类型缓冲器
		cameraFB.put(cameraLocation);//将摄像机数组放入缓冲器中
		cameraFB.position(0);//设置缓冲器起始位置
	}
	
	//设置透视投影的参数
	public static void setProjectFrustum
	(
		float left,//near面的left
		float right,//near面的right
		float bottom,//near面的bottom
		float top,//near面的top
		float near,//near面的距离
		float far//far面距离
	)
	{
		Matrix.frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);
	}
	
	//设置正交投影参数
	public static void setProjectOrtho
	(
		float left,//near面的left
		float right,//near面的right
		float bottom,//near面的bottom
		float top,//near面的top
		float near,//near面的距离
		float far//far面距离	
	)
	{
		Matrix.orthoM(mProjMatrix, 0, left, right, bottom, top, near, far);
	}
	
	//用于一帧内的摄像机矩阵
	private static float[] mVMatrixForSpecFrame = new float[16];
	public static float[] copyMVMatrix()
	{
		for(int i = 0; i < 16; i++)
		{
			mVMatrixForSpecFrame[i] = mVMatrix[i];
		}
		return mVMatrixForSpecFrame;
	}
	
	//获取具体物体的总变换矩阵
	static float[] mMVPMatrix = new float[16];
	public static float[] getFinalMatrix()
	{
		//当前摄像机矩阵mVMatrix  插入 当前变换矩阵currMatrix   插入 当前投影矩阵mProjMatrix , 三者结合结果就是   具体物体的总变换矩阵  
		//当前摄像机矩阵 与 当前矩阵  进行  multiplyMM操作  赋值给  mMVPMatrix矩阵
		//Matrix.multiplyMM(mMVPMatrix, 0, mVMatrixForSpecFrame, 0, currMatrix, 0);
		Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0);
		//mMVPMatrix矩阵 与 当前投影矩阵  进行 multiplyMM操作  赋值给 mMVPMatrix矩阵
		Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);
		return mMVPMatrix;
	}
	
	//获取具体物体的变换矩阵
	public static float[] getMMatrix()
	{
		return currMatrix;
	}
	
	//设置灯光位置的方法
	static ByteBuffer llbbL = ByteBuffer.allocateDirect(3*4);
	public static void setLightLocation(float x, float y, float z)
	{
		llbbL.clear();
		
		lightLocation[0] = x;
		lightLocation[1] = y;
		lightLocation[2] = z;
		
		llbbL.order(ByteOrder.nativeOrder());//设置字节的顺序
		lightPositionFB = llbbL.asFloatBuffer();//通过字节缓冲器 创建 float类型的缓冲器
		lightPositionFB.put(lightLocation);//放入数据
		lightPositionFB.position(0);//设置起始位置
	}
}