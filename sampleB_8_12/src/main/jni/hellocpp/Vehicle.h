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

	btVehicleRaycaster*	m_vehicleRayCaster;//小车回调类指针
	btRaycastVehicle*	m_vehicle;//小车类指针
	btCollisionShape*	m_wheelShape;//碰撞形状指针
	btRigidBody* m_carChassis;//车身类指针
	btDynamicsWorld* m_dynamicsWorld;//物理世界类指针

	TexCuboid* tc;//车身类指针
	TexCylinderShape* tcs;//车轮绘制类指针

public:

	Vehicle(btDynamicsWorld* m_dynamicsWorld,
			int texId_box,
			int texId_ball);//构造函数
	void clientResetScene();//设置属性信息
	void refresh();//更新方法

	btVector3 getPostion();//获取小车位置

	void carMove_go();//前进方法
	void carMove_back();//后退方法
	void carKong();//制动力置零方法
	void carBreak();//刹车方法

	void carRelease();//转向和刹车初始化方法
	void carMove_back_left();//后左转方法
	void carMove_back_right();//后右转方法
	void carMove_go_left();//前左转方法
	void carMove_go_right();//后右转方法

};

#endif
