#include <stdio.h>
#include <math.h>

#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <vector>
#include "Bullet/LinearMath/btAlignedObjectArray.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/BulletSoftBody/btSoftBody.h"
#include "Bullet/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet/BulletSoftBody/btSoftBodySolvers.h"
#include "Bullet/BulletSoftBody/btDefaultSoftBodySolver.h"
#include "Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "myEncapsulation/MatrixState.h"
#include "../myEncapsulation/TexCuboid.h"
#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/TexPlane.h"
#include "../myEncapsulation/TexBall.h"
#include "../myEncapsulation/TexCapsule.h"
#include "../myEncapsulation/TexCone.h"
#include "../myEncapsulation/TexCylinderShape.h"
#include "../myEncapsulation/FileUtil.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;

//地形  光线投射
extern "C" {

	int boxTexId;	//立方体纹理id
	int ballTexId;	//球体纹理id
	int planeTexId;	//地面纹理id
	int wood_binId;//门id
	int floorId;//室内地面纹理id
	int muwenId;

	float UNIT_SIZE = 1.0;	//单位长度

	vector<TexBody*> tca;	//存储刚体封装类对象的vector（包括 立方体，球体，地面）
	TexBody* tbTemp;		//临时刚体封装类对象

	int addBodyId = 0;		//添加刚体的id

	//摄像机的位置坐标
	float cx = 0;
	float cy = 30;
	float cz = 30;

	//up向量
	float upx = 0;
	float upy = 1;
	float upz = 0;

	btDefaultCollisionConfiguration* m_collisionConfiguration;//系统默认碰撞检测配置信息
	btCollisionDispatcher*	m_dispatcher;	//碰撞检测算法分配器
	btBroadphaseInterface*	m_broadphase;	//宽度阶段碰撞检测
	btConstraintSolver*		m_solver;		//碰撞约束解决器
	btDynamicsWorld*		m_dynamicsWorld;//物理世界

	//设置摄像机位置
	void setCamera(float x,float y,float z,float upxt,float upyt,float upzt)
	{
		cx = x;
		cy = y;
		cz = z;
		upx = upxt;
		upy = upyt;
		upz = upzt;
	}

	//初始化物理世界的方法
	void initWorld()
	{

		///collision configuration contains default setup for memory, collision setup
		//默认的碰撞检测配置信息 包含 默认的碰撞检测频率
		m_collisionConfiguration = new btDefaultCollisionConfiguration();//创建默认的碰撞检测配置信息对象
		//m_collisionConfiguration->setConvexConvexMultipointIterations();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		//创建碰撞检测算法分配器对象，其功能为扫描所有的碰撞检测对，并确定适用的检测策略对应的算法
		m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

		m_broadphase = new btSimpleBroadphase();
				//btDbvtBroadphase();//创建宽度阶段碰撞检测对象

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		//默认的约束解决器
		btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver();//创建推动约束解决器对象
		m_solver = sol;

		//创建离散的物理世界
		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
		m_dynamicsWorld->setGravity(btVector3(0,-10,0));//设置重力

	}

	btSliderConstraint* m_bsc_right;//右侧的滑动关节
	btSliderConstraint* m_bsc_left;//左侧的滑动关节
	btSliderConstraint* m_bsc_top;//上部的滑动关节
	btSliderConstraint* m_bsc_buttom;//下部的滑动关节
	btSliderConstraint* m_bsc_far;//背后的滑动关节
	btSliderConstraint* m_bsc_near;//前侧的滑动关节

	btRigidBody* m_body_top;//上方方块刚体对象指针
	btRigidBody* m_body_buttom;//上方方块刚体对象指针

	TexCylinderShape* tcs0;

	void initCreateBodys()
	{
		btCollisionShape* planeShape=new btStaticPlaneShape(btVector3(0, 1, 0), 0);//创建地面形状
		//创建地面对象
		tbTemp = new TexPlane(UNIT_SIZE*100,planeTexId,planeShape,m_dynamicsWorld,
				0,-20,0,
				0.4,0.8);
		//将地面加入到列表中
		tca.push_back(tbTemp);

		{

			tcs0 = new TexCylinderShape(
					muwenId,//底部圆形的纹理id
					muwenId,//顶部圆形的纹理id
					muwenId,//圆柱侧面的纹理id
					30,//切分份数  30
					btVector3(1.25f*UNIT_SIZE,20.0f,1.25f*UNIT_SIZE)//半区域
					);//初始圆柱侧面平行于y轴，圆形是平行xoz界面上的

		}

		{
			btRigidBody* bodyA = 0;//创建滑轮约束的刚体指针bodyA
			btRigidBody* bodyB = 0;//创建滑轮约束的刚体指针bodyB
			btTransform transformA;//创建刚体bodyA和滑轮关节的其中一个关节的变换
			btTransform transformB;//创建刚体bodyB和滑轮关节的另一个关节的变换
			{
				//创建中间静态的立方体
				tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(2*UNIT_SIZE,2*UNIT_SIZE,2*UNIT_SIZE),//长方体的半区域    赋值应为   (k1*UNIT_SIZE,k2*UNIT_SIZE,,k3*UNIT_SIZE)
								0.0f,//长方体的质量
								btVector3(0,0,0),//刚体的位置
								0.0,//恢复系数
								0.0,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,//前侧纹理图id
								boxTexId,//后侧纹理图id
								boxTexId,//上侧纹理图id
								boxTexId,//下侧纹理图id
								boxTexId,//左侧纹理图id
								boxTexId//右侧纹理图id
								);
				tca.push_back(tbTemp);//将新立方体加入到列表中
				bodyA = tbTemp->getBody();//给bodyA赋值
			}
			{
				//创建右侧的立方体
				tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(2*UNIT_SIZE,2*UNIT_SIZE,2*UNIT_SIZE),//长方体的半区域    赋值应为   (k1*UNIT_SIZE,k2*UNIT_SIZE,,k3*UNIT_SIZE)
								6.0f,//长方体的质量
								btVector3(10,0,0),//刚体的位置
								0.5,//恢复系数
								0.5,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,//前侧纹理图id
								boxTexId,//后侧纹理图id
								boxTexId,//上侧纹理图id
								boxTexId,//下侧纹理图id
								boxTexId,//左侧纹理图id
								boxTexId//右侧纹理图id
								);
				tca.push_back(tbTemp);//将新立方体加入到列表中
				bodyB = tbTemp->getBody();//给bodyB赋值
				bodyB->setActivationState(DISABLE_DEACTIVATION);//将刚体的动作状态设为动态

				transformA.setIdentity();//初始化变换transformA
				transformB.setIdentity();//初始化变换transformB
				transformA.setOrigin(btVector3(10,0,0));//设置节点位置
				m_bsc_right = new btSliderConstraint(*bodyA, *bodyB, transformA, transformB, true);//创建右侧滑动关节
				m_dynamicsWorld->addConstraint(m_bsc_right);//将关节添加到物理世界中
				m_bsc_right->setLowerLinLimit(-5.5f);//设置关节移动的最小距离	关节的左侧方向
				m_bsc_right->setUpperLinLimit(5.5f);//设置关节移动的最大距离  关节的右侧方向
			}
			{
				//创建左侧的立方体
				tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(2*UNIT_SIZE,2*UNIT_SIZE,2*UNIT_SIZE),//长方体的半区域    赋值应为   (k1*UNIT_SIZE,k2*UNIT_SIZE,,k3*UNIT_SIZE)
								6.0f,//长方体的质量
								btVector3(-10,0,0),//刚体的位置
								0.5,//恢复系数
								0.5,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,//前侧纹理图id
								boxTexId,//后侧纹理图id
								boxTexId,//上侧纹理图id
								boxTexId,//下侧纹理图id
								boxTexId,//左侧纹理图id
								boxTexId//右侧纹理图id
								);
				tca.push_back(tbTemp);//将新立方体加入到列表中
				bodyB = tbTemp->getBody();//给bodyB赋值
				bodyB->setActivationState(DISABLE_DEACTIVATION);//将刚体的动作状态设为动态

				transformA.setIdentity();//初始化变换transformA
				transformB.setIdentity();//初始化变换transformB
				transformA.setOrigin(btVector3(-10,0,0));//设置节点位置
				m_bsc_left = new btSliderConstraint(*bodyA, *bodyB, transformA, transformB, true);//创建左侧滑动关节
				m_dynamicsWorld->addConstraint(m_bsc_left);//将关节添加到物理世界中
				m_bsc_left->setLowerLinLimit(-5.5f);//设置关节移动的最小距离	关节的左侧方向
				m_bsc_left->setUpperLinLimit(5.5f);//设置关节移动的最大距离  关节的右侧方向
			}
			{
				//创建前方的立方体
				tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(2*UNIT_SIZE,2*UNIT_SIZE,2*UNIT_SIZE),//长方体的半区域    赋值应为   (k1*UNIT_SIZE,k2*UNIT_SIZE,,k3*UNIT_SIZE)
								6.0f,//长方体的质量
								btVector3(0,0,10),//刚体的位置
								0.5,//恢复系数
								0.5,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,//前侧纹理图id
								boxTexId,//后侧纹理图id
								boxTexId,//上侧纹理图id
								boxTexId,//下侧纹理图id
								boxTexId,//左侧纹理图id
								boxTexId//右侧纹理图id
								);
				tca.push_back(tbTemp);//将新立方体加入到列表中
				bodyB = tbTemp->getBody();//给bodyB赋值
				bodyB->setActivationState(DISABLE_DEACTIVATION);//将刚体的动作状态设为动态

				transformA.setIdentity();//初始化变换transformA
				transformB.setIdentity();//初始化变换transformB
				transformA.setOrigin(btVector3(0,0,10));//设置节点位置
				transformA.setRotation(btQuaternion(btVector3(0,1,0),SIMD_HALF_PI));//设置变换的旋转角
				transformB.setRotation(btQuaternion(btVector3(0,1,0),SIMD_HALF_PI));//设置变换的旋转角
				m_bsc_near = new btSliderConstraint(*bodyA, *bodyB, transformA, transformB, true);//创建前方滑动关节
				m_dynamicsWorld->addConstraint(m_bsc_near);//将关节添加到物理世界中
				m_bsc_near->setLowerLinLimit(-5.5f);//设置关节移动的最小距离	关节的左侧方向
				m_bsc_near->setUpperLinLimit(5.5f);//设置关节移动的最大距离  关节的右侧方向
			}
			{
				//创建后方的立方体
				tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(2*UNIT_SIZE,2*UNIT_SIZE,2*UNIT_SIZE),//长方体的半区域    赋值应为   (k1*UNIT_SIZE,k2*UNIT_SIZE,,k3*UNIT_SIZE)
								6.0f,//长方体的质量
								btVector3(0,0,-10),//刚体的位置
								0.5,//恢复系数
								0.5,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,//前侧纹理图id
								boxTexId,//后侧纹理图id
								boxTexId,//上侧纹理图id
								boxTexId,//下侧纹理图id
								boxTexId,//左侧纹理图id
								boxTexId//右侧纹理图id
								);
				tca.push_back(tbTemp);//将新立方体加入到列表中
				bodyB = tbTemp->getBody();//给bodyB赋值
				bodyB->setActivationState(DISABLE_DEACTIVATION);//将刚体的动作状态设为动态

				transformA.setIdentity();//初始化变换transformA
				transformB.setIdentity();//初始化变换transformB
				transformA.setOrigin(btVector3(0,0,-10));//设置节点位置
				transformA.setRotation(btQuaternion(btVector3(0,1,0),SIMD_HALF_PI));//设置变换的旋转角
				transformB.setRotation(btQuaternion(btVector3(0,1,0),SIMD_HALF_PI));//设置变换的旋转角
				m_bsc_far = new btSliderConstraint(*bodyA, *bodyB, transformA, transformB, true);//创建后方滑动关节
				m_dynamicsWorld->addConstraint(m_bsc_far);//将关节添加到物理世界中
				m_bsc_far->setLowerLinLimit(-5.5f);//设置关节移动的最小距离	关节的左侧方向
				m_bsc_far->setUpperLinLimit(5.5f);//设置关节移动的最大距离  关节的右侧方向
			}
			{
				//创建上方的立方体
				tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(2*UNIT_SIZE,2*UNIT_SIZE,2*UNIT_SIZE),//长方体的半区域    赋值应为   (k1*UNIT_SIZE,k2*UNIT_SIZE,,k3*UNIT_SIZE)
								6.0f,//长方体的质量
								btVector3(0,10,0),//刚体的位置
								0.5,//恢复系数
								0.5,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,//前侧纹理图id
								boxTexId,//后侧纹理图id
								boxTexId,//上侧纹理图id
								boxTexId,//下侧纹理图id
								boxTexId,//左侧纹理图id
								boxTexId//右侧纹理图id
								);
				tca.push_back(tbTemp);//将新立方体加入到列表中
				bodyB = tbTemp->getBody();//给bodyB赋值
				bodyB->setActivationState(ISLAND_SLEEPING);//将刚体的动作状态设为静态

				transformA.setIdentity();//初始化变换transformA
				transformB.setIdentity();//初始化变换transformB
				transformA.setOrigin(btVector3(0,10,0));//设置节点位置
				transformA.setRotation(btQuaternion(btVector3(0,0,1),SIMD_HALF_PI));//设置变换的旋转角
				transformB.setRotation(btQuaternion(btVector3(0,0,1),SIMD_HALF_PI));//设置变换的旋转角
				m_bsc_top = new btSliderConstraint(*bodyA, *bodyB, transformA, transformB, true);//创建上方滑动关节
				m_dynamicsWorld->addConstraint(m_bsc_top);//将关节添加到物理世界中
				m_bsc_top->setLowerLinLimit(-5.5f);//设置关节移动的最小距离	关节的左侧方向
				m_bsc_top->setUpperLinLimit(5.5f);//设置关节移动的最大距离  关节的右侧方向

				m_body_top = bodyB;//给全局变量下方刚体指针赋值
			}
			{
				//创建下方的立方体
				tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(2*UNIT_SIZE,2*UNIT_SIZE,2*UNIT_SIZE),//长方体的半区域    赋值应为   (k1*UNIT_SIZE,k2*UNIT_SIZE,,k3*UNIT_SIZE)
								6.0f,//长方体的质量
								btVector3(0,-10,0),//刚体的位置
								0.5,//恢复系数
								0.5,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,//前侧纹理图id
								boxTexId,//后侧纹理图id
								boxTexId,//上侧纹理图id
								boxTexId,//下侧纹理图id
								boxTexId,//左侧纹理图id
								boxTexId//右侧纹理图id
								);
				tca.push_back(tbTemp);//将新立方体加入到列表中
				bodyB = tbTemp->getBody();//给bodyB赋值
				bodyB->setActivationState(ISLAND_SLEEPING);//将刚体的动作状态设为静态

				transformA.setIdentity();//初始化变换transformA
				transformB.setIdentity();//初始化变换transformB
				transformA.setOrigin(btVector3(0,-10,0));//设置节点位置
				transformA.setRotation(btQuaternion(btVector3(0,0,1),SIMD_HALF_PI));//设置变换的旋转角
				transformB.setRotation(btQuaternion(btVector3(0,0,1),SIMD_HALF_PI));//设置变换的旋转角
				m_bsc_buttom = new btSliderConstraint(*bodyA, *bodyB, transformA, transformB, true);//创建下方滑动关节
				m_dynamicsWorld->addConstraint(m_bsc_buttom);//将关节添加到物理世界中
				m_bsc_buttom->setLowerLinLimit(-5.5f);//设置关节移动的最小距离	关节的左侧方向
				m_bsc_buttom->setUpperLinLimit(5.5f);//设置关节移动的最大距离  关节的右侧方向

				m_body_buttom = bodyB;//给全局变量下方刚体指针赋值
			}
		}
	}

	void setAddBodyId(int id)
	{
		addBodyId = id;
	}
	void addBody(int id)
	{
		if(id == 1)//当点击屏幕右侧时，物体向右移动
		{
			m_bsc_right->setPoweredLinMotor(true);//设置马达可用
			m_bsc_right->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_right->setTargetLinMotorVelocity(15.0f);//设置马达速度

			m_bsc_left->setPoweredLinMotor(true);//设置马达可用
			m_bsc_left->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_left->setTargetLinMotorVelocity(-15.0f);//设置马达速度

			m_body_top->setActivationState(DISABLE_DEACTIVATION);//将上方刚体的动作状态设为动态
			m_bsc_top->setPoweredLinMotor(true);//设置马达可用
			m_bsc_top->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_top->setTargetLinMotorVelocity(15.0f);//设置马达速度

			m_body_buttom->setActivationState(DISABLE_DEACTIVATION);//将下方刚体的动作状态设为动态
			m_bsc_buttom->setPoweredLinMotor(true);//设置马达可用
			m_bsc_buttom->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_buttom->setTargetLinMotorVelocity(-15.0f);//设置马达速度

			m_bsc_far->setPoweredLinMotor(true);//设置马达可用
			m_bsc_far->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_far->setTargetLinMotorVelocity(15.0f);//设置马达速度

			m_bsc_near->setPoweredLinMotor(true);//设置马达可用
			m_bsc_near->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_near->setTargetLinMotorVelocity(-15.0f);//设置马达速度
		}
		else if(id == 2)//当点击屏幕左侧时，物体向左移动
		{
			m_bsc_right->setPoweredLinMotor(true);//设置马达可用
			m_bsc_right->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_right->setTargetLinMotorVelocity(-15.0f);//设置马达速度

			m_bsc_left->setPoweredLinMotor(true);//设置马达可用
			m_bsc_left->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_left->setTargetLinMotorVelocity(15.0f);//设置马达速度

			m_body_top->setActivationState(DISABLE_DEACTIVATION);//将上方刚体的动作状态设为动态
			m_bsc_top->setPoweredLinMotor(true);//设置马达可用
			m_bsc_top->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_top->setTargetLinMotorVelocity(-15.0f);//设置马达速度

			m_body_buttom->setActivationState(DISABLE_DEACTIVATION);//将下方刚体的动作状态设为动态
			m_bsc_buttom->setPoweredLinMotor(true);//设置马达可用
			m_bsc_buttom->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_buttom->setTargetLinMotorVelocity(15.0f);//设置马达速度

			m_bsc_far->setPoweredLinMotor(true);//设置马达可用
			m_bsc_far->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_far->setTargetLinMotorVelocity(-15.0f);//设置马达速度

			m_bsc_near->setPoweredLinMotor(true);//设置马达可用
			m_bsc_near->setMaxLinMotorForce(10.0f);//设置马达最大力
			m_bsc_near->setTargetLinMotorVelocity(15.0f);//设置马达速度
		}
	}
	void cleanVector()
	{
		tca.clear();
	}

	bool onSurfaceChanged(int w, int h) {
	    glViewport(0, 0, w, h);
	    float ratio = (float) w/h;
	    MatrixState::setProjectFrustum(-ratio, ratio, -1, 1, 2, 100);
		return true;
	}


	bool onSurfaceCreated(JNIEnv * env,jobject obj) {

	    MatrixState::setCamera(0, 20, 20, 0, 0, -5, 0, 1, 0);
	    MatrixState::setLightLocation(0, 20, 20);//设置光源位置
	    MatrixState::setInitStack();
	    glClearColor(0, 0, 0, 0);
	    glEnable(GL_DEPTH_TEST);


		jclass cl = env->FindClass("com/bn/bullet/GL2JNIView");
		jmethodID id = env->GetStaticMethodID(cl,"initTextureRepeat","(Landroid/opengl/GLSurfaceView;Ljava/lang/String;)I");
		jstring name = env->NewStringUTF("box.jpg");
		boxTexId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("basketball.jpg");
		ballTexId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("grass.png");
		planeTexId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("wood_bin.jpg");//注意，文件名中不能出现数字！！！wood_bin1.jpg是错误的
		wood_binId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("floor.jpg");
		floorId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("muwen.jpg");
		muwenId = env->CallStaticIntMethod(cl,id,obj,name);

		initWorld();
		cleanVector();
		initCreateBodys();

		return true;
	}

	void renderFrame() {
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);//清除缓冲
        //设置camera位置
        MatrixState::setCamera
        (
        		cx,	//人眼位置的X
        		cy, //人眼位置的Y
        		cz, //人眼位置的Z
        		0, 	//人眼球看的点X
        		0,  //人眼球看的点Y
        		0,  //人眼球看的点Z
        		upx, 	//up向量
        		upy,
        		upz
        );
        MatrixState::setLightLocation(cx, cy, cz);//设置光源位置

	    for ( int i = 0; i < tca.size(); ++i )//遍历物体列表
	    {
	    	 tca[i]->drawSelf();//回调刚体的绘制方法
	    }

	    tcs0->drawSelfNoBody();

		MatrixState::pushMatrix();
		MatrixState::rotate(90,1,0,0);
	    tcs0->drawSelfNoBody();
	    MatrixState::popMatrix();

		MatrixState::pushMatrix();
		MatrixState::rotate(90,0,0,1);
	    tcs0->drawSelfNoBody();
	    MatrixState::popMatrix();

	    if(addBodyId!=0)
	    {
	    	addBody(addBodyId);//调用添加刚体的方法
	    	addBodyId = 0;//设置刚体id为0
	    }

	    m_dynamicsWorld->stepSimulation(1.0/60,5);//进行物理模拟计算
	}

	void loadObjData(int objId, float* vertices, int numsVer, float* normals, int numsNor){}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_setCamera
	  (JNIEnv *env, jclass jc,jfloat cx,jfloat cy,jfloat cz,jfloat upx,jfloat upy,jfloat upz)
	{
		setCamera(cx,cy,cz,upx,upy,upz);
	}

	float* copyFloats(float* src,int count)
	{
		float* dst=new float[count];
		for(int i=0;i<count;i++)
		{
			dst[i]=src[i];
		}
		return dst;
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_loadObjData
	  (JNIEnv *env, jclass jc, jint objId, jfloatArray vertices, jint numsVer, jfloatArray normals, jint numsNor)
	{
		jfloat*  jfVertexData= (jfloat*)(env->GetFloatArrayElements(vertices,0));
		jfloat*  jfNormalData= (jfloat*)(env->GetFloatArrayElements(normals,0));

		jsize vlen = env->GetArrayLength(vertices);
		jsize nlen = env->GetArrayLength(normals);


		loadObjData(
				(int)objId,
				copyFloats((float*)jfVertexData,(int)vlen),
				(int)numsVer,
				copyFloats((float*)jfNormalData,(int)nlen),
				(int)numsNor
				);


		env->ReleaseFloatArrayElements(vertices,jfVertexData,0);
		env->ReleaseFloatArrayElements(normals,jfNormalData,0);
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_nativeSetAssetManager
	  (JNIEnv *env, jclass cls, jobject assetManager)
	{
		AAssetManager* aamIn = AAssetManager_fromJava( env, assetManager );
	    FileUtil::setAAssetManager(aamIn);
	}


}

