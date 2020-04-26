package com.bn.Sample7_7;
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
import com.bulletphysics.dynamics.constraintsolver.SequentialImpulseConstraintSolver;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.collision.shapes.SphereShape;
import com.bulletphysics.collision.shapes.StaticPlaneShape;

import static com.bn.Sample7_7.Constant.*;

class MySurfaceView extends GLSurfaceView 
{
	private SceneRenderer mRenderer;//场景渲染器
	//=============拾取============
  //光源距离Y轴的距离
    final float LIGHT_Y=70;
    float mPreviousX;//上次的触控位置X坐标
    float mPreviousY;
    float preNanoTime;
    //是否移动的标志位
    boolean isMoveFlag=false;
    float cx=0;	//摄像机位置x
    float cy=1;  
    float cz=6;   //摄像机位置z
    float tx=0;   //摄像机目标点x
    float tz=-10;   //摄像机目标点z
    //摄像机转动的角度
    double tempRadians=0;
    //存储抬起手后摄像机转动的角度
    double upRadians=0;
    float left;
    float right;
	float top;
	float bottom;
	float near;
	float far;
	ArrayList<LoadedObjectVertexNormal> lovnList=new ArrayList<LoadedObjectVertexNormal>();
	//被选中物体的索引值，即id，没有被选中时索引值为-1
	int checkedIndex=-1;
	float xOffset=0;//在轴上移动的位置
	float yOffset=0;
	float zOffset=0;
	//===========分屏==========
	static enum Area{LU,RU,LD,RD,NONE};
	Area currArea=Area.NONE;
	boolean areaTouch=false;
	AreaTouchThread areaTouchThread;
	
	//========摄像机========
	Vector3f cameraCircleCenter=new Vector3f(0,0,0);
	
