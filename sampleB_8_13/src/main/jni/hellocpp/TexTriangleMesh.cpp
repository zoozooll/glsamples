#include "TexTriangleMesh.h"
#include "../myEncapsulation/ShaderUtil.h"
#include "../Bullet/BulletCollision/Gimpact/btGImpactShape.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/BulletCollision/CollisionShapes/btShapeHull.h"

#include "../Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"


#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

TexTriangleMesh::TexTriangleMesh(
	btScalar mass,//����

	float* vertices,//������������    ���ظ�ֵ
	int numsVer,//������������ĳ���
	int* indices,
	int numsInd,
	float* texs,
	int numsTex,


	float scale,//��������

	btVector3 pos,//λ��
	btScalar restitutionIn,//�ָ�ϵ��
	btScalar frictionIn,//Ħ��ϵ��
	btDynamicsWorld *dynamicsWorld,//��������ָ��
	int triangleTexId//����������id
	)
{
	for(int i = 0; i<numsVer; i++)//������������
	{
		vertices[i] = vertices[i]*scale;//���Ķ�������
	}

	this->triangleTexId = triangleTexId;//��¼����id

	int totalTriangles = numsInd / 3;//�������������� (һ�����������������xyz,һ����������������������ɵ�)
	int numsVertices = numsInd;//��¼����������  ���ظ�ֵ��

	int vertStride = 3*sizeof(float);//�������鲽��//һ��������ռ���ֽ���  4*4 ��ʾ:xyzw�ĸ�����ֵ * ÿһ�����������ֽ���
	int indexStride = 3*sizeof(int);//��������  //����int������ռ�ֽ���  3*4   ��ʾ��gIndices������int���͵�����

	//�����������������������
	btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(
			totalTriangles,					//��������
			indices,						//��������
			indexStride,					//��������   3*4��3���� * int������ռ���ֽ���4��
			numsVertices,						//���еĶ�����
			(btScalar*) &vertices[0],	//����������׵�ַ
			vertStride);					//���㲽��    4*4��һ��������� 4������ * һ��������ռ���ֽ���4��


	//�����̶���������״�����
	btGImpactMeshShape * gims = new btGImpactMeshShape(m_indexVertexArrays);
	gims->updateBound();//���·�Χ
	btCollisionShape* finalShape = gims;

	btCollisionDispatcher * dispatcher = static_cast<btCollisionDispatcher *>(dynamicsWorld ->getDispatcher());//��ȡ������
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);//ע���㷨

	//�������ƶ���
	ctm = new AnyShapeByVectices(
			vertices,//������������    ���ظ�ֵ
			numsVer,//������������ĳ���
			indices,
			numsInd,
			texs,
			numsTex,
			false,
			1.0f
			);//����������


	bool isDynamic = (mass != 0);//�����Ƿ�����˶�
	btVector3 localInertia = btVector3(0, 0, 0);//��������
	if(isDynamic)
	{
		finalShape->calculateLocalInertia(mass, localInertia);//�����������
	}
	btTransform transform = btTransform();//��������ĳ�ʼ�任����
	transform.setIdentity();//�任�����ʼ��
	transform.getOrigin().setValue(pos.x(),pos.y(),pos.z());//���ñ任�����λ��
	//���������˶�״̬����
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	//����������Ϣ����
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, finalShape, localInertia);
	rbInfo.m_restitution = restitutionIn;//���÷���ϵ��
	rbInfo.m_friction = frictionIn;//����Ħ��ϵ��
	body = new btRigidBody(rbInfo);//��������
	dynamicsWorld->addRigidBody(body);//��������ӽ���������

}

void TexTriangleMesh::drawSelf()
{
	MatrixState::pushMatrix();//�����ֳ�

	btTransform trans;
	trans = body->getWorldTransform();				//��ȡ����ı任��Ϣ����
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//����ǰ�ľ������ø��任��Ϣ����

	MatrixState::pushMatrix();//�����ֳ�
	ctm->drawSelf(triangleTexId);//���ƹ̶���״�����
	MatrixState::popMatrix();//�ָ��ֳ�


	MatrixState::popMatrix();//�ָ��ֳ�
}

btRigidBody* TexTriangleMesh::getBody()
{
	return body;
}
