package com.bn.Sample7_5;

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
import com.bulletphysics.dynamics.DiscreteDynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.constraintsolver.HingeConstraint;
import com.bulletphysics.dynamics.constraintsolver.SequentialImpulseConstraintSolver;
import com.bulletphysics.linearmath.Clock;
import com.bulletphysics.linearmath.MatrixUtil;
import com.bulletphysics.linearmath.Transform;

import static com.bn.Sample7_5.Constant.*;

public class MySurfaceView extends GLSurfaceView {
	
	DiscreteDynamicsWorld dynamicsWorld;//物理世界
	CollisionShape boxShape;//长方体碰撞形状
	HingeConstraint constraint;//铰链关节约束
	private	float time;//当前时间
	private float cyclePeriod; //周期时长
	private float muscleStrength;//驱动关节的力度
	Clock clock = new Clock();//时钟（JBullet提供）	
	private float mPreviousY;//上次的触控位置Y坐标
    private float mPreviousX;//上次的触控位置X坐标
    boolean flag=true;//循环控制标志
    MyRenderer renderer; //渲染器
    
	public MySurfaceView(Context context) {
		super(context);
		this.setEGLContextClientVersion(3);		
		renderer = new MyRenderer();//创建渲染器
		this.setRenderer(renderer);//设置渲染器
		this.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式
		initWorld();//初始化物理世界
		time = 0.0f;//将当前时间初始化为0
		cyclePeriod = 3000.0f; //设置周期时长
		muscleStrength = 5f;//设置驱动关节的力度
	}
	 @Override 
	    public boolean onTouchEvent(MotionEvent e) {
	        float y = e.getY();
	        float x = e.getX();
	        switch (e.getAction()) {
	        case MotionEvent.ACTION_MOVE:
	            float dy = y - mPreviousY;//计算触控笔Y位移
	            float dx = x - mPreviousX;//计算触控笔X位移 	            
	            if(Math.abs(dx)>10)
	            {
	            	yAngle += dx /100;//设置纹理圆球绕y轴旋转的角度
	 	            cx=(float) (Math.cos(yAngle)*15);
	 	            cz=(float) (Math.sin(yAngle)*15);
	            }
	            if(Math.abs(dy)>10)
	            {
	            	cy=cy+dy/10;
		            if(cy>15)
		            {
		            	cy=15;
		            }
		            else if(cy<-15)
		            {
		            	cy=-15;
		            }
	            }	            
	            break;
	        }
	        mPreviousY = y;//记录触控笔位置
	        mPreviousX = x;//记录触控笔位置
	        return true; 
	    }
	public void initWorld(){//初始化物理世界
		CollisionConfiguration collisionConfiguration = new DefaultCollisionConfiguration();
		CollisionDispatcher dispatcher = new CollisionDispatcher(collisionConfiguration);
		Vector3f worldAabbMin = new Vector3f(-10000, -10000, -10000);
		Vector3f worldAabbMax = new Vector3f(10000, 10000, 10000);
		int maxProxies = 1024;
		AxisSweep3 overlappingPairCache =new AxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
		SequentialImpulseConstraintSolver solver = new SequentialImpulseConstraintSolver();
		dynamicsWorld = new DiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration);
		dynamicsWorld.setGravity(new Vector3f(0, -10, 0));
		boxShape=new BoxShape(new Vector3f(halfX,halfY,halfZ));
	}
	private class MyRenderer implements GLSurfaceView.Renderer{
		Cuboid door1; //表示不动的门的纹理长方体
		Cuboid door2;	//表示运动的门的纹理长方体
		int activeTexId;//门的纹理id
		@Override
		public void onDrawFrame(GL10 gl) {//绘制一帧画面的方法
		
			GLES30.glClear(GLES30.GL_DEPTH_BUFFER_BIT|GLES30.GL_COLOR_BUFFER_BIT);//清空缓存
			float ms = getDeltaTimeMicroseconds();//获取两帧画面间的时间间隔
			time+=ms;//当前时间递增
			float preAngle = constraint.getHingeAngle();//获取关节上一时刻的角度
			//求得公式1中需要的x
			float x=(((int)(time / 1000) % (int)(cyclePeriod)) / cyclePeriod)*BulletGlobals.SIMD_2_PI;
			//根据公式1求出y
			float y= 0.5f * (1.0f + (float)Math.sin(x));
			//计算出当前时刻的旋转角度
			float currAngle = constraint.getLowerLimit() + y * (constraint.getUpperLimit() - constraint.getLowerLimit());
			float angleDelta = currAngle - preAngle;	//计算角度差
			float desiredAngularVel = 1000000.f * angleDelta/ms;//计算此帧的角速度
			constraint.enableAngularMotor(true, desiredAngularVel, muscleStrength);//设置旋转马达
			//设置摄像机
            MatrixState.setCamera(cx,cy,cz,0,0,0,0f,1.0f,0.0f);    
			MatrixState.pushMatrix();//保护现场
			door1.drawSelf(activeTexId);//绘制不动的门
			door2.drawSelf(activeTexId);//绘制运动的门
			MatrixState.popMatrix();//恢复现场
		}   

		@Override
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			 //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio= (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 1.5f, 100);            
		}

		@Override
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			//设置屏幕背景色RGBA
            GLES30.glClearColor(0.0f,0.0f,0.0f, 1.0f);
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            MatrixState.setInitStack();
            activeTexId = initTexture(R.drawable.wood_bin1);
            //创建不动的门对应的长方体
            door1 = new Cuboid(dynamicsWorld, boxShape, 0, 0, 0, 4, MySurfaceView.this,halfX,halfY,halfZ);
            //创建运动的门对应的长方体
            door2 = new Cuboid(dynamicsWorld, boxShape, 1, 0, 0, -4, MySurfaceView.this,halfX,halfY,halfZ);
            //对两个门对应的刚体添加铰链约束
            addHingeConstraint(door1.body,door2.body);
            new Thread()
            {
            	public void run()
            	{
            		while(flag)
            		{            			
            			try 
            			{
            				//模拟
                			dynamicsWorld.stepSimulation(1f/60.f, 5);
							Thread.sleep(20);
						} catch (Exception e) 
						{
							e.printStackTrace();
						}
            		}
            	}
            }.start();
		}
	}
	
	public void addHingeConstraint(RigidBody rbA,RigidBody rbB){//添加铰链约束的方法
		Transform transA = new Transform();//创建变换对象（从约束到不动门的质心）
		transA.setIdentity();//初始化变换对象
		MatrixUtil.setEulerZYX(transA.basis, BulletGlobals.SIMD_HALF_PI,0,0 );//设置变换对象中的旋转信息
		transA.origin.set(new Vector3f(0,0,-halfZ));//设置变换对象中的平移信息
		Transform transB = new Transform();//创建变换对象（从约束到运动门的质心）		
		transB.setIdentity();//初始化变换对象
		MatrixUtil.setEulerZYX(transB.basis, BulletGlobals.SIMD_HALF_PI,0,0 );//设置变换对象中的旋转信息
		transB.origin.set(new Vector3f(0,0,halfZ));//设置变换对象中的平移信息
		constraint = new HingeConstraint(rbA,rbB,transA,transB);//创建铰链约束
		constraint.setLimit(-BulletGlobals.SIMD_HALF_PI/2, BulletGlobals.SIMD_HALF_PI/2);//设置铰链约束的转动范围（即下限值与上限值）
		dynamicsWorld.addConstraint(constraint, true);//将约束添加到物理世界
	}
	public int initTexture(int drawableId)//textureId
	{
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
	public float getDeltaTimeMicroseconds() {//获取两帧画面间物理世界时间差的方法
		float dt = clock.getTimeMicroseconds();//获取从时钟创建或上次时间重置到现在的毫秒数
		clock.reset();//重置时钟
		return dt;//返回所求的时间差
	}
}