#ifndef _TexTriangleMesh_H_
#define _TexTriangleMesh_H_

#include "../myEncapsulation/TexBall.h"
#include "../myEncapsulation/MatrixState.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "../Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "../Bullet/LinearMath/btVector3.h"

#include "../myEncapsulation/geometry/AnyShapeByVectices.h"

class TexTriangleMesh : public TexBody
{
	btDynamicsWorld* m_dynamicsWorld;//��������ָ��
	btRigidBody* body;//�������ָ��
	AnyShapeByVectices* ctm;//���ƶ���
	int triangleTexId;//����id
public :
	TexTriangleMesh(
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
		);//���캯��
//	~TexTriangleMesh();
	void drawSelf();//��������ķ���
	btRigidBody* getBody();//��ȡ����ָ�뷽��
};

#endif
