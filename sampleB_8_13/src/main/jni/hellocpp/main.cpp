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

#include "TexTriangleMesh.h"
#include "../Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace std;

//����  ����Ͷ��
extern "C" {

	int boxTexId;	//����������id
	int ballTexId;	//��������id
	int planeTexId;	//��������id
	int wood_binId;//��id
	int floorId;//���ڵ�������id
	int muWenId;
	int footballId;
	int jsqId;


	float UNIT_SIZE = 1.0;	//��λ����

	vector<TexBody*> tca;	//�洢�����װ������vector������ �����壬���壬���棩
	TexBody* tbTemp;		//��ʱ�����װ�����

	int addBodyId = 0;		//��Ӹ����id

	//�������λ������
	float cx = 0;
	float cy = 20;
	float cz = 20;

	//up����
	float upx = 0;
	float upy = 1;
	float upz = 0;

	btDefaultCollisionConfiguration* m_collisionConfiguration;//ϵͳĬ����ײ���������Ϣ
	btCollisionDispatcher*	m_dispatcher;	//��ײ����㷨������
	btBroadphaseInterface*	m_broadphase;	//��Ƚ׶���ײ���
	btConstraintSolver*		m_solver;		//��ײԼ�������
	btDynamicsWorld*		m_dynamicsWorld;//��������

	//���������λ��
	void setCamera(float x,float y,float z,float upxt,float upyt,float upzt)
	{
		cx = x;
		cy = y;
		cz = z;
		upx = upxt;
		upy = upyt;
		upz = upzt;
	}

	//��ʼ����������ķ���
	void initWorld()
	{
		m_collisionConfiguration = new btDefaultCollisionConfiguration();//����Ĭ�ϵ���ײ���������Ϣ����
		//������ײ����㷨�����������书��Ϊɨ�����е���ײ���ԣ���ȷ�����õļ����Զ�Ӧ���㷨
		m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

		m_broadphase = new btSimpleBroadphase();
		btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver();//�����ƶ�Լ�����������
		m_solver = sol;

		//������ɢ����������
		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
		m_dynamicsWorld->setGravity(btVector3(0,-10,0));//��������

	}

	#define YUANHUAN_OBJ 5
	#define CHAHU_OBJ 6

	float* vertices_chahu;
	int numsVer_chahu;
	int* indices_chahu;
	int numsInd_chahu;
	float* texs_chahu;
	int numsTex_chahu;


	float* vertices_yuanhuan;
	int numsVer_yuanhuan;
	int* indices_yuanhuan;
	int numsInd_yuanhuan;
	float* texs_yuanhuan;
	int numsTex_yuanhuan;

	void initCreateBodys()
	{

		{
			tbTemp = new TexPlane(
					m_dynamicsWorld,//�����������
					btVector3(100, 1, 100),//������İ�����
					0.0f,//�����������
					btVector3(0,-20,0),//�����λ��
					0.8f,//�ָ�ϵ��
					0.8f,//Ħ��ϵ��
					UNIT_SIZE,//��λ����
					planeTexId,planeTexId,planeTexId,
					planeTexId,planeTexId,planeTexId
			);
			//�����������뵽�б���
			tca.push_back(tbTemp);
		}
		{

			tbTemp = new TexTriangleMesh(
				10.0f,//����
				vertices_yuanhuan,
				numsVer_yuanhuan ,//������������ĳ���
				indices_yuanhuan ,
				numsInd_yuanhuan ,
				texs_yuanhuan ,
				numsTex_yuanhuan ,
				2.0f,//��������
				btVector3(0,40,-5),//λ��
				0.5f,//�ָ�ϵ��
				0.5f,//Ħ��ϵ��
				m_dynamicsWorld,//��������ָ��
				jsqId//����������id
				);
			tca.push_back(tbTemp);//��Բ��������뵽�б���
		}

		{
			tbTemp = new TexCuboid(
					m_dynamicsWorld,//�����������
					btVector3(UNIT_SIZE*2, UNIT_SIZE*2, UNIT_SIZE*2),//������İ�����
					10.0f,//�����������
					btVector3(0,30,-5),//�����λ��
					0.6f,//�ָ�ϵ��
					0.8f,//Ħ��ϵ��
					UNIT_SIZE,//��λ����
					boxTexId,boxTexId,boxTexId,
					boxTexId,boxTexId,boxTexId
			);
			tca.push_back(tbTemp);//������������뵽�б���

			tbTemp = new TexTriangleMesh(
				10.0f,//����
				vertices_chahu,
				numsVer_chahu,
				indices_chahu,
				numsInd_chahu,
				texs_chahu,
				numsTex_chahu,
				5.0f,//��������
				btVector3(0,5,-10),//λ��
				0.5f,//�ָ�ϵ��
				0.5f,//Ħ��ϵ��
				m_dynamicsWorld,//��������ָ��
				muWenId//����������id
				);
			tca.push_back(tbTemp);//�����������뵽�б���
		}

	}


	void loadObjDataWd(int objId, float* vertices, int numsVer, int* indices, int numsInd,
					float* texs, int numsTex)
	{
		if(objId == CHAHU_OBJ)
		{
			vertices_chahu = vertices;
			numsVer_chahu = numsVer;
			indices_chahu = indices;
			numsInd_chahu = numsInd;
			texs_chahu = texs;
			numsTex_chahu = numsTex;
		}
		else if(objId == YUANHUAN_OBJ)
		{
			vertices_yuanhuan = vertices;
			numsVer_yuanhuan = numsVer;
			indices_yuanhuan = indices;
			numsInd_yuanhuan = numsInd;
			texs_yuanhuan = texs;
			numsTex_yuanhuan = numsTex;
		}
	}

	void setAddBodyId(int id)
	{
		addBodyId = id;

	}
	void addBody(int id)
	{
		tbTemp = new TexCuboid(
				m_dynamicsWorld,//�����������
				btVector3(UNIT_SIZE*2, UNIT_SIZE*2, UNIT_SIZE*2),//������İ�����
				10.0f,//�����������
				btVector3(cx,cy-10,cz),//�����λ��
				0.2f,//�ָ�ϵ��
				0.8f,//Ħ��ϵ��
				UNIT_SIZE,//��λ����
				boxTexId,boxTexId,boxTexId,
				boxTexId,boxTexId,boxTexId
		);
    	//�������ӵĳ�ʼ�ٶ�
		//btVector3 vvec = btVector3(0,2,-10);
		btVector3 vvec = btVector3(-cx,-cy+10,-cz);
		btVector3 avec = btVector3(0,0,0);
		tbTemp->getBody()->setLinearVelocity(vvec);//����ֱ���˶����ٶ�--Vx,Vy,Vz��������
		tbTemp->getBody()->setAngularVelocity(avec); //����������ת���ٶ�--�����������x,y,x������ת���ٶ�
    	//������������뵽�б���
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

		initWorld();
		cleanVector();
		initCreateBodys();


		return true;
	}


	bool onSurfaceCreated(JNIEnv * env,jobject obj) {

	    MatrixState::setCamera(0, 20, 20, 0, 0, -5, 0, 1, 0);
	    MatrixState::setLightLocation(0, 20, 20);//���ù�Դλ��
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
		name = env->NewStringUTF("wood_bin.jpg");//ע�⣬�ļ����в��ܳ������֣�����wood_bin1.jpg�Ǵ����
		wood_binId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("floor.jpg");
		floorId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("muwen.jpg");
		muWenId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("football.jpg");
		footballId = env->CallStaticIntMethod(cl,id,obj,name);
		name = env->NewStringUTF("jiushengquan.jpg");
		jsqId = env->CallStaticIntMethod(cl,id,obj,name);

		return true;
	}

	void renderFrame() {
	    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);//�������
        //����cameraλ��
        MatrixState::setCamera
        (
        		cx,	//����λ�õ�X
        		cy, //����λ�õ�Y
        		cz, //����λ�õ�Z
        		0, 	//�����򿴵ĵ�X
        		0,  //�����򿴵ĵ�Y
        		0,  //�����򿴵ĵ�Z
        		upx, 	//up����
        		upy,
        		upz
        );
        MatrixState::setLightLocation(cx, cy, cz);//���ù�Դλ��


	    for ( int i = 0; i < tca.size(); ++i )//���������б�
	    {
	    	 tca[i]->drawSelf();//�ص�����Ļ��Ʒ���
	    }

	    if(addBodyId!=0)
	    {
	    	addBody(addBodyId);//������Ӹ���ķ���
	    	addBodyId = 0;//���ø���idΪ0
	    }

	    m_dynamicsWorld->stepSimulation(1.0/60,5);//��������ģ�����
	}

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
	int* copyInts(int* src,int count)
	{
		int* dst=new int[count];
		for(int i=0;i<count;i++)
		{
			dst[i]=src[i];
		}
		return dst;
	}

	void loadObjData(int objId, float* vertices, int numsVer, float* normals, int numsNor){}

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

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_loadObjDataWd
	  (JNIEnv *env, jclass jc, jint objId, jfloatArray vertices, jint numsVer, jintArray indices, jint numsInd,
			  jfloatArray tex, jint numsTex)
	{
		jfloat*  jfVertexData= (jfloat*)(env->GetFloatArrayElements(vertices,0));
		jint* jiIndexData = (jint*)(env->GetIntArrayElements(indices,0));
		jfloat*  jfTexData= (jfloat*)(env->GetFloatArrayElements(tex,0));

		loadObjDataWd(
				(int)objId,
				copyFloats((float*)jfVertexData,(int)numsVer),
				(int)numsVer,
				copyInts((int*)jiIndexData,(int)numsInd),
				(int)numsInd,
				copyFloats((float*)jfTexData,(int)numsTex),
				(int)numsTex
				);


		env->ReleaseFloatArrayElements(vertices,jfVertexData,0);
		env->ReleaseFloatArrayElements(tex,jfTexData,0);

		env->ReleaseIntArrayElements(indices,jiIndexData,0);
	}

	JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_nativeSetAssetManager
	  (JNIEnv *env, jclass cls, jobject assetManager)
	{
		AAssetManager* aamIn = AAssetManager_fromJava( env, assetManager );
	    FileUtil::setAAssetManager(aamIn);
	}


}

