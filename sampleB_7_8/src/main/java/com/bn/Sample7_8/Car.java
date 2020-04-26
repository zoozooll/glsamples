package com.bn.Sample7_8;

import javax.vecmath.AxisAngle4f;
import javax.vecmath.Quat4f;
import javax.vecmath.Vector3f;

import com.bn.Sample7_8.util.LoadedObjectVertexNormalTexture;
import com.bn.Sample7_8.util.MatrixState;
import com.bn.Sample7_8.util.SYSUtil;
import com.bulletphysics.collision.dispatch.CollisionObject;
import com.bulletphysics.collision.shapes.BoxShape;
import com.bulletphysics.collision.shapes.CollisionShape;
import com.bulletphysics.collision.shapes.CompoundShape;
import com.bulletphysics.dynamics.DynamicsWorld;
import com.bulletphysics.dynamics.RigidBody;
import com.bulletphysics.dynamics.RigidBodyConstructionInfo;
import com.bulletphysics.dynamics.vehicle.DefaultVehicleRaycaster;
import com.bulletphysics.dynamics.vehicle.RaycastVehicle;
import com.bulletphysics.dynamics.vehicle.VehicleTuning;
import com.bulletphysics.dynamics.vehicle.WheelInfo;
import com.bulletphysics.linearmath.DefaultMotionState;
import com.bulletphysics.linearmath.Transform;
public class Car {
	float v_x1 = 2.26f; //车体上部分宽
    float v_y1 = 0.8f;//车体上部分高
    float v_z1 =6.14f;//车体上部分宽长 
	float v_x2 = 2.36f; //车体下部分宽宽
    float v_y2 = 0.9f;//车体下部分高
    float v_z2 =1.57f;///车体下部分长
    
	DefaultVehicleRaycaster vehicleRayCaster; 	//交通工具参数对象
	RaycastVehicle rayVehicle;					//交通工具对象
	VehicleTuning tuning; 						//协调器
	DynamicsWorld dynamicsWorld;				//物理世界
	RigidBody carChassis;						//车身钢体
	
	float wheelRadius = 0.4f;//车轮半径
	float wheelWidth = 0.4f;//车轮宽度
	float gVehicleSteering = 0f;//转向装置(前轮)旋转角度
	float gVehicleEngineForce = 0;//引擎驱动力
	