	boolean forward=false;
	//===============物理模拟====================
	DiscreteDynamicsWorld dynamicsWorld;//世界对象
	CollisionShape ballShape;//共用的立方体
	CollisionShape planeShape;//共用的平面形状
	Sample7_7_Activity activity;
	LoadedObjectVertexNormal[] loadedModels=new LoadedObjectVertexNormal[7];
	LoadedObjectVertexNormal[] bodyForDraws=new LoadedObjectVertexNormal[BodyPartIndex.BODYPART_COUNT.ordinal()];
	public MySurfaceView(Context context) 
	{
        super(context);
        this.activity=(Sample7_7_Activity) context;
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
		//创建碰撞检测算法分配者对象，其功能为扫描所有的碰撞检测对，并确定适用的检测策略对应的算法
		CollisionDispatcher dispatcher = new CollisionDispatcher(collisionConfiguration);		
		//设置整个物理世界的边界信息
		Vector3f worldAabbMin = new Vector3f(-10000, -10000, -10000);
		Vector3f worldAabbMax = new Vector3f(10000, 10000, 10000);
		int maxProxies = 1024;
		//创建碰撞检测粗测阶段的加速算法对象
		AxisSweep3 overlappingPairCache =new AxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
		//创建推动约束解决者对象
		SequentialImpulseConstraintSolver solver = new SequentialImpulseConstraintSolver();
		//创建物理世界对象
		dynamicsWorld = new DiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration);
		//设置重力加速度
		dynamicsWorld.setGravity(new Vector3f(0, -30, 0));
		//创建共用的立方体
		ballShape=new SphereShape(1);
		//创建共用的平面形状
		planeShape=new StaticPlaneShape(new Vector3f(0, 1, 0), 0);
	}
	
	public void loadCapsules(){//加载胶囊
		loadedModels[0] = LoadUtil.loadFromFile("head.obj", activity.getResources(), MySurfaceView.this);//头
		loadedModels[1] = LoadUtil.loadFromFile("spine.obj", activity.getResources(), MySurfaceView.this);//脊椎
		loadedModels[2] = LoadUtil.loadFromFile("pelvis.obj", activity.getResources(), MySurfaceView.this);//骨盆
		loadedModels[3] = LoadUtil.loadFromFile("upper_arm.obj", activity.getResources(), MySurfaceView.this);//大臂
		loadedModels[4] = LoadUtil.loadFromFile("lower_arm.obj", activity.getResources(), MySurfaceView.this);//小臂
		loadedModels[5] = LoadUtil.loadFromFile("upper_leg.obj", activity.getResources(), MySurfaceView.this);//大腿
		loadedModels[6] = LoadUtil.loadFromFile("lower_leg.obj", activity.getResources(), MySurfaceView.this);//小腿
	}
	
	public void initBodyForDraws(){
		bodyForDraws[BodyPartIndex.BODYPART_HEAD.ordinal()]=loadedModels[0];
		bodyForDraws[BodyPartIndex.BODYPART_SPINE.ordinal()]=loadedModels[1];
		bodyForDraws[BodyPartIndex.BODYPART_PELVIS.ordinal()]=loadedModels[2];
		bodyForDraws[BodyPartIndex.BODYPART_RIGHT_UPPER_ARM.ordinal()]=loadedModels[3];
		bodyForDraws[BodyPartIndex.BODYPART_LEFT_UPPER_ARM.ordinal()]=loadedModels[3].clone();
		bodyForDraws[BodyPartIndex.BODYPART_LEFT_LOWER_ARM.ordinal()]=loadedModels[4];
		bodyForDraws[BodyPartIndex.BODYPART_RIGHT_LOWER_ARM.ordinal()]=loadedModels[4].clone();
		bodyForDraws[BodyPartIndex.BODYPART_RIGHT_UPPER_LEG.ordinal()]=loadedModels[5];
		bodyForDraws[BodyPartIndex.BODYPART_LEFT_UPPER_LEG.ordinal()]=loadedModels[5].clone();
		bodyForDraws[BodyPartIndex.BODYPART_RIGHT_LOWER_LEG.ordinal()]=loadedModels[6];
		bodyForDraws[BodyPartIndex.BODYPART_LEFT_LOWER_LEG.ordinal()]=loadedModels[6].clone();
		for(int i=0;i<bodyForDraws.length;i++){
			lovnList.add(bodyForDraws[i]);
		}
	}
	float x;
	float y;
	@Override
	public boolean onTouchEvent(MotionEvent event)//处理触控事件的方法
	{
		x=event.getX();//获取触控点的位置
		y=event.getY();
		switch(event.getAction())//根据不同的动作类型执行不同的操作
		{
			case MotionEvent.ACTION_DOWN://按下的动作
				mPreviousX=event.getX();//记录按下动作对应的位置
				mPreviousY=event.getY();
				preNanoTime=System.nanoTime();//获取当前系统时间
    			checkedIndex=-1;//设置选中物体索引(-1表示没有选中物体)
    			
    			//计算拾取用射线的起点与终点
    			float[] AB=IntersectantUtil.calculateABPosition
    			(
    				x, //触控点X坐标
    				y, //触控点Y坐标
    				Sample7_7_Activity.screenWidth, //屏幕宽度
    				Sample7_7_Activity.screenHeight, //屏幕高度
    				left, //近平面left值
    				top,//近平面top值
    				near, //近平面距离
    				far//远平面距离
    			);    			  

    			//射线AB
    			MyVector3f start = new MyVector3f(AB[0], AB[1], AB[2]);//拾取射线的起点A
    			MyVector3f end = new MyVector3f(AB[3], AB[4], AB[5]);//拾取射线的终点B
    			MyVector3f dir = end.minus(start);//拾取射线的长度和方向
    			/*
    			 * 计算AB线段与每个物体包围盒的最佳交点(与A点最近的交点)，
    			 * 并记录有最佳交点的物体在列表中的索引值
    			 */
    			//记录列表中时间最小的索引值
        		int tmpIndex=-1;//用于记录距离A点最近物体的索引值
        		float minTime=1;//用于记录列表中物体与射线相交的最短时间
        		for(int i=0;i<lovnList.size();i++){//遍历列表中的物体
        			AABB3 box = lovnList.get(i).getCurrBox(); //获得物体的AABB包围盒   
    				float t = box.rayIntersect(start, dir, null);//计算相交时间
        			if (t <= minTime) {
    					minTime = t;//更新最短时间
    					tmpIndex = i;//更新最近物体索引
    				}
        		}
        		checkedIndex=tmpIndex;//记录最近(选中)物体索引    		
        		changeObj(checkedIndex);//改变被选中物体	
        		
			break;
			case MotionEvent.ACTION_MOVE://移动的动作
				//移动距离大于指定的阈值，才认为是真正的移动
				if(x-mPreviousX>=10.0f||x-mPreviousX<=-10.0f)
				{//移动大于相应的值，则认为其真正的移动
					isMoveFlag=true;
				}
				if(isMoveFlag)//若构成移动
				{
					if(checkedIndex!=-1){//若被选中物体索引不为-1
						//获取选中物体
						LoadedObjectVertexNormal lovo = lovnList.get(checkedIndex);
						lovo.isPicked=true;//设置此物体的选中标志位为true
						//计算新的触控点在世界坐标系中的位置
						float[] nearXY=IntersectantUtil.calculateABPosition(
								x,y,
								Sample7_7_Activity.screenWidth,Sample7_7_Activity.screenHeight,
								left,top,
								near,far
								);
						//计算上一触控点在世界坐标系中的位置
						float[] nearPreXY = IntersectantUtil.calculateABPosition(
								mPreviousX,mPreviousY,
								Sample7_7_Activity.screenWidth,Sample7_7_Activity.screenHeight,
								left,top,
								near,far
								);
						//获取被选中物体所属点对点关节在刚体坐标系中的位置
						Vector3f currPivot = lovo.p2p.getPivotInB(new Vector3f());
						//计算出拖动方向向量3个分量的值
						Vector3f dir1 = new Vector3f(nearXY[0]-nearPreXY[0],nearXY[1]-nearPreXY[1],nearXY[2]-nearPreXY[2]);
						float vFactor = 0.5f;
						dir1.set(dir1.x*vFactor,dir1.y*vFactor,dir1.z*vFactor);
						currPivot.add(dir1);//计算出拖动后的关节位置
						lovo.p2p.setPivotB(currPivot);//设置拖动后的关节位置
					}
				}
			break;
			case MotionEvent.ACTION_UP://抬起的动作
				
				isMoveFlag=false;//将移动标志位设置为false
				if(checkedIndex!=-1){//若有物体被选中
					LoadedObjectVertexNormal lovo = lovnList.get(checkedIndex);
	    			lovo.removePickedConstraint();//删除用于拖动的点对点关节
	    			checkedIndex=-1;//选中物体索引设置为-1，表示无选中物体
				}
				if(checkedIndex==-1 && currArea!=Area.NONE){//若没有选中物体
					currArea=Area.NONE;
					areaTouch=false;
					upRadians=tempRadians;
				}
			break;
		}
		return true;
	}

	//改变列表中下标为index的物体
	public void changeObj(int index){//根据索引改变选中物体
		if(index != -1){//如果有物体被选中			
			LoadedObjectVertexNormal lovo = lovnList.get(index);
			lovo.body.activate();
			lovo.addPickedConstraint();
        }
    	else{
    		//表示没有拾取到
			if(0<x && x<Sample7_7_Activity.screenWidth/2 && 0<y && y<Sample7_7_Activity.screenHeight/2){
				currArea = Area.LU;
			}else if(Sample7_7_Activity.screenWidth/2<x &&  0<y && y<Sample7_7_Activity.screenHeight/2){
				currArea = Area.RU;
			}else if(0<x && x<Sample7_7_Activity.screenWidth/2 && y>Sample7_7_Activity.screenHeight/2){
				currArea = Area.LD;
			}else if(x>Sample7_7_Activity.screenWidth/2 && y>Sample7_7_Activity.screenHeight/2){
				currArea = Area.RD;
			}
			areaTouch=true;
			if(areaTouchThread==null || !areaTouchThread.isAlive()){
				areaTouchThread = new AreaTouchThread();
				areaTouchThread.start();
			}		
    	}
	}
	private class SceneRenderer implements GLSurfaceView.Renderer 
    {
		int floorTextureId;//地面纹理
		TexFloor floor;//纹理矩形1		
		Doll doll;
		
        public void onDrawFrame(GL10 gl) {            
            
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);

            //初始化光源位置
        	MatrixState.setLightLocation((float)(LIGHT_Y*Math.sin(tempRadians)), 30, (float)(LIGHT_Y*Math.cos(tempRadians)));
            
            //初始化变换矩阵
            cx=(float) (25*Math.sin(tempRadians)+cameraCircleCenter.x);
			cz=(float) (25*Math.cos(tempRadians)+cameraCircleCenter.z);
			tx=(float) (10*Math.sin(tempRadians)+cameraCircleCenter.x);
			tz=(float) (10*Math.cos(tempRadians)+cameraCircleCenter.z);
            MatrixState.setCamera(cx,cy,cz,tx,0f,tz,0f,1.0f,0.0f);
            
            MatrixState.pushMatrix();
            MatrixState.copyMVMatrix();
        	doll.drawSelf(checkedIndex);
            //绘制地板
            MatrixState.pushMatrix();
            floor.drawSelf( floorTextureId);
            MatrixState.popMatrix();         
            
            MatrixState.popMatrix(); 
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height);
            //计算透视投影的比例
            float ratio = (float) width / height;
            left=right=ratio;
            top=bottom=1;
            near=2;
            far=100;
            MatrixState.setProjectFrustum(-left, right, -bottom, top, near, far);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //设置屏幕背景色黑色RGBA
            GLES30.glClearColor(0,0,0,0);            
            //启用深度测试
            GLES30.glEnable(GL10.GL_DEPTH_TEST);  
            //设置为打开背面剪裁
            GLES30.glEnable(GL10.GL_CULL_FACE);
           
            
            MatrixState.setInitStack();
            ShaderManager.loadCodeFromFile(activity.getResources());
            ShaderManager.compileShader();
            loadCapsules();
            initBodyForDraws();
            
            //初始化纹理
            floorTextureId=initTextureRepeat(R.drawable.f6);            
            doll = new Doll(MySurfaceView.this,dynamicsWorld,bodyForDraws);
            
            //创建地面矩形
            floor=new TexFloor(ShaderManager.getTextureShaderProgram(),80*Constant.UNIT_SIZE,-Constant.UNIT_SIZE,planeShape,dynamicsWorld);
            new Thread()
            {
            	public void run()
            	{
            		while(true)
            		{            			
            			try 
            			{
            				//开始模拟
                			dynamicsWorld.stepSimulation(TIME_STEP, MAX_SUB_STEPS);
							Thread.sleep(20);	//当前线程睡眠20毫秒
						} catch (Exception e) 
						{
							e.printStackTrace();
						}
            		}
            	}
            }.start();					//启动线程
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
        		GLES30.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL20.GL_TEXTURE_2D
        		0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
        		bitmapTmp, 			  //纹理图像
        		0					  //纹理边框尺寸
        );
        bitmapTmp.recycle(); 		  //纹理加载成功后释放图片
        
        return textureId;
	}
	class AreaTouchThread extends Thread{
		float time=0;
		float linearV=0.1f;
		float angularV=2;
		public void run(){
			while(areaTouch){
				System.out.println("run");
				System.out.println(currArea.ordinal());
				
				time++;
				if(currArea==Area.LU){//左转
					tempRadians=upRadians-Math.toRadians(time*angularV);
				}
				if(currArea==Area.RU){//右转
					tempRadians=upRadians+Math.toRadians(time*angularV);
				}
				if(currArea==Area.LD){//前进
					float moveLength=time*linearV;
					cameraCircleCenter.x=(float) (cameraCircleCenter.x-moveLength*Math.sin(tempRadians));
					cameraCircleCenter.z=(float) (cameraCircleCenter.z-moveLength*Math.cos(tempRadians));
				}
				if(currArea==Area.RD){//后退
					float moveLength=time*linearV;
					cameraCircleCenter.x=(float) (cameraCircleCenter.x+moveLength*Math.sin(tempRadians));
					cameraCircleCenter.z=(float) (cameraCircleCenter.z+moveLength*Math.cos(tempRadians));
				}
				try {
					Thread.sleep(50);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	}
}