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
	float cx = -20;
	float cy = 20;
	float cz = 20;

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


	void initCreateBodys()
	{
			btCollisionShape* planeShape=new btStaticPlaneShape(btVector3(0, 1, 0), 0);//创建地面形状
			//创建地面
			tbTemp = new TexPlane(UNIT_SIZE*100,planeTexId,planeShape,m_dynamicsWorld,
					0,-6,0,
					0.4,0.8);
			//将地面加入到列表中
			tca.push_back(tbTemp);

			btRigidBody* bodyA=0;//指向第一个刚体的指针
			btRigidBody* bodyB=0;//指向第二个刚体的指针
			float gearRadio = 3.0f;//齿轮半径
			float gearHeight = 0.2f;//齿轮高度
			//创建圆盘对象
			tbTemp = new TexCylinderShape(
					muwenId,//底部圆形的纹理id
					muwenId,//顶部圆形的纹理id
					boxTexId,//圆柱侧面的纹理id
					30,//切分份数
					btVector3(gearRadio,gearHeight,gearRadio),//半区域
					m_dynamicsWorld,//物理世界指针
					6.0,//质量
					btVector3(-gearRadio,0,0),//位置
					0.0f,//恢复系数
					0.8f);//摩擦系数
			//将圆盘加入到列表中
			tca.push_back(tbTemp);

			//创建铰链关节对象
			btHingeConstraint* tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0));
			m_dynamicsWorld->addConstraint(tempConstraint,true);//将铰链约束添加到物理世界中

			bodyA = tbTemp->getBody();//获取指向第一个刚体的指针

			//创建圆盘对象
			tbTemp = new TexCylinderShape(
					muwenId,//底部圆形的纹理id
					muwenId,//顶部圆形的纹理id
					boxTexId,//圆柱侧面的纹理id
					30,//切分份数
					btVector3(gearRadio,gearHeight,gearRadio),//半区域
					m_dynamicsWorld,//物理世界指针
					6.0,//质量
					btVector3(gearHeight,gearRadio,0),//位置
					0.0f,//恢复系数
					0.8f);//摩擦系数
			//将圆盘加入到列表中
			tca.push_back(tbTemp);

			btTransform tempT = tbTemp->getBody()->getWorldTransform();//获取圆盘对象的变化对象
			tempT.setRotation(btQuaternion(btVector3(0,0,1),SIMD_PI/2.f));//更改变换
			tbTemp->getBody()->setWorldTransform(tempT);//给圆盘设置变换对象

			//第三个参数：轴坐标，它是按照创建刚体时的初始变换为坐标原点的，不是按照经过上面三行代码（后天更改过变换对象）的坐标系的
			//创建铰链关节对象
			tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0));
			m_dynamicsWorld->addConstraint(tempConstraint,true);//将铰链约束添加到物理世界中

			tbTemp->getBody()->setAngularVelocity(btVector3(10,0,0));//给圆盘设置角速度

			bodyB = tbTemp->getBody();//获取指向第二个刚体的指针

			btVector3	axisA(0,1,0);
			btVector3	axisB(0,1,0);
			btGearConstraint* gear = new btGearConstraint(*bodyA,*bodyB, axisA,axisB,1);
			m_dynamicsWorld->addConstraint(gear,true);

			tbTemp = new TexCylinderShape(
					muwenId,//底部圆形的纹理id
					muwenId,//顶部圆形的纹理id
					boxTexId,//圆柱侧面的纹理id
					30,//切分份数
					btVector3(gearRadio,gearHeight,gearRadio),//半区域
					m_dynamicsWorld,//物理世界指针
					6.0,//质量
					btVector3(-gearRadio,0,-10),//位置
					0.0f,//恢复系数
					0.8f);//摩擦系数
			//将圆盘加入到列表中
			tca.push_back(tbTemp);

			//创建铰链关节对象
			tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0));
			m_dynamicsWorld->addConstraint(tempConstraint,true);//将铰链约束添加到物理世界中

			//创建圆盘对象
			tbTemp = new TexCylinderShape(
					muwenId,//底部圆形的纹理id
					muwenId,//顶部圆形的纹理id
					boxTexId,//圆柱侧面的纹理id
					30,//切分份数
					btVector3(gearRadio,gearHeight,gearRadio),//半区域
					m_dynamicsWorld,//物理世界指针
					6.0,//质量
					btVector3(gearHeight,gearRadio,-10),//位置
					0.0f,//恢复系数
					0.8f);//摩擦系数
			//将新立方体加入到列表中
			tca.push_back(tbTemp);

			tempT = tbTemp->getBody()->getWorldTransform();//获取圆盘对象的变化对象
			tempT.setRotation(btQuaternion(btVector3(0,0,1),SIMD_PI/2.f));//更改变换
			tbTemp->getBody()->setWorldTransform(tempT);//给圆盘设置变换对象

			//第三个参数：轴坐标，它是按照创建刚体时的初始变换为坐标原点的，不是按照经过上面三行代码（后天更改过变换对象）的坐标系的
			//创建铰链关节对象
			tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0));
			m_dynamicsWorld->addConstraint(tempConstraint,true);//将铰链约束添加到物理世界中

			//给圆盘设置角速度
			tbTemp->getBody()->setAngularVelocity(btVector3(10,0,0));//在yoz平面上进行旋转


