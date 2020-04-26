#ifndef _Landform_H_
#define _Landform_H_

#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/geometry/AnyShapeByVectices.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/btBulletCollisionCommon.h"

#include "../Bullet/LinearMath/btQuickprof.h"


class Landform : public TexBody
{

	AnyShapeByVectices* asbi;//����ָ��

	btDynamicsWorld *m_dynamicsWorld;//��������ָ��
	btBvhTriangleMeshShape* trimeshShape;//��״ָ��   trimesh��������������
	btRigidBody* body;

	float waveheight;//�����߶�
	float TRIANGLE_SIZE;//�����α߳��ĵ�λ��С
	int texId;//����id

	int NUM_VERTS_X;//x�����ϵ����Ŀ����x-1��	 ȡֵ���ڵ���2
	int NUM_VERTS_Y;//y�����ϵ����Ŀ����y-1��     ȡֵ���ڵ���2

	btVector3*	gVertices;		//��������ָ��
	int* gIndices;		//��������ָ��

	float* tempVer;

public:
	Landform(
			btDynamicsWorld *dynamicsWorldIn,//��������ָ��
			float waveheightIn,//��������ĸ߶�
			float triangleUnitSizeIn, //ÿһ�������εı߳��ĵ�λ��С
			float numsXIn,//x�����ϵ����Ŀ
			float numsYIn,//y�����ϵ����Ŀ
			int texIdIn,//����id
			float restitutionIn,//�ָ�ϵ��
			float frictionIn,//Ħ����
			btVector3 pos//λ��
			);
	void drawSelf();
	btRigidBody* getBody();
	void setVertexPositions(float waveheight, float offset);//����ĸ߶ȣ�λ��

};

#endif


//{
//	tbTemp = new Landform(
//			m_dynamicsWorld,//��������ָ��
//			5.0f,//��������ĸ߶�
//			8.0f, //ÿһ�������εı߳��ĵ�λ��С
//			30,//x�����ϵ����Ŀ
//			30,//y�����ϵ����Ŀ
//			planeTexId,//����id
//			0.1f,//�ָ�ϵ��
//			0.5f,//Ħ����
//			btVector3(0.0f, -16.0f, 0.0f)//λ��
//			);
//	//������������뵽�б���
//	tca.push_back(tbTemp);
//}
