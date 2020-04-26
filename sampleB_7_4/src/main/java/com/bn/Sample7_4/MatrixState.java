package com.bn.Sample7_4;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.Stack;

import android.opengl.Matrix;

//存储系统矩阵状态的类
public class MatrixState 
{
	private static float[] mProjMatrix = new float[16];//4x4矩阵 投影用
    static float[] mVMatrix = new float[16];//摄像机位置朝向9参数矩阵
    static float[] mMVPMatrix;//最后起作用的总变换矩阵
    public static float[] lightLocationRed=new float[]{0,0,0};//红色定位光光源位置
    public static float[] lightLocationGreenBlue=new float[]{0,0,0};//天蓝色定位光光源位置
    public static FloatBuffer cameraFB;
    public static FloatBuffer lightPositionFBRed;
    public static FloatBuffer lightPositionFBGreenBlue;
    
    //设置摄像机
    public static void setCamera
    (
    		float cx,	//摄像机位置x
    		float cy,   //摄像机位置y
    		float cz,   //摄像机位置z
    		float tx,   //摄像机目标点x
    		float ty,   //摄像机目标点y
    		float tz,   //摄像机目标点z
    		float upx,  //摄像机UP向量X分量
    		float upy,  //摄像机UP向量Y分量
    		float upz   //摄像机UP向量Z分量		
    )
    {
    	Matrix.setLookAtM
        (
        		mVMatrix, 
        		0, 
        		cx,
        		cy,
        		cz,
        		tx,
        		ty,
        		tz,
        		upx,
        		upy,
        		upz
        );
    	
    	float[] cameraLocation=new float[3];//摄像机位置
    	cameraLocation[0]=cx;
    	cameraLocation[1]=cy;
    	cameraLocation[2]=cz;
    	//摄像机位置矩阵
    	ByteBuffer llbb = ByteBuffer.allocateDirect(3*4);
        llbb.order(ByteOrder.nativeOrder());//设置字节顺序
        cameraFB=llbb.asFloatBuffer();
        cameraFB.put(cameraLocation);
        cameraFB.position(0);  
    }
    
    //设置透视投影参数
    public static void setProjectFrustum
    (
    	float left,		//near面的left
    	float right,    //near面的right
    	float bottom,   //near面的bottom
    	float top,      //near面的top
    	float near,		//near面距离
    	float far       //far面距离
    )
    {
    	Matrix.frustumM(mProjMatrix, 0, left, right, bottom, top, near, far);    	
    }
    
    //设置正交投影参数
    public static void setProjectOrtho
    (
    	float left,		//near面的left
    	float right,    //near面的right
    	float bottom,   //near面的bottom
    	float top,      //near面的top
    	float near,		//near面距离
    	float far       //far面距离
    )
    {    	
    	Matrix.orthoM(mProjMatrix, 0, left, right, bottom, top, near, far);
    }   
   
    //获取具体物体的总变换矩阵
    public static float[] getFinalMatrix()
    {
    	float[] mMVPMatrix=new float[16];
    	Matrix.multiplyMM(mMVPMatrix, 0, mVMatrix, 0, currMatrix, 0);
        Matrix.multiplyMM(mMVPMatrix, 0, mProjMatrix, 0, mMVPMatrix, 0);        
        return mMVPMatrix;
    }
    
    //设置红色灯光位置的方法
    public static void setLightLocationRed(float x,float y,float z)
    {
    	lightLocationRed[0]=x;
    	lightLocationRed[1]=y;
    	lightLocationRed[2]=z;
    	ByteBuffer llbb = ByteBuffer.allocateDirect(3*4);
        llbb.order(ByteOrder.nativeOrder());//设置字节顺序
        lightPositionFBRed=llbb.asFloatBuffer();
        lightPositionFBRed.put(lightLocationRed);
        lightPositionFBRed.position(0);
    }
    
    //设置天蓝色灯光位置的方法
    public static void setLightLocationGreenBlue(float x,float y,float z)
    {
    	lightLocationGreenBlue[0]=x;
    	lightLocationGreenBlue[1]=y;
    	lightLocationGreenBlue[2]=z;
    	ByteBuffer llbb = ByteBuffer.allocateDirect(3*4);
        llbb.order(ByteOrder.nativeOrder());//设置字节顺序
        lightPositionFBGreenBlue=llbb.asFloatBuffer();
        lightPositionFBGreenBlue.put(lightLocationGreenBlue);
        lightPositionFBGreenBlue.position(0);
    }
    
    public static Stack<float[]> mStack=new Stack<float[]>();//保护变换矩阵的栈
    static float[] currMatrix;//当前变换矩阵
 
    public static void setInitStack()//获取不变换初始矩阵
    {
    	currMatrix=new float[16];
    	Matrix.setRotateM(currMatrix, 0, 0, 1, 0, 0);
    }
    
    public static void pushMatrix()//保护变换矩阵
    {
    	mStack.push(currMatrix.clone());
    }
    
    public static void popMatrix()//恢复变换矩阵
    {
    	currMatrix=mStack.pop();
    }
    
    public static void translate(float x,float y,float z)//设置沿xyz轴移动
    {
    	Matrix.translateM(currMatrix, 0, x, y, z);
    }
    
    public static void rotate(float angle,float x,float y,float z)//设置绕xyz轴移动
    {
    	Matrix.rotateM(currMatrix,0,angle,x,y,z);
    }
    
    public static void scale(float x,float y,float z){
    	Matrix.scaleM(currMatrix, 0, x, y, z);
    }
    
    //插入自带矩阵
    public static void matrix(float[] self)
    {
    	float[] result=new float[16];
    	Matrix.multiplyMM(result,0,currMatrix,0,self,0);
    	currMatrix=result;
    }

	public static float[] getMMatrix() {
		return currMatrix;
	}
}