//		{
//			btRigidBody* bodyA=0;
//			btRigidBody* bodyB=0;
//			float gearRadio = 3.0f;
//			float gearHeight = 0.2f;
//			{
//				//水平圆盘
//				tbTemp = new TexCylinderShape(
//						ballTexId,//底部圆形的纹理id
//						ballTexId,//顶部圆形的纹理id
//						boxTexId,//圆柱侧面的纹理id
//						30,//切分份数
//						btVector3(gearRadio,gearHeight,gearRadio),//半区域
//						m_dynamicsWorld,//物理世界指针
//						6.0,//质量
//						btVector3(-gearRadio,0,0),//位置
//						0.0f,//恢复系数
//						0.8f);//摩擦系数
//				//将新立方体加入到列表中
//				tca.push_back(tbTemp);
//
//				btHingeConstraint* tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0),false);
//				m_dynamicsWorld->addConstraint(tempConstraint,true);
//
//				bodyA = tbTemp->getBody();
//				//bodyA->setAngularVelocity(btVector3(0,5,0));
//			}
//			{
//				tbTemp = new TexCylinderShape(
//						ballTexId,//底部圆形的纹理id
//						ballTexId,//顶部圆形的纹理id
//						boxTexId,//圆柱侧面的纹理id
//						30,//切分份数
//						btVector3(gearRadio,gearHeight,gearRadio),//半区域
//						m_dynamicsWorld,//物理世界指针
//						6.0,//质量
//						btVector3(gearHeight,gearRadio,0),//位置
//						0.0f,//恢复系数
//						0.8f);//摩擦系数
//				//将新立方体加入到列表中
//				tca.push_back(tbTemp);
//
//				btTransform tempT = tbTemp->getBody()->getWorldTransform();
//				tempT.setRotation(btQuaternion(btVector3(0,0,1),SIMD_PI/2.f));//更改变换
//				tbTemp->getBody()->setWorldTransform(tempT);//设置变换对象
//
//				//第三个参数：轴坐标，它是按照创建刚体时的初始变换为坐标原点的，不是按照经过上面三行代码（后天更改过变换对象）的坐标系的
//				btHingeConstraint* tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0),false);
//				m_dynamicsWorld->addConstraint(tempConstraint,true);
//
//				tbTemp->getBody()->setAngularVelocity(btVector3(10,0,0));//在yoz平面上进行旋转
//
//				bodyB = tbTemp->getBody();
//			}
//
//			btVector3	axisA(0,1,0);
//			btVector3	axisB(0,1,0);
//			btGearConstraint* gear = new btGearConstraint(*bodyA,*bodyB, axisA,axisB,1);
//			m_dynamicsWorld->addConstraint(gear,true);
//
//			{
//				tbTemp = new TexCylinderShape(
//						ballTexId,//底部圆形的纹理id
//						ballTexId,//顶部圆形的纹理id
//						boxTexId,//圆柱侧面的纹理id
//						30,//切分份数
//						btVector3(gearRadio,gearHeight,gearRadio),//半区域
//						m_dynamicsWorld,//物理世界指针
//						6.0,//质量
//						btVector3(-gearRadio,0,-10),//位置
//						0.0f,//恢复系数
//						0.8f);//摩擦系数
//				//将新立方体加入到列表中
//				tca.push_back(tbTemp);
//
//				btHingeConstraint* tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0),false);
//				m_dynamicsWorld->addConstraint(tempConstraint,true);
//
//			}
//			{
//				tbTemp = new TexCylinderShape(
//						ballTexId,//底部圆形的纹理id
//						ballTexId,//顶部圆形的纹理id
//						boxTexId,//圆柱侧面的纹理id
//						30,//切分份数
//						btVector3(gearRadio,gearHeight,gearRadio),//半区域
//						m_dynamicsWorld,//物理世界指针
//						6.0,//质量
//						btVector3(gearHeight,gearRadio,-10),//位置
//						0.0f,//恢复系数
//						0.8f);//摩擦系数
//				//将新立方体加入到列表中
//				tca.push_back(tbTemp);
//
//				btTransform tempT = tbTemp->getBody()->getWorldTransform();
//				tempT.setRotation(btQuaternion(btVector3(0,0,1),SIMD_PI/2.f));//更改变换
//				tbTemp->getBody()->setWorldTransform(tempT);//设置变换对象
//
//				//第三个参数：轴坐标，它是按照创建刚体时的初始变换为坐标原点的，不是按照经过上面三行代码（后天更改过变换对象）的坐标系的
//				btHingeConstraint* tempConstraint = new btHingeConstraint(*(tbTemp->getBody()),btVector3(0,0,0),btVector3(0,1,0),false);
//				m_dynamicsWorld->addConstraint(tempConstraint,true);
//
//				tbTemp->getBody()->setAngularVelocity(btVector3(10,0,0));//在yoz平面上进行旋转
//			}
//
//		}

	}

	void setAddBodyId(int id)
	{
		addBodyId = id;
	}
	void addBody(int id)
	{
		tbTemp = new TexCuboid(
				m_dynamicsWorld,//物理世界对象
				btVector3(UNIT_SIZE*2, UNIT_SIZE*2, UNIT_SIZE*2),//长方体的半区域
				10.0f,//长方体的质量
				btVector3(cx,cy-10,cz),//刚体的位置
				0.2f,//恢复系数
				0.8f,//摩擦系数
				UNIT_SIZE,//单位长度
				boxTexId,boxTexId,boxTexId,
				boxTexId,boxTexId,boxTexId
		);
    	//设置箱子的初始速度
		//btVector3 vvec = btVector3(0,2,-10);
		btVector3 vvec = btVector3(-cx,-cy+10,-cz);
		btVector3 avec = btVector3(0,0,0);
		tbTemp->getBody()->setLinearVelocity(vvec);//箱子直线运动的速度--Vx,Vy,Vz三个分量
		tbTemp->getBody()->setAngularVelocity(avec); //箱子自身旋转的速度--绕箱子自身的x,y,x三轴旋转的速度
    	//将新立方体加入到列表中
    	tca.push_back(tbTemp);
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

