package com.bn.Sample7_1;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import javax.vecmath.Vector3f;
import com.bulletphysics.collision.broadphase.AxisSweep3;
import com.bulletphysics.collision.dispatch.CollisionConfiguration;
import com.bulletphysics.collision.dispatch.CollisionDispatcher;
import com.bulletphysics.collision.dispatch.DefaultCollisionConfiguration;
import com.bulletphysics.dynamics.DiscreteDynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.constraintsolver.SequentialImpulseConstraintSolver;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import com.bulletphysics.collision.shapes.BoxShape;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.collision.shapes.StaticPlaneShape;

import static com.bn.Sample7_1.Constant.*;

class MySurfaceView extends GLSurfaceView 
{
	private SceneRenderer mRenderer;//场景渲染器	
	DiscreteDynamicsWorld dynamicsWorld;//世界对象
	ArrayList<TexCube> tca=new ArrayList<TexCube>();
	ArrayList<TexCube> tcaForAdd=new ArrayList<TexCube>();
	CollisionShape boxShape;//共用的立方体
	CollisionShape planeShape;//共用的平面形状
	Sample7_1_Activity activity;
	
	public MySurfaceView(Context context) 
	{
        super(context);
        this.activity=(Sample7_1_Activity) context;
        this.setEGLContextClientVersion(3);
        //初始化物理世界
        initWorld();        
        mRenderer = new SceneRenderer();	//创建场景渲染器
        setRenderer(mRenderer);				//设置渲染器		
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式为主动渲染   
    }
	