    float wheelFrontX = 0.99f;	//前车轮到车身中轴线距离
    float wheelBackX = 1.1f;	//后车轮到车身中轴线距离
    float wheelFrontY = 1.925f;	//前车轮到车身原点纵向距离
    float wheelBackY = 1.848f;	//后车轮到车身原点纵向距离
    //////////车轮属性
	static final float wheelFriction = 1000;//轮子摩擦力
	static final float suspensionStiffness = 20.f;//悬架刚度
	static final float suspensionDamping = 2.3f;//车轮阻尼
	static final float suspensionCompression = 4.4f;//悬梁压缩系数
	static final float rollInfluence = 0.1f;//滚动影响值   
	//车身悬挂高度
	float suspensionRestLength = 0.9f;
	/////////摄像机属性
	Vector3f cameraPosition = new Vector3f(0f, 0f, 0f);	//摄像机位置
	Vector3f cameraTargetPosition = new Vector3f(0f, 0f, 0f); //摄像机目标点
	Vector3f cameraUp = new Vector3f(0f, 1f,0f);	//摄像机up向量		
	//轮胎向量
	final static Vector3f wheelDirectionCS0 = new Vector3f(0,-1,0);//轮胎垂直方向偏移向量
	final static Vector3f wheelAxleCS = new Vector3f(-1,0,0);	//车轮轴向量
	//车绘制对象
	VehicleDraw vd;
	//前进挡和倒档标志位
	boolean DRR = true;
    float camera_angle=0;//摄像机初始角度
    //此向量第1个值代表车身绕Y轴旋转的初始角度，第2、第3个值代表车身初始的XZ坐标
    com.bn.Sample7_8.util.Vector3f carStartPoint; 
	static final int rightIndex = 0;//交通工具坐标系统右轴向量索引
	static final int upIndex = 1;//交通工具坐标系统上轴向量索引
	static final int forwardIndex = 2; //交通工具坐标系统前轴向量索引  

	
	public Car(
		DynamicsWorld dynamicsWorld,	//物理世界
		LoadedObjectVertexNormalTexture vBox,//从Obj文件中加载的车身模型
		LoadedObjectVertexNormalTexture vWheel,//从Obj文件中加载的车轮模型
		int vehicleTextureId,//汽车的纹理Id
		com.bn.Sample7_8.util.Vector3f carStartPoint	//汽车初始位置
	){
		this.dynamicsWorld = dynamicsWorld;//初始化物理世界引用
		this.carStartPoint = carStartPoint;	//初始化汽车初始位置	
		//创建车身下面的长方体形状
		CollisionShape chassisShape_down = new BoxShape(new Vector3f(v_x1/2,v_y1/2,v_z1/2));
		//创建车身上面的长方体形状
		CollisionShape chassisShape_up = new BoxShape(new Vector3f(v_x2/2,v_y2/2,v_z2/2));
		//创建车身组合体
		CompoundShape compound = new CompoundShape();
		//localTrans 切实转移了相对于底盘的中心
		// localTrans effectively shifts the center of mass with respect to the chassis
		//车身下部长方体在车身组合体中的初始变换
		Transform localTrans1 = new Transform();
		localTrans1.setIdentity();//将变换对象初始化
		localTrans1.origin.set(0, 1, 0);//设置为向Y轴正方向平移1的变换
		//将车身下部分长方体碰撞形状加入车身组合体
		compound.addChildShape(localTrans1, chassisShape_down);
		//车身上部长方体在车身组合体中的初始变换
		Transform localTrans2=new Transform();
		localTrans2.setIdentity();//将变换对象初始化
		localTrans2.origin.set(0,1.4f,-0.4f);//设置Y轴平移1.4，Z轴平移-0.4的变换
		//将车身上部分长方体碰撞形状加入车身组合体
		compound.addChildShape(localTrans2, chassisShape_up);	
		
		//车身组合体的初始变换
		Transform tr = new Transform();
		tr.setIdentity();//将变换对象初始化		
		tr.origin.set(carStartPoint.y,0, carStartPoint.z);//设置平移变换，平移到汽车初始位置
		
		if(carStartPoint.x!=0)//若车身初始旋转角不为0
		{
			Quat4f q = new Quat4f(0,0,0,1f);//新建一个四元数对象
			//新建一个轴旋转数据对象，前三个值表示旋转轴向量，最后一个值表示旋转角度
			AxisAngle4f aa = new AxisAngle4f(0, 1, 0, carStartPoint.x);
			q.set(aa);//将轴旋转数据转换为四元数
			tr.setRotation(q);//将四元数表示的旋转加入变换对象
		}

		
		//创建车身刚体
		carChassis = localCreateRigidBody(500, tr, compound);//设置用户信息用于检测碰撞 
		//设置用户信息用于检测碰撞
		carChassis.setUserPointer(1);
		
		//重置场景
		//clientResetScene();
		//创建交通工具
		{
			//创建交通工具
			tuning = new VehicleTuning();//创建协调器
			//创建交通工具参数对象
			vehicleRayCaster = new DefaultVehicleRaycaster(dynamicsWorld);
			//创建表示场景中汽车的交通工具对象
			rayVehicle = new RaycastVehicle(tuning, carChassis, vehicleRayCaster);

			//设置交通工具不冻结
			carChassis.setActivationState(CollisionObject.DISABLE_DEACTIVATION);
			//将交通工具对象添加进物理世界
			dynamicsWorld.addVehicle(rayVehicle);
			//车轮连接点高度
			float connectionHeight = 1.2f;

			// choose coordinate system
			//设置交通工具坐标系统的三个坐标轴的向量索引
			rayVehicle.setCoordinateSystem(rightIndex, upIndex, forwardIndex);
			//两个前轮
			boolean isFrontWheel = true;//是否为前轮标志位
			//创建第1个前轮的连接点对象
			Vector3f connectionPointCS0 = new Vector3f(wheelFrontX - (0.3f * wheelWidth), connectionHeight, wheelFrontY - wheelRadius);
			
			//添加第1个前轮
			rayVehicle.addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel);
			//第2个前轮的连接点数据
			connectionPointCS0.set(-wheelFrontX + (0.3f * wheelWidth), connectionHeight, wheelFrontY - wheelRadius);
			
			//添加第2个前轮
			rayVehicle.addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel);
			//第1个后轮的连接点数据
			connectionPointCS0.set(-wheelBackX + (0.3f * wheelWidth), connectionHeight, -wheelBackY + wheelRadius);
			
