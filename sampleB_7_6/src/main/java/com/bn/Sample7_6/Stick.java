package com.bn.Sample7_6;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;

import javax.vecmath.Quat4f;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.linearmath.Transform;

import android.opengl.GLES30;
import static com.bn.Sample7_6.Constant.*;
//默认是横向X圆柱
public class Stick {
	int mProgram;//自定义渲染管线程序id 
    int muMVPMatrixHandle;//总变换矩阵引用id   
    int muMMatrixHandle;//位置、旋转变换矩阵
    int maCameraHandle; //摄像机位置属性引用id  
    int maPositionHandle; //顶点位置属性引用id  
    int maNormalHandle; //顶点法向量属性引用id  
    int maLightLocationHandle;//光源位置属性引用id  
        
    String mVertexShader;//顶点着色器    	 
    String mFragmentShader;//片元着色器
    static float[] mMMatrix = new float[16];//具体物体的移动旋转矩阵
	  
	FloatBuffer   mVertexBuffer;//顶点坐标数据缓冲
	FloatBuffer   mNormalBuffer;//顶点纹理坐标数据缓冲
	
    int vCount=0;    
	float length=10f;//圆柱长度
	float circle_radius=2f;//圆截环半径
	float degreespan=18f;  //圆截环每一份的度数大小
	
	RigidBody body;
	MySurfaceView mv;
    
    public Stick(MySurfaceView mv,float length,float circle_radius,float degreespan,float[] colorValue,
    		RigidBody body)
    {    	
    	//初始化顶点坐标与着色数据
    	this.mv=mv;
    	initVertexData( length, circle_radius, degreespan,colorValue);
    	//初始化shader        
    	intShader(mv);
    	this.body = body;
    } 
    
    //初始化顶点坐标与着色数据的方法
    public void initVertexData(float length,float circle_radius,float degreespan,float[] colorValue)
    {
    	//顶点坐标数据的初始化================begin============================
		ArrayList<Float> val=new ArrayList<Float>();//顶点存放列表
		ArrayList<Float> ial=new ArrayList<Float>();//法向量存放列表
    	
    	this.length = length; //半长
    	this.circle_radius = circle_radius;
    	this.degreespan = degreespan;
    	
    	for(float circle_degree=360.0f;circle_degree>0.0f;circle_degree-=degreespan)//循环行
		{
				float x1 =(float)(-length);
				float y1=(float) (circle_radius*Math.sin(Math.toRadians(circle_degree)));
				float z1=(float) (circle_radius*Math.cos(Math.toRadians(circle_degree)));
				
				float a1=0;
				float b1=y1;
				float c1=z1;
				float l1=getVectorLength(a1, b1, c1);//模长
				a1=a1/l1;//法向量规格化
				b1=b1/l1;
				c1=c1/l1;
				
				float x2 =(float)(-length);
				float y2=(float) (circle_radius*Math.sin(Math.toRadians(circle_degree-degreespan)));
				float z2=(float) (circle_radius*Math.cos(Math.toRadians(circle_degree-degreespan)));
				
				float a2=0;
				float b2=y2;
				float c2=z2;
				float l2=getVectorLength(a2, b2, c2);//模长
				a2=a2/l2;//法向量规格化
				b2=b2/l2;
				c2=c2/l2;
				
				float x3 =(float)(length);
				float y3=(float) (circle_radius*Math.sin(Math.toRadians(circle_degree-degreespan)));
				float z3=(float) (circle_radius*Math.cos(Math.toRadians(circle_degree-degreespan)));
				
				float a3=0;
				float b3=y3;
				float c3=z3;
				float l3=getVectorLength(a3, b3, c3);//模长
				a3=a3/l3;//法向量规格化
				b3=b3/l3;
				c3=c3/l3;
				
				float x4 =(float)(length);
				float y4=(float) (circle_radius*Math.sin(Math.toRadians(circle_degree)));
				float z4=(float) (circle_radius*Math.cos(Math.toRadians(circle_degree)));
				
				float a4=0;
				float b4=y4;
				float c4=z4;
				float l4=getVectorLength(a4, b4, c4);//模长
				a4=a4/l4;//法向量规格化
				b4=b4/l4;
				c4=c4/l4;
				
				val.add(x1);val.add(y1);val.add(z1);//两个三角形，共6个顶点的坐标
				val.add(x2);val.add(y2);val.add(z2);
				val.add(x4);val.add(y4);val.add(z4);
				
				val.add(x2);val.add(y2);val.add(z2);
				val.add(x3);val.add(y3);val.add(z3);
				val.add(x4);val.add(y4);val.add(z4);
				
				ial.add(a1);ial.add(b1);ial.add(c1);//顶点对应的法向量
				ial.add(a2);ial.add(b2);ial.add(c2);
				ial.add(a4);ial.add(b4);ial.add(c4);
				
				ial.add(a2);ial.add(b2);ial.add(c2);
				ial.add(a3);ial.add(b3);ial.add(c3);
				ial.add(a4);ial.add(b4);ial.add(c4);
		}
    	
    	vCount=val.size()/3;//顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
    	
        //将alVertix中的坐标值转存到一个float数组中
        float vertices[]=new float[vCount*3];
        for(int i=0;i<val.size();i++)
    	{
    		vertices[i]=val.get(i);
    	}
		  
        //创建顶点坐标数据缓冲
        //vertices.length*4是因为一个整数四个字节
        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer = vbb.asFloatBuffer();//转换为int型缓冲
        mVertexBuffer.put(vertices);//向缓冲区中放入顶点坐标数据
        mVertexBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        
        //将alVertix中的坐标值转存到一个float数组中
        float normals[]=new float[ial.size()];
    	for(int i=0;i<ial.size();i++)
    	{
    		normals[i]=ial.get(i);
    	}
		
        //创建顶点坐标数据缓冲
        //vertices.length*4是因为一个整数四个字节
        ByteBuffer nbb = ByteBuffer.allocateDirect(normals.length*4);
        nbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mNormalBuffer = nbb.asFloatBuffer();//转换为int型缓冲
        mNormalBuffer.put(normals);//向缓冲区中放入顶点坐标数据
        mNormalBuffer.position(0);//设置缓冲区起始位置
    }
  
