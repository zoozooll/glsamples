package com.bn.Sample7_8;

import java.io.IOException;
import java.io.InputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.view.MotionEvent;

import com.bn.Sample7_8.util.LoadUtil;
import com.bn.Sample7_8.util.LoadedObjectVertexNormalTexture;
import com.bn.Sample7_8.util.MatrixState;
import com.bn.Sample7_8.util.Vector3f;
import com.bulletphysics.collision.broadphase.AxisSweep3;
import com.bulletphysics.collision.dispatch.CollisionConfiguration;
import com.bulletphysics.collision.dispatch.CollisionDispatcher;
import com.bulletphysics.collision.dispatch.DefaultCollisionConfiguration;
import com.bulletphysics.collision.shapes.BoxShape;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.collision.shapes.CylinderShape;
import com.bulletphysics.dynamics.DiscreteDynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.RigidBodyConstructionInfo;
import com.bulletphysics.dynamics.constraintsolver.SequentialImpulseConstraintSolver;
import com.bulletphysics.linearmath.DefaultMotionState;
import com.bulletphysics.linearmath.Transform;

public class MySurfaceView extends GLSurfaceView {
	SenceRenderer mRenderer;
	int land;
	int vehicleTextureId;
	int boxId;
	int pailId;
	DiscreteDynamicsWorld dynamicsWorld;//世界对象\
	Vector3f carStartPoint=new Vector3f(0,0,0);
	Car car;
	Box box1;
	Box box2;
	Box box3;
	Box box4;
	Box box5;
	Box box6;
	Pail pail1;
	Pail pail2;
	Pail pail3;
	TextFloor tf;
	float Width;
	float Hight;
	boolean flag;
	float cam=0;
	