	//初始化物理世界的方法
	public void initWorld()
	{
		//创建碰撞检测配置信息对象
		CollisionConfiguration collisionConfiguration = new DefaultCollisionConfiguration();		
		//创建碰撞检测算法分配者对象，用来扫描所有的碰撞检测对，确定使用何种检测策略
		CollisionDispatcher dispatcher = new CollisionDispatcher(collisionConfiguration);		
		
		Vector3f worldAabbMin = new Vector3f(-10000, -10000, -10000);//设置整个物理世界的边界最小值
		Vector3f worldAabbMax = new Vector3f(10000, 10000, 10000);//设置整个物理世界的边界最大值
		int maxProxies = 1024;//最大代理数量
		//创建碰撞检测粗测阶段的加速算法对象
		AxisSweep3 overlappingPairCache =new AxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
		//创建推动约束解决者对象
		SequentialImpulseConstraintSolver solver = new SequentialImpulseConstraintSolver();
		//创建物理世界对象
		dynamicsWorld = new DiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration);
		//设置重力加速度
		dynamicsWorld.setGravity(new Vector3f(0, -10, 0));
		//创建共用的立方体碰撞形状
		boxShape=new BoxShape(new Vector3f(Constant.UNIT_SIZE,Constant.UNIT_SIZE,Constant.UNIT_SIZE));
		//创建共用的平面碰撞形状
		planeShape=new StaticPlaneShape(new Vector3f(0, 1, 0), 0);
	}

	private class SceneRenderer implements GLSurfaceView.Renderer 
    {
		int[] cubeTextureId=new int[2];//箱子面纹理
		int floorTextureId;//地面纹理
		TexFloor floor;//纹理矩形1		
		
        public void onDrawFrame(GL10 gl) { 
        	//清除颜色缓存于深度缓存
        	GLES30.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);            
            //绘制箱子
            synchronized(tca)
			{
	            for(TexCube tc:tca)
	            {
	            	MatrixState.pushMatrix();
	                tc.drawSelf(cubeTextureId); 
	                MatrixState.popMatrix();         
	            }            
			}
            
            //绘制地板
            MatrixState.pushMatrix();
            floor.drawSelf( floorTextureId);
            MatrixState.popMatrix();         
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height);
            //计算透视投影的比例
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 2, 100);
            
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //设置屏幕背景色黑色RGBA
            GLES30.glClearColor(0,0,0,0);            
            //启用深度测试
            GLES30.glEnable(GL10.GL_DEPTH_TEST);  
            //设置为打开背面剪裁
            GLES30.glEnable(GL10.GL_CULL_FACE);
            //初始化变换矩阵
            MatrixState.setInitStack();
            MatrixState.setCamera( 
            		EYE_X,   //人眼位置的X
            		EYE_Y, 	//人眼位置的Y
            		EYE_Z,   //人眼位置的Z
            		TARGET_X, 	//人眼球看的点X
            		TARGET_Y,   //人眼球看的点Y
            		TARGET_Z,   //人眼球看的点Z
            		0, 
            		1, 
            		0);
            //初始化所用到的shader程序
            ShaderManager.loadCodeFromFile(activity.getResources());
            ShaderManager.compileShader();
            //初始化纹理
            cubeTextureId[0]=initTexture(R.drawable.wood_bin2);
            cubeTextureId[1]=initTexture(R.drawable.wood_bin1);
            floorTextureId=initTextureRepeat(R.drawable.f6);            
            
            //创建地面矩形
            floor=new TexFloor(ShaderManager.getTextureShaderProgram(),80*Constant.UNIT_SIZE,-Constant.UNIT_SIZE,planeShape,dynamicsWorld);
           
            //创建立方体       
            int size=2;   //立方体箱子尺寸
            float xStart=(-size/2.0f+0.5f)*(2+0.4f)*Constant.UNIT_SIZE;//x坐标起始值
            float yStart=0.02f;//y坐标起始值
            float zStart=(-size/2.0f+0.5f)*(2+0.4f)*Constant.UNIT_SIZE-4f;//z坐标起始值
            for(int i=0;i<size;i++)//对层、行、列进行循环，创建两层8个立方体箱子
            {
            	for(int j=0;j<size;j++)
            	{
            		for(int k=0;k<size;k++)
            		{
            			TexCube tcTemp=new TexCube       //创建立方体箱子
            			(
            					MySurfaceView.this,		//MySurfaceView的引用
                				Constant.UNIT_SIZE,		//箱子的尺寸
                				boxShape,				//碰撞形状
                				dynamicsWorld,			//物理世界
                				1,						//刚体质量		
                				xStart+i*(2+0.4f)*Constant.UNIT_SIZE,//箱子初始x坐标
                				yStart+j*(2.02f)*Constant.UNIT_SIZE, //箱子初始y坐标        
                				zStart+k*(2+0.4f)*Constant.UNIT_SIZE,//箱子初始z坐标
                				ShaderManager.getTextureShaderProgram()//着色器程序引用
                		);            			
            			tca.add(tcTemp);//将新箱子添加到待待添加箱子集合中
            			//将立方体箱子设置为一开始是不激活的
            			tcTemp.body.forceActivationState(RigidBody.WANTS_DEACTIVATION);
            		}
            	}
            }
            
            new Thread()//通过匿名内部类创建线程对象
            {
            	public void run()//重写线程中执行任务的run方法
            	{
            		while(true)//模拟循环
            		{            			
            			try 
            			{
            				synchronized(tcaForAdd)//锁定待添加箱子集合
            	            {
            					synchronized(tca)//锁定总箱子集合
            					{
            						for(TexCube tc:tcaForAdd)
                	                {
                	            		tca.add(tc);  //向总箱子集合中添加箱子
                	                }
            					}            	            	
            	            	tcaForAdd.clear();		//将待添加箱子集合清空
            	            }           
            				//执行模拟
                			dynamicsWorld.stepSimulation(TIME_STEP, MAX_SUB_STEPS);
							Thread.sleep(20);	//当前线程睡眠20毫秒
						} catch (Exception e) //若有异常则打印异常栈
						{
							e.printStackTrace();
						}
            		}
            	}
            }.start();					//启动线程
        }
    }
	
	//触摸事件回调方法
    @Override public boolean onTouchEvent(MotionEvent e) 
    {
        switch (e.getAction()) 
        {
           case MotionEvent.ACTION_DOWN:			//处理触控点按下的事件
        	TexCube tcTemp=new TexCube				//创建一个箱子
   			(
   					this,							//MySurfaceView的引用
       				Constant.UNIT_SIZE,				//箱子的尺寸
       				boxShape,						//碰撞形状
       				dynamicsWorld,					//物理世界
       				1,								//刚体质量
       				0,								//箱子初始x坐标
       				2,         						//箱子初始y坐标 
       				4,								//箱子初始z坐标
       				ShaderManager.getTextureShaderProgram()//着色器程序引用
       		);        
        	//设置箱子的初始线速度
        	tcTemp.body.setLinearVelocity(new Vector3f(0,2,-12));
        	//设置箱子的初始角速度
        	tcTemp.body.setAngularVelocity(new Vector3f(0,0,0));
        	
        	synchronized(tcaForAdd)//锁定待添加箱子集合
            {
        	   tcaForAdd.add(tcTemp);//将新箱子添加到待添加箱子集合中
            }
           break;
        }
        return true;//返回true通知系统此事已被处理
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
	public int initTextureRepeat(int drawableId)//textureId
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
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_REPEAT);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_REPEAT);
        
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