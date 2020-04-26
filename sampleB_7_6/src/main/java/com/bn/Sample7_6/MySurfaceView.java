package com.bn.Sample7_6;
import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import javax.vecmath.Vector3f;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.view.MotionEvent;
import com.bulletphysics.BulletGlobals;
import com.bulletphysics.collision.broadphase.AxisSweep3;
import com.bulletphysics.collision.dispatch.CollisionConfiguration;
import com.bulletphysics.collision.dispatch.CollisionDispatcher;
import com.bulletphysics.collision.dispatch.DefaultCollisionConfiguration;
import com.bulletphysics.collision.shapes.BoxShape;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.collision.shapes.CylinderShape;
import com.bulletphysics.collision.shapes.CylinderShapeX;
import com.bulletphysics.collision.shapes.CylinderShapeZ;
import com.bulletphysics.dynamics.DiscreteDynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.constraintsolver.SequentialImpulseConstraintSolver;
import com.bulletphysics.dynamics.constraintsolver.SliderConstraint;
import com.bulletphysics.linearmath.MatrixUtil;
import com.bulletphysics.linearmath.Transform;

import static com.bn.Sample7_6.Constant.*;

public class MySurfaceView extends GLSurfaceView {

	DiscreteDynamicsWorld dynamicsWorld;
	CollisionShape boxShape;
	CollisionShape stickShape;
	CollisionShape stickFBSliderShape;
	CollisionShape stickLRSliderShape;
	//刚体
	RigidBodyHelper cubeBody;
	RigidBodyHelper stickFBSliderBody;
	RigidBodyHelper stickLRFSliderBody;
	RigidBodyHelper stickLRNSliderBody;
	//添加滑动约束
	SliderConstraint sliderFB;
	SliderConstraint sliderLRF;//远端的横向轴
	SliderConstraint sliderLRN;//近端的横向轴
	SliderConstraint[] sliders=new SliderConstraint[3];
	static boolean sliding=false;
	static boolean init=true;
	int currIndex;
	MyRenderer renderer; 
    public static int keyState=0;
	boolean flag=true;
	
	float screenWidth;
	float screenHeight;
	float buttonPixels;
	