    //初始化shader
    public void intShader(MySurfaceView mv)
    {
    	//加载顶点着色器的脚本内容       
        mVertexShader=ShaderUtil.loadFromAssetsFile("vertex_color.sh", mv.getResources());
        ShaderUtil.checkGlError("==ss==");   
        //加载片元着色器的脚本内容
        mFragmentShader=ShaderUtil.loadFromAssetsFile("frag_color.sh", mv.getResources());  
        //基于顶点着色器与片元着色器创建程序
        ShaderUtil.checkGlError("==ss==");      
        mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用id  
        maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中总变换矩阵引用id
        muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");   
        muMMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMMatrix");  
        //获取程序中顶点法向量属性引用  
        maNormalHandle= GLES30.glGetAttribLocation(mProgram, "aNormal");
        //获取程序中光源位置引用
        maLightLocationHandle=GLES30.glGetUniformLocation(mProgram, "uLightLocation");
        //获取程序中摄像机位置引用
        maCameraHandle=GLES30.glGetUniformLocation(mProgram, "uCamera");
    }
    
    public void drawSelf(float angle,float x,float y,float z) 
    {        
    	 //制定使用某套shader程序
    	MatrixState.pushMatrix();
    		MySurfaceView.init=false;
    		Transform trans = body.getMotionState().getWorldTransform(new Transform());
     		MatrixState.translate(trans.origin.x,trans.origin.y, trans.origin.z);
     		Quat4f ro=trans.getRotation(new Quat4f());
     		if(ro.x!=0||ro.y!=0||ro.z!=0)
     		{
     			float[] fa=SYSUtil.fromSYStoAXYZ(ro);
     			if(isNumber(fa[0]+"") && isNumber(fa[1]+"") && isNumber(fa[2]+"")){
     				
     	 			MatrixState.rotate(fa[0], fa[1], fa[2], fa[3]);
     			}
     		}
    	
    	GLES30.glUseProgram(mProgram); 
    	MatrixState.rotate(angle, x, y, z);
         //将最终变换矩阵传入shader程序
         GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0);
         //将位置、旋转变换矩阵传入着色器程序
         GLES30.glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState.currMatrix, 0);   
         //将光源位置传入着色器程序   
         GLES30.glUniform3fv(maLightLocationHandle, 1, MatrixState.lightPositionFBRed);
         //将摄像机位置传入着色器程序   
         GLES30.glUniform3fv(maCameraHandle, 1, MatrixState.cameraFB);
         
         //为画笔指定顶点位置数据    
         GLES30.glVertexAttribPointer        
         (
         		maPositionHandle,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4, 
                mVertexBuffer   
         );
         //为画笔指定顶点法向量数据
         GLES30.glVertexAttribPointer  
         (
        		maNormalHandle, 
         		4, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mNormalBuffer
         );            
         //允许顶点位置数据数组
         GLES30.glEnableVertexAttribArray(maPositionHandle);  
         GLES30.glEnableVertexAttribArray(maNormalHandle);  
         
         //绘制三角形
         GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount); 
         MatrixState.popMatrix();
    }
    
  //法向量规格化，求模长度
	public float getVectorLength(float x,float y,float z)
	{
		float pingfang=x*x+y*y+z*z;
		float length=(float) Math.sqrt(pingfang);
		return length;
	}
}