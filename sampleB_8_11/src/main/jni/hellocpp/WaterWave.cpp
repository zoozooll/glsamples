#include "WaterWave.h"
#include "../myEncapsulation/MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

WaterWave::WaterWave(
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
	this->m_dynamicsWorld = dynamicsWorldIn;//��¼���������սָ��
	this->waveheight = waveheightIn;//��¼ˮ�Ʋ����ĸ߶�
	this->TRIANGLE_SIZE = triangleUnitSizeIn;//��¼�����ε�λ��С
	//��¼����
	this->NUM_VERTS_X = numsXIn;//x����ĵ����Ŀ	���ڵ���2
	//��¼����
	this->NUM_VERTS_Y = numsYIn;//y����ĵ����Ŀ	���ڵ���2
	this->texId = texIdIn;//��¼��������

	int totalVerts = NUM_VERTS_X*NUM_VERTS_Y;//�ܵĶ������������ظ�����
	int totalTriangles = 2*(NUM_VERTS_X-1)*(NUM_VERTS_Y-1);//����������
	gVertices = new btVector3[totalVerts];//�������飨���ظ����㣩
	gIndices = new int[totalTriangles*3];//�洢���������˳���� ����������*3 ����
	m_vertices = new float[totalVerts*3];//������������

	setVertexPositions(waveheightIn,0.f);//�Զ���������и�ֵ
	//�Զ����������鸳ֵ	ע�⣺��ʱ��
	int index=0;
	for (int i=0;i<NUM_VERTS_X-1;i++)//����ÿһ��
	{
		for (int j=0;j<NUM_VERTS_Y-1;j++)//����ÿһ��
		{
			gIndices[index++] = j*NUM_VERTS_X+i;//��һ�������εĵ�һ������ֵ
			gIndices[index++] = j*NUM_VERTS_X+i+1;//��һ�������εĵڶ�������ֵ
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;//��һ�������εĵ���������ֵ

			gIndices[index++] = j*NUM_VERTS_X+i;//�ڶ��������εĵ�һ������ֵ
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;//�ڶ��������εĵڶ�������ֵ
			gIndices[index++] = (j+1)*NUM_VERTS_X+i;//�ڶ��������εĵ���������ֵ
		}
	}

	//�������������������
	btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(
			totalTriangles,					//��������
			gIndices,						//��������
			3*sizeof(int),					//��������   3*4��3���� * int������ռ���ֽ���4��
			totalVerts,						//���еĶ�����
			(btScalar*) &gVertices[0].x(),	//����������׵�ַ
			sizeof(btVector3));					//���㲽��    4*4��һ��������� 4������ * һ��������ռ���ֽ���4��

	bool useQuantizedAabbCompression = true;
	btVector3 aabbMin(-1000,-1000,-1000),aabbMax(1000,1000,1000);//��Χ��
	//�������������״����
	trimeshShape  = new btBvhTriangleMeshShape(m_indexVertexArrays,useQuantizedAabbCompression,aabbMin,aabbMax);
	float mass = 0.f;//��������,���������Ҫ������ʾ��״�Ƚϸ��ӵĵ��Ρ�ɽ�ڵȡ���Ҫע����ǣ�����������������ӦΪ�㡣
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

	//������ײ����־
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);//STATIC_OBJECT);
	//enable custom material callback
	//������ײ����־
	body->setCollisionFlags(body->getCollisionFlags()  | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	body->setActivationState(DISABLE_DEACTIVATION);//���ø���Ϊ��̬��

	//�������ƶ���
	asbi = new AnyShapeByVectices(
			m_vertices,
			totalVerts*3,//�������������ظ�ֵ��
			gIndices,//��������	(unsigned int*)gIndices
			totalTriangles*3,//��������
			-1.0f
			);
}

btScalar WaterWave::getDeltaTimeMicroseconds()
{
	btScalar dt = (btScalar)m_clock.getTimeMicroseconds();//��ȡʱ����
	m_clock.reset();//����ʱ��
	return dt;//����ʱ����
}

void WaterWave::drawSelf()
{
	float dt = getDeltaTimeMicroseconds() * 0.000001f;//λ������

	static float offset=0.f;
	offset+=dt;//λ��

	btVector3 aabbMin(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
	btVector3 aabbMax(-BT_LARGE_FLOAT,-BT_LARGE_FLOAT,-BT_LARGE_FLOAT);

	for (int i=0;i<NUM_VERTS_X;i++)//����ÿһ��
	{
		for (int j=0;j<NUM_VERTS_Y;j++)//����ÿһ��
		{

			aabbMax.setMax(gVertices[i+j*NUM_VERTS_X]);
			aabbMin.setMin(gVertices[i+j*NUM_VERTS_X]);

			gVertices[i+j*NUM_VERTS_X].setValue((i-NUM_VERTS_X*0.5f)*TRIANGLE_SIZE,
				//0.f,
				waveheight*sinf((float)i+offset)*cosf((float)j+offset),
				(j-NUM_VERTS_Y*0.5f)*TRIANGLE_SIZE);

			aabbMin.setMin(gVertices[i+j*NUM_VERTS_X]);
			aabbMax.setMax(gVertices[i+j*NUM_VERTS_X]);

		}
	}
	trimeshShape->partialRefitTree(aabbMin,aabbMax);//����״�ֲ��Ż�


	for(int i = 0; i<NUM_VERTS_X*NUM_VERTS_Y; i++)//�������еĶ���
	{
		m_vertices[i*3+0] = gVertices[i].x();//�����x����
		m_vertices[i*3+1] = gVertices[i].y();//�����y����
		m_vertices[i*3+2] = gVertices[i].z();//�����z����
	}

	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	MatrixState::pushMatrix();//�����ֳ�
	asbi->drawSelf(texId);//����ˮ��
	MatrixState::popMatrix();//�ָ��ֳ�


	MatrixState::popMatrix();//�ָ��ֳ�
}

//���ö����λ��
void WaterWave::setVertexPositions(float waveheight, float offset)//offsetλ��
{
	for (int i=0;i<NUM_VERTS_X;i++)//����ÿһ��
	{
		for (int j=0;j<NUM_VERTS_Y;j++)//����ÿһ��
		{
			//��ÿ���㸳��ά����
			gVertices[i+j*NUM_VERTS_X].setValue(
					(i-NUM_VERTS_X*0.5f)*TRIANGLE_SIZE,
					waveheight*sinf((float)i+offset)*cosf((float)j+offset),
					(j-NUM_VERTS_Y*0.5f)*TRIANGLE_SIZE
				);
		}
	}
	for(int i = 0; i<NUM_VERTS_X*NUM_VERTS_Y; i++)//�������еĶ���
	{
		m_vertices[i*3+0] = gVertices[i].x();//�����x����
		m_vertices[i*3+1] = gVertices[i].y();//�����y����
		m_vertices[i*3+2] = gVertices[i].z();//�����z����
	}
}

btRigidBody* WaterWave::getBody()
{
	return body;
}