			isFrontWheel = false;//是否为前轮标志位
			//添加第1个后轮
			rayVehicle.addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel);
			//第2个后轮的连接点数据
			connectionPointCS0.set(wheelBackX - (0.3f * wheelWidth), connectionHeight, -wheelBackY + wheelRadius);
			 //添加第2个后轮
			rayVehicle.addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, tuning, isFrontWheel);
			//遍历所有车轮
			for (int i = 0; i < rayVehicle.getNumWheels(); i++) {
				WheelInfo wheel = rayVehicle.getWheelInfo(i);//获取车轮信息
				wheel.suspensionStiffness = suspensionStiffness; //设置悬架刚度
				wheel.wheelsDampingRelaxation = suspensionDamping; //设置车轮的阻尼系数
				wheel.wheelsDampingCompression = suspensionCompression;//设置悬梁压缩系数
				wheel.frictionSlip = wheelFriction;//设置轮胎摩擦系数
				wheel.rollInfluence = rollInfluence;//设置滚动影响值
			}
		

		}
		//创建汽车绘制对象
		vd = new VehicleDraw(rayVehicle, vBox,//车身
				vWheel,//车轮
				vehicleTextureId);
	}
	
	public void carMove_go()//汽车前进的方法
	{
		float gEngineForce = 3000;//引擎力	
		int wheelIndex = 2;//需要设置引擎力的车轮索引
		rayVehicle.applyEngineForce(gEngineForce,wheelIndex);//给指定车轮设置引擎力
		rayVehicle.setBrake(0,wheelIndex);//将指定车轮的刹车力设置为0
		wheelIndex = 3;//另一个需要设置引擎力的车轮索引
		rayVehicle.applyEngineForce(gEngineForce,wheelIndex);//给指定车轮设置引擎力
		rayVehicle.setBrake(0,wheelIndex);//将另一指定车轮的刹车力设置为0
	}
	public void carMove_back()//汽车后退的方法
	{
		float gEngineForce= -3000;//引擎力
		int wheelIndex = 2;//需要设置引擎力的车轮索引
		rayVehicle.applyEngineForce(gEngineForce,wheelIndex);//给指定车轮设置引擎力
		rayVehicle.setBrake(0,wheelIndex);//将指定车轮的刹车力设置为0
		wheelIndex = 3;//另一个需要设置引擎力的车轮索引
		rayVehicle.applyEngineForce(gEngineForce,wheelIndex);//给指定车轮设置引擎力
		rayVehicle.setBrake(0,wheelIndex);//将另一指定车轮的刹车力设置为0
	}
	public void carKong() {//汽车驻车制动方法
		
		float gEngineForce = 0.0f;//引擎力
		int wheelIndex = 2;//需要设置引擎力的车轮索引
		rayVehicle.applyEngineForce(gEngineForce,wheelIndex);//给指定车轮设置引擎力
		rayVehicle.setBrake(60,wheelIndex);//将指定车轮的刹车力设置为60
		wheelIndex = 3;//另一个需要设置引擎力的车轮索引
		rayVehicle.applyEngineForce(gEngineForce,wheelIndex);//给指定车轮设置引擎力
		rayVehicle.setBrake(60,wheelIndex);//将另一指定车轮的刹车力设置为60
		
	}

	public void carBreak() {//汽车刹车方法
		int wheelIndex = 2;//需要设置刹车力的车轮索引
		rayVehicle.setBrake(100f,wheelIndex);//将指定车轮的刹车力设置为100
		wheelIndex = 3;//另一个需要设置刹车力的车轮索引
		rayVehicle.setBrake(100f,wheelIndex);//将另一指定车轮的刹车力设置为100
	}
	public void carTurn(float angle){//汽车转弯的方法
		camera_angle=angle;//设置当前转弯角度
		int wheelIndex = 0;//需要设置转弯角度的车轮索引
		rayVehicle.setSteeringValue(angle/900,wheelIndex);//对指定车轮设置旋转角度
		wheelIndex = 1;//另一个需要设置转弯角度的车轮索引
		rayVehicle.setSteeringValue(angle/900,wheelIndex);//对另一指定车轮设置旋转角度
	}

	public void clientResetScene() {
		Transform tr = new Transform();
		tr.setIdentity();
		tr.origin.set(carStartPoint.y,1, carStartPoint.z);

		if(carStartPoint.x!=0)
		{
			Quat4f q = new Quat4f(0,0,0,1f);
			AxisAngle4f aa = new AxisAngle4f(0, 1, 0, carStartPoint.x);
			q.set(aa);
			tr.setRotation(q);
		}
		carChassis.setCenterOfMassTransform(tr); //设置质量中心
		carChassis.setLinearVelocity(new Vector3f(0, 0, 0));//设置线速度
		carChassis.setAngularVelocity(new Vector3f(0, 0, 0));//设置角速度
		dynamicsWorld.getBroadphase().getOverlappingPairCache().cleanProxyFromPairs(carChassis.getBroadphaseHandle(), dynamicsWorld.getDispatcher());
		if (rayVehicle != null) {
			rayVehicle.resetSuspension();
			for (int i = 0; i < rayVehicle.getNumWheels(); i++) {
				// synchronize the wheels with the (interpolated) chassis worldtransform
				// 同步轮与底盘变换
				rayVehicle.updateWheelTransform(i, true);
			}
			
		}
	}
	public RigidBody localCreateRigidBody(float mass, Transform startTransform,
			CollisionShape shape) {
		boolean isDynamic = (mass != 0f);
		Vector3f localInertia = new Vector3f(0f, 0f, 0f);
		if (isDynamic) {
			shape.calculateLocalInertia(mass, localInertia);
		}
		// using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		DefaultMotionState myMotionState = new DefaultMotionState(startTransform);
		RigidBodyConstructionInfo cInfo = new RigidBodyConstructionInfo(mass, myMotionState, shape, localInertia);
		RigidBody body = new RigidBody(cInfo);
		dynamicsWorld.addRigidBody(body);
		return body;
	}
	public void drawSelf() {
		vd.drawSelf();
	}
	public void updateCamera1()
	{
		Transform chassisWorldTrans = new Transform();
		carChassis.getMotionState().getWorldTransform(chassisWorldTrans);	//获得车身变换
		
		float[] fa=new float[4];
		Quat4f ro=chassisWorldTrans.getRotation(new Quat4f());//获取当前变换的旋转信息
		fa=SYSUtil.fromSYStoAXYZ(ro);//将四元数转换成AXYZ的形式  
		//System.out.println("/////////////////////"+fa[0]+"////"+fa[1]+"////"+fa[2]+"////"+fa[3]+"///////"+camera_angle);
		com.bn.Sample7_8.util.Vector3f start= new com.bn.Sample7_8.util.Vector3f(0, 0,6);
		com.bn.Sample7_8.util.Vector3f end= new com.bn.Sample7_8.util.Vector3f(0, 0,0);
		com.bn.Sample7_8.util.Vector3f end2;
		if(0<=fa[0]&&fa[0]<=120&&fa[2]<0)
		{
			end = start.roty(Math.toRadians(-fa[0]));
		}
		else
		{
			end = start.roty(Math.toRadians(fa[0]));
		}

		//if(camera_angle<=4&&camera_angle>=-4)//摄像机跟随
		{
			end2=end;
		}
		//else
		//end2=end.roty(Math.toRadians(-camera_angle*rayVehicle.getCurrentSpeedKmHour()/2000));
		com.bn.Sample7_8.util.Vector3f cam = new com.bn.Sample7_8.util.Vector3f(end2.x*1.0f,end2.y,end2.z*1.0f);
		cameraTargetPosition.set(chassisWorldTrans.origin.x,chassisWorldTrans.origin.y,chassisWorldTrans.origin.z);	//设置相机目标点
		cameraPosition.set(chassisWorldTrans.origin.x,chassisWorldTrans.origin.y+12,chassisWorldTrans.origin.z);//计算摄像机位置
		
		MatrixState.setInitStack();
//		 MatrixState.scale(-1, 1, 1);
		MatrixState.setCamera(cameraPosition.x,cameraPosition.y,cameraPosition.z,
				  cameraTargetPosition.x,cameraTargetPosition.y, cameraTargetPosition.z,
				  cam.x,cam.y,cam.z);

	}
	//车后跟随
	public void updateCamera2() {
		Transform chassisWorldTrans = new Transform();
		carChassis.getMotionState().getWorldTransform(chassisWorldTrans);	//获得车身变换
		
		float[] fa=new float[4];
		Quat4f ro=chassisWorldTrans.getRotation(new Quat4f());//获取当前变换的旋转信息
		fa=SYSUtil.fromSYStoAXYZ(ro);//将四元数转换成AXYZ的形式  
		//System.out.println("/////////////////////"+fa[0]+"////"+fa[1]+"////"+fa[2]+"////"+fa[3]+"///////"+camera_angle);
		com.bn.Sample7_8.util.Vector3f start= new com.bn.Sample7_8.util.Vector3f(0, 0,6);
		com.bn.Sample7_8.util.Vector3f end= new com.bn.Sample7_8.util.Vector3f(0, 0,0);
		com.bn.Sample7_8.util.Vector3f end2;
		if(0<=fa[0]&&fa[0]<=120&&fa[2]<0)
		{
			end = start.roty(Math.toRadians(-fa[0]));
		}
		else
		{
			end = start.roty(Math.toRadians(fa[0]));
		}

		if(camera_angle<=4&&camera_angle>=-4)//摄像机跟随
		{
			end2=end;
		}
		else
		end2=end.roty(Math.toRadians(-camera_angle*rayVehicle.getCurrentSpeedKmHour()/2000));
		com.bn.Sample7_8.util.Vector3f cam = new com.bn.Sample7_8.util.Vector3f(-end2.x*1.0f,end2.y+4.0f,-end2.z*1.0f);
		cameraTargetPosition.set(chassisWorldTrans.origin.x+end2.x,chassisWorldTrans.origin.y+end2.y,chassisWorldTrans.origin.z+end2.z);	//设置相机目标点
		cameraPosition.set(chassisWorldTrans.origin.x+cam.x,chassisWorldTrans.origin.y+cam.y,chassisWorldTrans.origin.z+cam.z);//计算摄像机位置
		
		MatrixState.setInitStack();
//		 MatrixState.scale(-1, 1, 1);
		MatrixState.setCamera(cameraPosition.x,cameraPosition.y,cameraPosition.z,
				  cameraTargetPosition.x,cameraTargetPosition.y, cameraTargetPosition.z,
				  cameraUp.x,cameraUp.y,cameraUp.z);
	
	}

}