	public MySurfaceView(Context context) {
		super(context);
		this.setEGLContextClientVersion(3);
		initWorld();
		renderer = new MyRenderer();
		this.setRenderer(renderer);
		this.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	}
	 @Override 
	    public boolean onTouchEvent(MotionEvent e) {
	        float y = e.getY();
	        float x = e.getX();
	        switch (e.getAction()) {	       
	        case MotionEvent.ACTION_DOWN:
	        	handleArrowDown(x,y);
	        	break;
	        case MotionEvent.ACTION_UP:
	        	handleArrowUp(x,y);
	        	break;
	        }
	        return true; 
	    }
	 public void handleArrowDown(float x,float y){
		 float buttonCenterX=screenWidth-buttonPixels/2;
		 float buttonCenterY=screenHeight-buttonPixels/2;
		 float baseUnit=buttonPixels/4;
		 
		 float upXMax=baseUnit+buttonCenterX;
		 float upXMin=-baseUnit+buttonCenterX;
		 float upYMin=-2*baseUnit+buttonCenterY;
		 float upYMax=-baseUnit+buttonCenterY;
		 
		 float downXMax=baseUnit+buttonCenterX;
		 float downXMin=-baseUnit+buttonCenterX;
		 float downYMax=2*baseUnit+buttonCenterY;
		 float downYMin=baseUnit+buttonCenterY;  
		 
		 float leftXMax=-baseUnit+buttonCenterX;
		 float leftXMin=-2*baseUnit+buttonCenterX;
		 float leftYMax=baseUnit+buttonCenterY;
		 float leftYMin=-baseUnit+buttonCenterY;  
		 
		 float rightXMax=2*baseUnit+buttonCenterX;
		 float rightXMin=baseUnit+buttonCenterX;
		 float rightYMax=baseUnit+buttonCenterY;
		 float rightYMin=-baseUnit+buttonCenterY;  
		 
		 if(upXMin<x && x<upXMax && upYMin<y && y<upYMax){
			 keyState=0x1;
		 }else if(rightXMin<x && x<rightXMax && rightYMin<y && y<rightYMax){
			 keyState=0x8;
		 }else if(downXMin<x && x<downXMax && downYMin<y && y<downYMax){
			 keyState=0x2;
		 }else if(leftXMin<x && x<leftXMax && leftYMin<y && y<leftYMax){
			 keyState=0x4;
		 }
	 }
	 public void handleArrowUp(float x,float y){ 
		 float buttonCenterX=screenWidth-buttonPixels/2;
		 float buttonCenterY=screenHeight-buttonPixels/2;
		 float baseUnit=buttonPixels/4;  
		 
		 float upXMax=baseUnit+buttonCenterX;
		 float upXMin=-baseUnit+buttonCenterX;
		 float upYMin=-2*baseUnit+buttonCenterY;
		 float upYMax=-baseUnit+buttonCenterY;
		 
		 float downXMax=baseUnit+buttonCenterX;  
		 float downXMin=-baseUnit+buttonCenterX;
		 float downYMax=2*baseUnit+buttonCenterY;
		 float downYMin=baseUnit+buttonCenterY;  
		 
		 float leftXMax=-baseUnit+buttonCenterX;
		 float leftXMin=-2*baseUnit+buttonCenterX;
		 float leftYMax=baseUnit+buttonCenterY;
		 float leftYMin=-baseUnit+buttonCenterY;  
		 
		 float rightXMax=2*baseUnit+buttonCenterX;
		 float rightXMin=baseUnit+buttonCenterX;
		 float rightYMax=baseUnit+buttonCenterY;
		 float rightYMin=-baseUnit+buttonCenterY;  
		 
		 if(upXMin<x && x<upXMax && upYMin<y && y<upYMax){
			 keyState=0;
			 stopSlide();
		 }else if(rightXMin<x && x<rightXMax && rightYMin<y && y<rightYMax){
			 keyState=0;
			 stopSlide();
		 }else if(downXMin<x && x<downXMax && downYMin<y && y<downYMax){
			 keyState=0;
			 stopSlide();
		 }else if(leftXMin<x && x<leftXMax && leftYMin<y && y<leftYMax){
			 keyState=0;
			 stopSlide();
		 }
	 }
	public void initWorld(){//初始化物理世界的方法
		CollisionConfiguration collisionConfiguration = new DefaultCollisionConfiguration();
		CollisionDispatcher dispatcher = new CollisionDispatcher(collisionConfiguration);
		Vector3f worldAabbMin = new Vector3f(-10000, -10000, -10000);
		Vector3f worldAabbMax = new Vector3f(10000, 10000, 10000);
		int maxProxies = 1024;
		AxisSweep3 overlappingPairCache =new AxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
		SequentialImpulseConstraintSolver solver = new SequentialImpulseConstraintSolver();
		dynamicsWorld = new DiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration);
		dynamicsWorld.setGravity(new Vector3f(0, -10, 0));
		boxShape=new BoxShape(new Vector3f(cubeSize,cubeSize,cubeSize));
		stickShape = new CylinderShape(new Vector3f(Stick_R,Stick_Length,Stick_R));//Y方向圆柱
		stickFBSliderShape = new CylinderShapeZ(new Vector3f(Stick_R,Stick_R,Stick_Length));//Z方向圆柱
		stickLRSliderShape = new CylinderShapeX(new Vector3f(Stick_Length,Stick_R,Stick_R));//X方向圆柱
		initRigidBody();//创建刚体
		
