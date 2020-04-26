#include "Vehicle.h"

Vehicle::Vehicle(btDynamicsWorld* m_dynamicsWorld,
				int texId_box,
				int texId_ball)
{
	float scale = 2;//���Ŵ�С

	float cube_half_extents_x = 1 * scale;//������

	this->m_dynamicsWorld = m_dynamicsWorld;//��¼��������ָ��
	int rightIndex = 0;//��������ֵ
	int upIndex = 1;//��������ֵ
	int forwardIndex = 2;//ǰ������ֵ
	btVector3 wheelDirectionCS0(0,-1,0);//���ִ�ֱ����ƫ����
	btVector3 wheelAxleCS(-1,0,0);//����������
	float wheelRadius = 0.5f * scale;//���ְ뾶
	float wheelWidth = 0.4f * scale;//���ֿ��
	btScalar suspensionRestLength(0.4 * scale);//�����߶�
	float suspensionStiffness = 20.f;//��Ӳϵ��
	float suspensionDamping = 2.3f;//����ϵ��
	float suspensionCompression = 4.4f;//ѹ��ϵ��
	float rollInfluence = -0.1f;//ѹ��ϵ��
	float wheelFriction = 1000;//����Ħ��

	tc = new TexCuboid(m_dynamicsWorld,btVector3(cube_half_extents_x,0.5f * scale,2.f * cube_half_extents_x),1000,btVector3(0,0,0),0.5,0.5,1,
			texId_box,texId_box,texId_box,texId_box,texId_box,texId_box);//�����������


	m_carChassis = tc->getBody();//��ȡ�������ָ��

	tcs = new TexCylinderShape(
			texId_ball,//�ײ�Բ�ε�����id
			texId_ball,//����Բ�ε�����id
			texId_box,//Բ�����������id
			30,//�зַ���
			btVector3(wheelRadius,wheelWidth,wheelRadius)//������
			);//�������ֻ��ƶ���

	clientResetScene();//����������Ϣ

	btRaycastVehicle::btVehicleTuning	m_tuning;//��ͨ����Э����

	m_vehicleRayCaster = new btDefaultVehicleRaycaster(m_dynamicsWorld);//����Ĭ�ϵĽ�ͨ���߻ص������
	m_vehicle = new btRaycastVehicle(m_tuning,m_carChassis,m_vehicleRayCaster);//����С������

	///never deactivate the vehicle
	m_carChassis->setActivationState(DISABLE_DEACTIVATION);//���ó������Ϊ��̬��

	m_dynamicsWorld->addVehicle(m_vehicle);//��С��������ӵ�����������

	float connectionHeight = -0.25f * scale;//���Ӹ߶�
	bool isFrontWheel = true;//��ʾ�Ƿ�Ϊǰ��

	//choose coordinate system
	m_vehicle->setCoordinateSystem(rightIndex,upIndex,forwardIndex);//����С������ϵͳ

	//�����������
	//ǰ����λ������
	btVector3 connectionPointCS0(cube_half_extents_x-(0.3*wheelWidth),connectionHeight,2*cube_half_extents_x-wheelRadius);
	//��С�����ǰ����
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
	//ǰ����λ������
	connectionPointCS0 = btVector3(-cube_half_extents_x+(0.3*wheelWidth),connectionHeight,2*cube_half_extents_x-wheelRadius);
	//��С�����ǰ����
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

	//�������ǰ��
	isFrontWheel = false;//��ǰ�ֱ�־����Ϊ���ֱ�־
	//������λ������
	connectionPointCS0 = btVector3(-cube_half_extents_x+(0.3*wheelWidth),connectionHeight,-2*cube_half_extents_x+wheelRadius);
	//��С����Ӻ�����
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);
	//������λ������
	connectionPointCS0 = btVector3(cube_half_extents_x-(0.3*wheelWidth),connectionHeight,-2*cube_half_extents_x+wheelRadius);
	//��С����Ӻ�����
	m_vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_tuning,isFrontWheel);

	for (int i=0;i<m_vehicle->getNumWheels();i++)//�������еĳ���
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);//��ȡ������Ϣ
		wheel.m_suspensionStiffness = suspensionStiffness;//���ó��ֵļ�Ӳϵ��
		wheel.m_wheelsDampingRelaxation = suspensionDamping;//���ó��ֵ�����ϵ��
		wheel.m_wheelsDampingCompression = suspensionCompression;//���ó��ֵ�ѹ��ϵ��
		wheel.m_frictionSlip = wheelFriction;//���ó��ֵ�Ħ��ϵ��
		wheel.m_rollInfluence = rollInfluence;//���ó��ֵ�ѹ��ϵ��
	}

}

