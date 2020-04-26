#include "Vehicle.h"

Vehicle::Vehicle(btDynamicsWorld* m_dynamicsWorld,
				int texId_box,
				int texId_ball)
{
	float scale = 2;//缩放大小

	float cube_half_extents_x = 1 * scale;//车身半宽

	this->m_dynamicsWorld = m_dynamicsWorld;//记录物理世界指针
	int rightIndex = 0;//右轴索引值
	int upIndex = 1;//上轴索引值
	int forwardIndex = 2;//前轴索引值
	btVector3 wheelDirectionCS0(0,-1,0);//车轮垂直方向偏移量
	btVector3 wheelAxleCS(-1,0,0);//车轮轴向量
	float wheelRadius = 0.5f * scale;//车轮半径
	float wheelWidth = 0.4f * scale;//车轮宽度
	btScalar suspensionRestLength(0.4 * scale);//悬浮高度
	float suspensionStiffness = 20.f;//坚硬系数
	float suspensionDamping = 2.3f;//阻尼系数
	float suspensionCompression = 4.4f;//压缩系数
	float rollInfluence = -0.1f;//压力系数
	float wheelFriction = 1000;//车轮摩擦

	tc = new TexCuboid(m_dynamicsWorld,btVector3(cube_half_extents_x,0.5f * scale,2.f * cube_half_extents_x),1000,btVector3(0,0,0),0.5,0.5,1,
			texId_box,texId_box,texId_box,texId_box,texId_box,texId_box);//创建车身对象


	m_carChassis = tc->getBody();//获取车身刚体指针

	tcs = new TexCylinderShape(
			texId_ball,//底部圆形的纹理id
			texId_ball,//顶部圆形的纹理id
			texId_box,//圆柱侧面的纹理id
			30,//切分份数
			btVector3(wheelRadius,wheelWidth,wheelRadius)//半区域
			);//创建车轮绘制对象

	clientResetScene();//设置属性信息

	btRaycastVehicle::btVehicleTuning	m_tuning;//交通工具协调器

	m_vehicleRayCaster = new btDefaultVehicleRaycaster(m_dynamicsWorld);//创建默认的交通工具回调类对象
	m_vehicle = new btRaycastVehicle(m_tuning,m_carChassis,m_vehicleRayCaster);//创建小车对象

	///never deactivate the vehicle
	m_carChassis->setActivationState(DISABLE_DEACTIVATION);//设置车身刚体为动态的

	m_dynamicsWorld->addVehicle(m_vehicle);//将小车对象添加到物理世界中

	float connectionHeight = -0.25f * scale;//连接高度
	bool isFrontWheel = true;//表示是否为前轮

	//choose coordinate system
	m_vehicle->setCoordinateSystem(rightIndex,upIndex,forwardIndex);//设置小车坐标系统

	//添加两个后轮
	//前右轮位置向量
	btVector3 connectionPointCS0(cube_half_extents_x-(0.3*wheelWidth),connectionHeight,2*cube_half_extents_x-wheelRadius);
	//给小车添加前右轮
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
	//前左轮位置向量
	connectionPointCS0 = btVector3(-cube_half_extents_x+(0.3*wheelWidth),connectionHeight,2*cube_half_extents_x-wheelRadius);
	//给小车添加前左轮
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

	//添加两个前轮
	isFrontWheel = false;//将前轮标志设置为后轮标志
	//后左轮位置向量
	connectionPointCS0 = btVector3(-cube_half_extents_x+(0.3*wheelWidth),connectionHeight,-2*cube_half_extents_x+wheelRadius);
	//给小车添加后左轮
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
	//后右轮位置向量
	connectionPointCS0 = btVector3(cube_half_extents_x-(0.3*wheelWidth),connectionHeight,-2*cube_half_extents_x+wheelRadius);
	//给小车添加后右轮
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

	for (int i=0;i<m_vehicle->getNumWheels();i++)//遍历所有的车轮
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);//获取车轮信息
		wheel.m_suspensionStiffness = suspensionStiffness;//设置车轮的坚硬系数
		wheel.m_wheelsDampingRelaxation = suspensionDamping;//设置车轮的阻尼系数
		wheel.m_wheelsDampingCompression = suspensionCompression;//设置车轮的压缩系数
		wheel.m_frictionSlip = wheelFriction;//设置车轮的摩擦系数
		wheel.m_rollInfluence = rollInfluence;//设置车轮的压力系数
	}

}