		Vector3f originA = new Vector3f(0, 0, 0);//从约束到竖杆质心的平移变换信息
		Vector3f originB = new Vector3f(0, 0, 0);//从约束到箱子质心的平移变换信息
		//调用方法添加滑动关节约束
		addSliderConstraint(0,stickFBSliderBody.body,cubeBody.body,BulletGlobals.SIMD_PI/2,originA,originB,true);//BulletGlobals.SIMD_PI/2
		//下面的代码添加竖杆与上侧横杆之间的滑动关节约束
		originA = new Vector3f(0,0,0);//从约束到上侧横杆质心的平移变换信息
		originB = new Vector3f(0, 0, -Stick_Length);//从约束到竖杆质心的平移变换信息
		//调用方法添加滑动关节约束
		addSliderConstraint(1,stickLRFSliderBody.body,stickFBSliderBody.body,0,originA,originB,true);
		//下面的代码添加竖杆与上侧横杆之间的滑动关节约束
		originA = new Vector3f(0,0,0);//从约束到下侧横杆质心的平移变换信息
		originB = new Vector3f(0, 0, Stick_Length);//从约束到竖杆质心的平移变换信息
		//调用方法添加滑动关节约束
		addSliderConstraint(2,stickLRNSliderBody.body,stickFBSliderBody.body,0,originA,originB,true);
	}
	public void initRigidBody(){ //创建刚体的方法
		//创建立方体刚体（对应于场景中的箱子）
		cubeBody = new RigidBodyHelper(boxShape,1f,dynamicsWorld, 0.0f, 0.8f, new Vector3f(0,Ceiling_Height-5,0),true);
		//创建圆柱刚体（对应于场景中的竖杆）
		stickFBSliderBody = new RigidBodyHelper(stickFBSliderShape, 1f, dynamicsWorld, 0.0f, 0.8f, new Vector3f(0,Ceiling_Height-5,0),true);
		//创建圆柱刚体（对应于上侧的横杆）
		stickLRFSliderBody = new RigidBodyHelper(stickLRSliderShape, 0, dynamicsWorld, 0.0f, 0.8f, new Vector3f(0,Ceiling_Height-5,-Stick_Length),true);
		//创建圆柱刚体（对应于下侧的横杆）
		stickLRNSliderBody = new RigidBodyHelper(stickLRSliderShape, 0, dynamicsWorld, 0.0f, 0.8f, new Vector3f(0,Ceiling_Height-5,Stick_Length),true);
	}
	public void addSliderConstraint(int index,RigidBody ra,RigidBody rb,float angle,Vector3f originA,Vector3f originB,boolean force){
		Transform localA = new Transform();//创建变换对象A（表示从变换到刚体ra质心的变换）
		Transform localB = new Transform();//创建变换对象B（表示从变换到刚体rb质心的变换）
		localA.setIdentity();//初始化变换对象A
		localB.setIdentity();//初始化变换对象B
		MatrixUtil.setEulerZYX(localA.basis, 0,angle, 0 );//设置变换对象A的旋转部分
		localA.origin.set(originA);//设置变换对象A的平移部分
		MatrixUtil.setEulerZYX(localB.basis, 0, angle, 0);//设置变换对象B的旋转部分
		localB.origin.set(originB);	//设置变换对象B的平移部分
		if(index==0){//若索引号为0，则添加箱子与场景中竖杆之间的滑动关节约束
			sliderFB = new SliderConstraint(ra, rb, localA, localB, force);//创建滑动关节约束对象
			//设置初始的limit
			sliderFB.setLowerLinLimit(-Stick_Length);//滑动的下限
			sliderFB.setUpperLinLimit(Stick_Length);//滑动的上限
			sliderFB.setLowerAngLimit(0);//转动的下限
			sliderFB.setUpperAngLimit(0);//转动的上限
			sliderFB.setDampingDirLin(0.05f); //设置滑动阻尼系数
			dynamicsWorld.addConstraint(sliderFB,true);//将约束添加进物理世界
			sliders[index]=sliderFB;//将约束放进约束数组
		}
		if(index==1){//若索引号为1，则添加竖杆与上侧横杆之间的滑动关节约束
			sliderLRF = new SliderConstraint(ra, rb, localA, localB, force);
			//设置初始的limit
			sliderLRF.setLowerLinLimit(-Stick_Length);//滑动的下限
			sliderLRF.setUpperLinLimit(Stick_Length);//滑动的上限
			sliderLRF.setLowerAngLimit(0);//转动的下限
			sliderLRF.setUpperAngLimit(0);//转动的上限
			sliderLRF.setDampingDirLin(0.5f); //设置滑动阻尼系数
			dynamicsWorld.addConstraint(sliderLRF,true);//将约束添加进物理世界
			sliders[index]=sliderLRF;//将约束放进约束数组
		}
		if(index==2){//若索引号为2，则添加竖杆与下侧横杆之间的滑动关节约束
			sliderLRN = new SliderConstraint(ra, rb, localA, localB, force);
			//设置初始的limit
			sliderLRN.setLowerLinLimit(-Stick_Length);//滑动的下限
			sliderLRN.setUpperLinLimit(Stick_Length);//滑动的上限
			sliderLRN.setLowerAngLimit(0);//转动的下限
			sliderLRN.setUpperAngLimit(0);//转动的上限
			sliderLRN.setDampingDirLin(0.5f); //设置滑动阻尼系数
			dynamicsWorld.addConstraint(sliderLRN,true);//将约束添加进物理世界
			sliders[index]=sliderLRN;//将约束放进约束数组
		}
	}
	public void slideFB(float mulFactor){
		sliding=true;//设置滑动标志位为true
		sliderFB.getRigidBodyB().activate();//激活箱子刚体
		currIndex=0;//设置当前动作索引号为0
		sliderFB.setPoweredLinMotor(true);//启动关节对应的马达
		sliderFB.setMaxLinMotorForce(1.0f);//设置马达的滑动驱动力
		sliderFB.setTargetLinMotorVelocity(5.0f*mulFactor);//设置马达的滑动驱动速度
	}
	public void slideLR(float mulFactor){
		sliding=true;//设置滑动标志位为true
		sliderLRF.getRigidBodyB().activate();//激活竖杆刚体
		currIndex=1;//设置当前动作索引号为1
		sliderLRF.setPoweredLinMotor(true);//启动竖杆与上侧横杆间关节对应的马达
		sliderLRF.setMaxLinMotorForce(5.0f);//设置马达的滑动驱动力
		sliderLRF.setTargetLinMotorVelocity(5.0f*mulFactor);//设置马达的滑动驱动速度
		
		sliderLRN.setPoweredLinMotor(true);//启动竖杆与上侧横杆间关节对应的马达
		sliderLRN.setMaxLinMotorForce(5.0f);//设置马达的滑动驱动力
		sliderLRN.setTargetLinMotorVelocity(5.0f*mulFactor);//设置马达的滑动驱动速度
	}
	public void stopSlide(){
		sliding=false;//设置滑动标志位为false
		sliders[currIndex].setPoweredLinMotor(false);//关闭当前动作编号对应的马达
		sliders[currIndex].setMaxLinMotorForce(0.0f);//设置马达的滑动驱动力为0
		sliders[currIndex].setTargetLinMotorVelocity(0.0f);//设置马达的滑动驱动速度为0
		if(currIndex==1){//若动作编号为1
			sliders[currIndex+1].setPoweredLinMotor(false);//关闭下一动作编号对应的马达
			sliders[currIndex+1].setMaxLinMotorForce(0.0f);//设置马达的滑动驱动力为0
			sliders[currIndex+1].setTargetLinMotorVelocity(0.0f);//设置马达的滑动驱动速度为0
		}
	}
	private class MyRenderer implements GLSurfaceView.Renderer{
		float ratio;
		Cube cube;
		Stick stickFBSliderAxis;
		Stick stickLRFSliderAxis;
		Stick stickLRNSliderAxis;
		int activeTexId;
		int deactiveTexId;
	    int textureArrow;//系统分配的游戏前进虚拟按钮纹理id
		int[] texIds = new int[2];
		TexRect button;//虚拟按钮 
		@Override public void onDrawFrame(GL10 gl) {
			GLES30.glEnable(GLES30.GL_DEPTH_TEST);
			GLES30.glClear(GLES30.GL_DEPTH_BUFFER_BIT|GLES30.GL_COLOR_BUFFER_BIT);
			//调用此方法计算产生透视投影矩阵   
			MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 100);
			//调用此方法产生摄像机9参数位置矩阵
            MatrixState.setCamera(2,14,2f,0,0,0,0f,0f,-1f);   
            MatrixState.setLightLocationRed(5, 50, 15);
            
			MatrixState.pushMatrix();
			cube.drawSelf(texIds, 1);
			stickFBSliderAxis.drawSelf(90,0,1,0);//绘制纵向轴
			stickLRFSliderAxis.drawSelf(0,1, 0, 0);//绘制远端横向轴
			stickLRNSliderAxis.drawSelf(0,1, 0, 0);//绘制近端横向轴
			MatrixState.popMatrix();
			 //绘制虚拟按钮
            //开启混合
			MatrixState.setCamera(0,0,10,0,0,0,0f,1.0f,0.0f); 
			MatrixState.setProjectOrtho(-ratio, ratio, -1, 1, 1.5f, 100);
			MatrixState.pushMatrix();
			GLES30.glDisable(GLES30.GL_DEPTH_TEST);
            GLES30.glEnable(GLES30.GL_BLEND);
            //设置混合因子
            GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA);
            //绘制按钮
            MatrixState.pushMatrix();
            MatrixState.translate(ratio-0.5f,-(1-0.5f),0);  
            button.drawSelf(textureArrow);
            MatrixState.popMatrix();
            //关闭混合
            GLES30.glDisable(GLES30.GL_BLEND);
            MatrixState.popMatrix();
		}
		@Override public void onSurfaceChanged(GL10 gl, int width, int height) {
			 //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            ratio= (float) width / height;   
            screenWidth= width;
            screenHeight=height;
            buttonPixels=screenHeight/2;
		}
		@Override public void onSurfaceCreated(GL10 gl, EGLConfig config) {//TODO
			  //设置屏幕背景色RGBA
            GLES30.glClearColor(0.0f,0.0f,0.0f, 1.0f);
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            MatrixState.setInitStack();
            activeTexId = initTexture(R.drawable.wood_bin1);
            textureArrow = initTexture(R.drawable.arrow_small);
            texIds[0]=deactiveTexId;
            texIds[1]=activeTexId;
            cube = new Cube(MySurfaceView.this,cubeSize,cubeBody.body);
            stickFBSliderAxis = new Stick(MySurfaceView.this,Stick_Length,Stick_R,11.25f,
					new float[]{1,0,0,1},
					stickFBSliderBody.body);
            stickLRFSliderAxis = new Stick(MySurfaceView.this,Stick_Length,Stick_R,11.25f,
					new float[]{1,0,0,1},
					stickLRFSliderBody.body);
            stickLRNSliderAxis = new Stick(MySurfaceView.this,Stick_Length,Stick_R,11.25f,
					new float[]{1,0,0,1},
					stickLRNSliderBody.body);
            button = new TexRect(MySurfaceView.this, 0.5f,1f,1f);
            
            new Thread(){
            	public void run(){
            		while(flag){            			
            			try{
            				//模拟
                			dynamicsWorld.stepSimulation(1f/60.f, 5);
							Thread.sleep(20);
						} catch (Exception e){
							e.printStackTrace();
						}
            		}
            	}
            }.start();
            new KeyThread(MySurfaceView.this).start();
		}
	}
	public int initTexture(int drawableId){//textureId
		//生成纹理ID
		int[] textures = new int[1];
		GLES30.glGenTextures
		(
				1,          //产生的纹理id的数量
				textures,   //纹理id的数组
				0           //偏移量
		);    
		int textureId=textures[0];    
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE);
        
        //通过输入流加载图片===============begin===================
        InputStream is = this.getResources().openRawResource(drawableId);
        Bitmap bitmapTmp;
        try 
        {
        	bitmapTmp = BitmapFactory.decodeStream(is);
        } 
        finally 
        {
            try 
            {
                is.close();
            } 
            catch(IOException e) 
            {
                e.printStackTrace();
            }
        }
        //通过输入流加载图片===============end=====================  
        //实际加载纹理
        GLUtils.texImage2D
        (
        		GLES30.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
        		0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
        		bitmapTmp, 			  //纹理图像
        		0					  //纹理边框尺寸
        );
        bitmapTmp.recycle(); 		  //纹理加载成功后释放图片
        
        return textureId;
	}
}