btVector3 Vehicle::getPostion()
{
	btTransform frame = this->m_carChassis->getWorldTransform();//��ȡС���ı任
	btVector3 pos = frame.getOrigin();//��ȡ�任��λ��
	return pos;//����λ������
}

void Vehicle::clientResetScene()
{
	m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());//�������ı任
	m_carChassis->setLinearVelocity(btVector3(0,0,0));//�������ٶ�
	m_carChassis->setAngularVelocity(btVector3(0,0,0));//���ý��ٶ�
	m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(),m_dynamicsWorld->getDispatcher());
}

void Vehicle::refresh()
{
	MatrixState::pushMatrix();//�����ֳ�

	for (int i=0;i<m_vehicle->getNumWheels();i++)//�������еĳ���
	{
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);//���³���
		//draw wheels (cylinders)
		MatrixState::pushMatrix();//�����ֳ�

		m_vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(MatrixState::currMatrix);//���þ���

		MatrixState::pushMatrix();//�����ֳ�
		MatrixState::rotate(90,0,0,1);//��z����ת90��
		tcs->drawSelfNoBody();//���Ƴ���
		MatrixState::popMatrix();//�ָ��ֳ�

		MatrixState::popMatrix();//�ָ��ֳ�
	}

	MatrixState::pushMatrix();//�����ֳ�
	tc->drawSelf();//���Ƴ���
	MatrixState::popMatrix();//�ָ��ֳ�

	MatrixState::popMatrix();//�ָ��ֳ�
}


void Vehicle::carMove_go()
{
	this->carRelease();//ת���ɲ����ʼ��

	float gEngineForce = 3000;//�ƶ���
	int wheelIndex = 2;//����������ֵ
	m_vehicle->applyEngineForce(gEngineForce,wheelIndex);//���ú�����������
	wheelIndex = 3;//����������ֵ
	m_vehicle->applyEngineForce(gEngineForce,wheelIndex);//���ú�����������
}
void Vehicle::carMove_go_right()
{
	carMove_go();//���ó���ǰ������
	m_vehicle->setSteeringValue(-0.3f,0);//����ǰ����ת��
	m_vehicle->setSteeringValue(-0.3f,1);//����ǰ����ת��
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

	float gEngineForce = 0.0f;//�ƶ���
	for(int wheelIndex = 0; wheelIndex < m_vehicle->getNumWheels(); wheelIndex++)//�������еĳ���
	{
		m_vehicle->applyEngineForce(gEngineForce,wheelIndex);//���ó��ֵ��ƶ���
	}
	this->carBreak();//����ɲ������

}

void Vehicle::carRelease(){
	for(int wheelIndex = 0; wheelIndex < m_vehicle->getNumWheels(); wheelIndex++)//�������еĳ���
	{
		m_vehicle->setBrake(0.0f,wheelIndex);//����ɲ��ϵ��Ϊ0
		m_vehicle->setSteeringValue(0.0f,wheelIndex);//���ó���ת��ϵ��
	}
}

void Vehicle::carBreak() {

	this->carRelease();//����ת���ɲ����ʼ������

	for(int wheelIndex = 0; wheelIndex < m_vehicle->getNumWheels(); wheelIndex++)//�������еĳ���
	{
		m_vehicle->setBrake(50.0f,wheelIndex);//����ɲ��ϵ��
	}
}