btVector3 Vehicle::getPostion()
{
	btTransform frame = this->m_carChassis->getWorldTransform();//获取小车的变换
	btVector3 pos = frame.getOrigin();//获取变换的位置
	return pos;//返回位置坐标
}

void Vehicle::clientResetScene()
{
	m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());//设置质心变换
	m_carChassis->setLinearVelocity(btVector3(0,0,0));//设置线速度
	m_carChassis->setAngularVelocity(btVector3(0,0,0));//设置角速度
	m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(),m_dynamicsWorld->getDispatcher());
}

void Vehicle::refresh()
{
	MatrixState::pushMatrix();//保护现场

	for (int i=0;i<m_vehicle->getNumWheels();i++)//遍历所有的车轮
	{
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);//更新车轮
		//draw wheels (cylinders)
		MatrixState::pushMatrix();//保护现场

		m_vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(MatrixState::currMatrix);//设置矩阵

		MatrixState::pushMatrix();//保护现场
		MatrixState::rotate(90,0,0,1);//绕z轴旋转90度
		tcs->drawSelfNoBody();//绘制车轮
		MatrixState::popMatrix();//恢复现场

		MatrixState::popMatrix();//恢复现场
	}

	MatrixState::pushMatrix();//保护现场
	tc->drawSelf();//绘制车身
	MatrixState::popMatrix();//恢复现场

	MatrixState::popMatrix();//恢复现场
}


void Vehicle::carMove_go()
{
	this->carRelease();//转向和刹车初始化

	float gEngineForce = 3000;//制动力
	int wheelIndex = 2;//后左轮索引值
	m_vehicle->applyEngineForce(gEngineForce,wheelIndex);//设置后左轮驱动力
	wheelIndex = 3;//后右轮索引值
	m_vehicle->applyEngineForce(gEngineForce,wheelIndex);//设置后右轮驱动力
}
void Vehicle::carMove_go_right()
{
	carMove_go();//调用车轮前进方法
	m_vehicle->setSteeringValue(-0.3f,0);//设置前右轮转向
	m_vehicle->setSteeringValue(-0.3f,1);//设置前左轮转向
}
void Vehicle::carMove_go_left()
{
	carMove_go();

	m_vehicle->setSteeringValue(0.3f,0);
	m_vehicle->setSteeringValue(0.3f,1);
}
void Vehicle::carMove_back()
{
	this->carRelease();
	float gEngineForce= -3000;
	int wheelIndex = 2;
	m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
	wheelIndex = 3;
	m_vehicle->applyEngineForce(gEngineForce,wheelIndex);
}
void Vehicle::carMove_back_right()
{
	carMove_back();

	m_vehicle->setSteeringValue(-0.3f,0);
	m_vehicle->setSteeringValue(-0.3f,1);
}
void Vehicle::carMove_back_left()
{
	carMove_back();

	m_vehicle->setSteeringValue(0.3f,0);
	m_vehicle->setSteeringValue(0.3f,1);
}

void Vehicle::carKong() {

	float gEngineForce = 0.0f;//制动力
	for(int wheelIndex = 0; wheelIndex < m_vehicle->getNumWheels(); wheelIndex++)//遍历所有的车轮
	{
		m_vehicle->applyEngineForce(gEngineForce,wheelIndex);//设置车轮的制动力
	}
	this->carBreak();//调用刹车方法

}

void Vehicle::carRelease(){
	for(int wheelIndex = 0; wheelIndex < m_vehicle->getNumWheels(); wheelIndex++)//遍历所有的车轮
	{
		m_vehicle->setBrake(0.0f,wheelIndex);//设置刹车系数为0
		m_vehicle->setSteeringValue(0.0f,wheelIndex);//设置车轮转动系数
	}
}

void Vehicle::carBreak() {

	this->carRelease();//调用转向和刹车初始化方法

	for(int wheelIndex = 0; wheelIndex < m_vehicle->getNumWheels(); wheelIndex++)//遍历所有的车轮
	{
		m_vehicle->setBrake(50.0f,wheelIndex);//设置刹车系数
	}
}
