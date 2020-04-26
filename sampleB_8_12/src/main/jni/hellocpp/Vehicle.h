#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <vector>
#include "../myEncapsulation/TexBody.h"
#include "../Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "../Bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h"
#include "../Bullet/BulletDynamics/Vehicle/btRaycastVehicle.h"
#include "../Bullet/btBulletDynamicsCommon.h"

#include "../myEncapsulation/TexCuboid.h"
#include "../myEncapsulation/TexCylinderShape.h"
#include "../myEncapsulation/MatrixState.h"


#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;

class Vehicle
{

	btVehicleRaycaster*	m_vehicleRayCaster;//С���ص���ָ��
	btRaycastVehicle*	m_vehicle;//С����ָ��
	btCollisionShape*	m_wheelShape;//��ײ��״ָ��
	btRigidBody* m_carChassis;//������ָ��
	btDynamicsWorld* m_dynamicsWorld;//����������ָ��

	TexCuboid* tc;//������ָ��
	TexCylinderShape* tcs;//���ֻ�����ָ��

public:

	Vehicle(btDynamicsWorld* m_dynamicsWorld,
			int texId_box,
			int texId_ball);//���캯��
	void clientResetScene();//����������Ϣ
	void refresh();//���·���

	btVector3 getPostion();//��ȡС��λ��

	void carMove_go();//ǰ������
	void carMove_back();//���˷���
	void carKong();//�ƶ������㷽��
	void carBreak();//ɲ������

	void carRelease();//ת���ɲ����ʼ������
	void carMove_back_left();//����ת����
	void carMove_back_right();//����ת����
	void carMove_go_left();//ǰ��ת����
	void carMove_go_right();//����ת����

};

#endif
