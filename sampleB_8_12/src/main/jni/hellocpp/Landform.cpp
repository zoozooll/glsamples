#include "Landform.h"
#include "../myEncapsulation/MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

Landform::Landform(
			btDynamicsWorld *dynamicsWorldIn,//��������ָ��
			float waveheightIn,//��������ĸ߶�
			float triangleUnitSizeIn, //ÿһ�������εı߳��ĵ�λ��С
			float numsXIn,//x�����ϵ����Ŀ
			float numsYIn,//y�����ϵ����Ŀ
			int texIdIn,//����id
			float restitutionIn,//�ָ�ϵ��
			float frictionIn,//Ħ����
			btVector3 pos//λ��
			)
{
	this->m_dynamicsWorld = dynamicsWorldIn;
	this->waveheight = waveheightIn;
	this->TRIANGLE_SIZE = triangleUnitSizeIn;
	this->NUM_VERTS_X = numsXIn;//x����ĵ����Ŀ	���ڵ���2
	this->NUM_VERTS_Y = numsYIn;//y����ĵ����Ŀ	���ڵ���2
	this->texId = texIdIn;

	int totalVerts = NUM_VERTS_X*NUM_VERTS_Y;//�ܵĶ������������ظ�����
	int totalTriangles = 2*(NUM_VERTS_X-1)*(NUM_VERTS_Y-1);//����������
	gVertices = new btVector3[totalVerts];//�������飨���ظ����㣩
	gIndices = new int[totalTriangles*3];//�洢���������˳���� ����������*3 ����

	setVertexPositions(waveheightIn,0.f);//�Զ���������и�ֵ
	//�Զ����������鸳ֵ	ע�⣺��ʱ��
	int index=0;
	for (int i=0;i<NUM_VERTS_X-1;i++)
	{
		for (int j=0;j<NUM_VERTS_Y-1;j++)
		{
			gIndices[index++] = j*NUM_VERTS_X+i;
			gIndices[index++] = j*NUM_VERTS_X+i+1;
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;

			gIndices[index++] = j*NUM_VERTS_X+i;
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;
			gIndices[index++] = (j+1)*NUM_VERTS_X+i;
		}
	}

	btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(
			totalTriangles,					//��������
			gIndices,						//��������
			3*sizeof(int),					//��������   3*4��3���� * int������ռ���ֽ���4��
			totalVerts,						//���еĶ�����
			(btScalar*) &gVertices[0].x(),	//����������׵�ַ
			sizeof(btVector3));					//���㲽��    4*4��һ��������� 4������ * һ��������ռ���ֽ���4��

	bool useQuantizedAabbCompression = true;
	btVector3 aabbMin(-1000,-1000,-1000),aabbMax(1000,1000,1000);
	//�������������״����
	//btBvhTriangleMeshShape*
	trimeshShape  = new btBvhTriangleMeshShape(m_indexVertexArrays,useQuantizedAabbCompression,aabbMin,aabbMax);
	float mass = 0.f;//��������
	btTransform	startTransform;//�任����
	startTransform.setIdentity();//�任�����ʼ��
	startTransform.setOrigin(btVector3(pos.x(),pos.y(),pos.z()));//����ԭ��λ��
	btVector3 localInertia(0,0,0);//��������
	//����������˶�״̬����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	//����������Ϣ����
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,(btCollisionShape*)trimeshShape,localInertia);
	rbInfo.m_restitution = restitutionIn;//���÷���ϵ��
	rbInfo.m_friction = frictionIn;//����Ħ��ϵ��
	body = new btRigidBody(rbInfo);//��������
	m_dynamicsWorld->addRigidBody(body);//��������ӽ���������

	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);//STATIC_OBJECT);
	//enable custom material callback
	body->setCollisionFlags(body->getCollisionFlags()  | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	body->setActivationState(DISABLE_DEACTIVATION);

	tempVer = new float[totalVerts*3];//float*
	for(int i = 0; i<totalVerts; i++)
	{
		tempVer[i*3+0] = gVertices[i].x();
		tempVer[i*3+1] = gVertices[i].y();
		tempVer[i*3+2] = gVertices[i].z();
	}
	asbi = new AnyShapeByVectices(
			tempVer,
			totalVerts*3,//�������������ظ�ֵ��
			gIndices,//��������	(unsigned int*)gIndices
			totalTriangles*3,//��������
			-1.0f
			);
}

void Landform::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;									//��ȡ������ӵı任��Ϣ����
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	//�����
	MatrixState::pushMatrix();//�����ֳ�
	for(int i = 0; i<NUM_VERTS_X*NUM_VERTS_Y; i++)
	{
		tempVer[i*3+0] = gVertices[i].x();
		tempVer[i*3+1] = gVertices[i].y();
		tempVer[i*3+2] = gVertices[i].z();
	}
	asbi->drawSelf(texId);
	MatrixState::popMatrix();//�ָ��ֳ�


	MatrixState::popMatrix();//�ָ��ֳ�
}

//���ö����λ��
void Landform::setVertexPositions(float waveheight, float offset)//offsetλ��
{
	for (int i=0;i<NUM_VERTS_X;i++)//����ÿһ��
	{
		for (int j=0;j<NUM_VERTS_Y;j++)//����ÿһ��
		{
			//��ÿ���㸳��ά����
			gVertices[i+j*NUM_VERTS_X].setValue(
					(i-NUM_VERTS_X*0.5f)*TRIANGLE_SIZE,
//					0.f,
					waveheight*sinf((float)i+offset)*cosf((float)j+offset),
					(j-NUM_VERTS_Y*0.5f)*TRIANGLE_SIZE
				);
		}
	}
}

btRigidBody* Landform::getBody()
{
	return body;
}