	float v_x = 1f; //箱子
    float v_y = 1f;//箱子高
    float v_z = 1f;//箱子长
	float v_xp =1.9f;//pail宽
    float v_yp = 1.9f;//pail高
    float v_zp = 1.9f;//pail长
    CollisionShape rbShape;
    CollisionShape rbShape_pail;
	public MySurfaceView(GameActivity context) {
		super(context);
		initWorld(); //初始化物理世界
		this.setEGLContextClientVersion(3);	//设置使用OpenGL ES3.0
		mRenderer=new SenceRenderer();//创建场景渲染器
		this.setRenderer(mRenderer);//设置渲染器
		//设置渲染模式为主动渲染
		this.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	}
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		if(flag){//如果加载完，触控可以触发
		float X=event.getX();
		float Y=event.getY();
		switch(event.getAction()){
			case MotionEvent.ACTION_DOWN:
				if(X<Width/2)
				{
					car.carMove_go();
					if(Y<Hight/2)
					{
						cam++;
					}
				}else{
					car.carMove_back();
					if(Y<Hight/2)
					{
						car.clientResetScene();
						box1.clientResetScene();
						box2.clientResetScene();
						box3.clientResetScene();
						box4.clientResetScene();
						box5.clientResetScene();
						box6.clientResetScene();
						pail1.clientResetScene();
						pail2.clientResetScene();
						pail3.clientResetScene();
					}
				}
			break;
			case MotionEvent.ACTION_MOVE:
				break;
			case MotionEvent.ACTION_UP:
				car.carKong();
			break;
		}
		}
		return true;
		
	}
	class SenceRenderer implements GLSurfaceView.Renderer//场景渲染器内部类
	{
		LoadedObjectVertexNormalTexture pm;//加载的地面
		LoadedObjectVertexNormalTexture vehicleBox;
		LoadedObjectVertexNormalTexture vehicleWheel;
		LoadedObjectVertexNormalTexture boxTexture;
		LoadedObjectVertexNormalTexture pailTexture;
		
		public void onDrawFrame(GL10 gl) {//绘制一帧画面的方法
			//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
            if(cam%2==0)//使用车后跟随的视角
            car.updateCamera2();  
            else if(cam%2==1)
            car.updateCamera1();//使用俯视的视角
           // pm.drawSelf(land); 
            MatrixState.pushMatrix();//保护现场
        	        	 
        	car.carTurn(Constant.Angle*60.0f*-1);//设置车的前轮方向
        	car.drawSelf();//绘制汽车
        	box1.drawSelf();//绘制箱子
        	box2.drawSelf();
        	box3.drawSelf();
        	box4.drawSelf();
        	box5.drawSelf();
        	box6.drawSelf();
        	pail1.drawSelf();
        	pail2.drawSelf();  
        	pail3.drawSelf();
        	
        	MatrixState.popMatrix(); //恢复现场 
        	tf.drawSelf(land);//绘制地面
        	//物理世界模拟计算
        	dynamicsWorld.stepSimulation(1.0f/60, 1);
		}
		public void onSurfaceChanged(GL10 gl, int width, int height) {
			GLES30.glViewport(0,0, width, height);
			float ratio=(float)width/height;
			MatrixState.setProjectFrustum(-ratio,ratio,-1,1,1,1000);
			Width=width;
			Hight=height;
		}
		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	       	//设置屏幕背景色RGBA
            GLES30.glClearColor(0.0f,0.0f,0.0f,1.0f);    
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            //打开背面剪裁   
            GLES30.glEnable(GLES30.GL_CULL_FACE);
            //初始化变换矩阵
            MatrixState.setInitStack();            
			land=initTexture(R.drawable.grass,MySurfaceView.this);//加载草皮纹理
			vehicleTextureId=initTexture(R.drawable.car, MySurfaceView.this);
			boxId=initTexture(R.drawable.box, MySurfaceView.this);  
			pailId=initTexture(R.drawable.pail, MySurfaceView.this);
			//加载车身obj模型
			vehicleBox=LoadUtil.loadFromFile("carBox.obj",MySurfaceView.this.getResources(),MySurfaceView.this);
			vehicleWheel=LoadUtil.loadFromFile("carWheel.obj",MySurfaceView.this.getResources(),MySurfaceView.this);
			boxTexture=LoadUtil.loadFromFile("cube.obj",MySurfaceView.this.getResources(),MySurfaceView.this);
			pailTexture=LoadUtil.loadFromFile("tong.obj",MySurfaceView.this.getResources(),MySurfaceView.this);
			//新建汽车对象
			car=new Car(dynamicsWorld, vehicleBox, vehicleWheel, vehicleTextureId,carStartPoint);
			//新建箱子对象
			box1=new Box(1,   0.6f, 10,0,dynamicsWorld, boxTexture, boxId,rbShape);
			box2=new Box(0,   0.6f, 10,0, dynamicsWorld, boxTexture, boxId,rbShape);
			box3=new Box(2,   0.6f, 10,0, dynamicsWorld, boxTexture, boxId,rbShape);
			box4=new Box(0.5f,1.7f, 10,0,dynamicsWorld, boxTexture, boxId,rbShape);
			box5=new Box(1.5f,1.7f, 10,0, dynamicsWorld, boxTexture, boxId,rbShape);
			box6=new Box(1,   2.7f, 10,0, dynamicsWorld, boxTexture, boxId,rbShape);
			pail1=new Pail(-3,1.1f, 10,0,dynamicsWorld, pailTexture, pailId,rbShape_pail);
			pail2=new Pail(-5,1.1f, 10,0,dynamicsWorld, pailTexture, pailId,rbShape_pail);
			pail3=new Pail(-4,3.2f, 10, 0,dynamicsWorld, pailTexture, pailId,rbShape_pail);			
			flag=true;//设置标志位，表明可以触控
			tf=new TextFloor(30,0,MySurfaceView.this);//新建绘制地面的对象  
		}	
	}
  	public static int initTexture(int drawableId,MySurfaceView gsv)//textureId
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
        InputStream is = gsv.getResources().openRawResource(drawableId);
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
	   	GLUtils.texImage2D
	    (
	    		GLES30.GL_TEXTURE_2D, //纹理类型
	     		0, 
	     		GLUtils.getInternalFormat(bitmapTmp), 
	     		bitmapTmp, //纹理图像
	     		//GLUtils.getType(bitmapTmp), 
	     		0 //纹理边框尺寸
	     );

	    bitmapTmp.recycle(); 		  //纹理加载成功后释放图片
        return textureId; 
	}
 	public void initWorld()//初始化物理世界的方法
	{
 		
		CollisionConfiguration collisionConfiguration = new DefaultCollisionConfiguration();		
		CollisionDispatcher dispatcher = new CollisionDispatcher(collisionConfiguration);		
		javax.vecmath.Vector3f worldAabbMin = new javax.vecmath.Vector3f(-10000, -10000, -10000);
		javax.vecmath.Vector3f worldAabbMax = new javax.vecmath.Vector3f(10000, 10000, 10000);
		int maxProxies = 102400;
		AxisSweep3 overlappingPairCache =new AxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
		SequentialImpulseConstraintSolver solver = new SequentialImpulseConstraintSolver();
		dynamicsWorld = new DiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver,collisionConfiguration);
		dynamicsWorld.setGravity(new javax.vecmath.Vector3f(0,-10,0));
		
		//创建地面碰撞形状
		CollisionShape groundShape = new BoxShape(new javax.vecmath.Vector3f(30f, 0f, 30f));
		Transform groundTransform = new Transform();//创建刚体的初始变换对象
		groundTransform.setIdentity();//对初始变换对象初始化
		groundTransform.origin.set(0, 0, 0);//移动刚体
		//调用localCreateRigidBody方法创建地面刚体对象并将其加入物理世界
		localCreateRigidBody(0, groundTransform, groundShape);
		//创建箱子所需的长方体盒碰撞形状
		rbShape = new BoxShape(new javax.vecmath.Vector3f(v_x/2, v_y/2, v_z/2));
		//创建油桶所需的圆柱碰撞形状
		rbShape_pail = new CylinderShape(new javax.vecmath.Vector3f(v_xp/2, v_yp/2, v_zp/2));
	}
 	//创建地面刚体的方法
	public RigidBody localCreateRigidBody(float mass, Transform startTransform, CollisionShape shape) {
		// rigidbody is dynamic if and only if mass is non zero, otherwise static
		boolean isDynamic = (mass != 0f);//物体是否可以移动
		//惯性结果向量
		javax.vecmath.Vector3f localInertia = new javax.vecmath.Vector3f(0f, 0f, 0f);
		if (isDynamic) {//如果物体可移动
			shape.calculateLocalInertia(mass, localInertia);//计算惯性
		}
		// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		//创建刚体的运动状态对象
		DefaultMotionState myMotionState = new DefaultMotionState(startTransform);
		//创建刚体描述信息对象
		RigidBodyConstructionInfo cInfo = new RigidBodyConstructionInfo(mass, myMotionState, shape, localInertia);
		//新建刚体对象
		RigidBody body = new RigidBody(cInfo);
		//设置摩擦系数
		body.setFriction(0.8f);
		dynamicsWorld.addRigidBody(body);//把此刚体加入到物理世界中
		return body;
	}
}