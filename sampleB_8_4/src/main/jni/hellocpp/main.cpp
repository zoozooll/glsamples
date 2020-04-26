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

	TexBody* m_ball;//全局的球物体指针
	TexBody* m_ground;//全局的地面物体指针

	struct MyResultCallback : public btCollisionWorld::ContactResultCallback
	{
		virtual	btScalar addSingleResult(
				btManifoldPoint& cp,
				const btCollisionObjectWrapper* colObj0Wrap,
				int partId0,
				int index0,
				const btCollisionObjectWrapper* colObj1Wrap,
				int partId1,
				int index1
				)
		{
//			btVector3 ptA = cp.getPositionWorldOnA();
//			btVector3 ptB = cp.getPositionWorldOnB();
			const btCollisionObject* co0 = colObj0Wrap->getCollisionObject();//获取参与碰撞的物体
			const btCollisionObject* co1 = colObj1Wrap->getCollisionObject();//获取参与碰撞的另一个物体
			btRigidBody* rb0 = (btRigidBody*)co0;//获取参与碰撞的刚体
			btRigidBody* rb1 = (btRigidBody*)co1;//获取参与碰撞的另一个刚体
			if(rb0 == m_ball->getBody() && rb1 != m_ground->getBody())//判断一个刚体是否为球，另一个刚体是否不为地面
			{//若rb0为球 并且 rb1不为地面

//				LOGI("rb0");
//				LOGI("partId0 = %d",partId0);
//				LOGI("index0 = %d",index0);

				TexBody* temp;//临时物体指针
				std::vector<TexBody*>::iterator iter=tca.begin();//物体列表的迭代器
				for(;iter!=tca.end();)//遍历物体列表
				{
					if((*iter)->getBody() == rb1 )//满足删除条件//判断当前刚体是否为参与碰撞的另一个刚体
					{
						temp = (*iter);//记录当前刚体
						iter = tca.erase(iter);//从列表中删除当前物体
						break;
					}
					else//若不满足
					{
						iter++;//迭代器自加
					}
				}

				m_dynamicsWorld->removeCollisionObject(temp->getBody());//从物理世界中删除当前刚体
				delete temp;//删除临时物体
			}
//			LOGI("partId0 = %d",partId0);
//			LOGI("index0 = %d",index0);
//			LOGI("partId1 = %d",partId1);
//			LOGI("index1 = %d",index1);

			return 0;
		}
	};


	void initCreateBodys()
	{

		{
			btCollisionShape* planeShape=new btStaticPlaneShape(btVector3(0, 1, 0), 0);//创建地面形状
			//创建地面
			tbTemp = new TexPlane(UNIT_SIZE*100,planeTexId,planeShape,m_dynamicsWorld,
					0,-16,0,
					0.4,0.8);
			//将新立方体加入到列表中
			tca.push_back(tbTemp);
			m_ground = tbTemp;//给全局变量赋值
		}

		{
			#define ARRAY_SIZE_X 3//x方向上数组大小
			#define ARRAY_SIZE_Y 3//y方向上数组大小
			#define ARRAY_SIZE_Z 3//z方向上数组大小
			#define START_POS_X -3//x方向上起始坐标值
			#define START_POS_Y -3//y方向上起始坐标值
			#define START_POS_Z -3//z方向上起始坐标值
			float start_x = START_POS_X - ARRAY_SIZE_X/2;//计算起点x坐标
			float start_y = START_POS_Y;//计算起点y坐标
			float start_z = START_POS_Z - ARRAY_SIZE_Z/2;//计算起点z坐标

			for (int k=0;k<ARRAY_SIZE_Y;k++)
			{
				for (int i=0;i<ARRAY_SIZE_X;i++)
				{
					for(int j = 0;j<ARRAY_SIZE_Z;j++)
					{
						//起点坐标
						btVector3 pos(btScalar((UNIT_SIZE*4+2)*i + start_x),
								btScalar(0+(UNIT_SIZE*4+2)*k + start_y),
								btScalar((UNIT_SIZE*4+2)*j + start_z));

						//创建立方体刚体
						tbTemp = new TexCuboid(
								m_dynamicsWorld,//物理世界对象
								btVector3(UNIT_SIZE*2, UNIT_SIZE*2, UNIT_SIZE*2),//长方体的半区域
								10.0f,//长方体的质量
								pos,//刚体的位置
								0.2f,//恢复系数
								0.8f,//摩擦系数
								UNIT_SIZE,//单位长度
								boxTexId,boxTexId,boxTexId,
								boxTexId,boxTexId,boxTexId
						);
						//将新立方体加入到列表中
						tca.push_back(tbTemp);
						tbTemp->getBody()->setActivationState(DISABLE_DEACTIVATION);//设置木块为动态的状态
					}
				}
			}
		}

		{
			//创建球形形状
			m_ball=new TexBall(UNIT_SIZE*1.5f,ballTexId,m_dynamicsWorld,10,btVector3(2,30,2), 0.4, 0.8);
			tca.push_back(m_ball);//将新立方体加入到列表中
		}


	}

	void setAddBodyId(int id)
	{
		addBodyId = id;
	}
	void addBody(int id)
	{
		//创建球形形状
		tbTemp=new TexBall(UNIT_SIZE*1.5f,ballTexId,m_dynamicsWorld,10,btVector3(cx,cy-10,cz), 0.4, 0.8);

		//btVector3 vvec = btVector3(0,2,-10);
		btVector3 vvec = btVector3(-cx,-cy+10,-cz);//初始线速度
		btVector3 avec = btVector3(0,0,0);//初始角速度
		tbTemp->getBody()->setLinearVelocity(vvec);//设置球的线速度
		tbTemp->getBody()->setAngularVelocity(avec); //设置球的角速度
    	tca.push_back(tbTemp);//将新立方体加入到列表中
    	m_ball = tbTemp;//给全局的球指针赋值
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

	    MyResultCallback renderCallback;
	    m_dynamicsWorld->contactTest(m_ball->getBody(),renderCallback);//进行接触检测

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

