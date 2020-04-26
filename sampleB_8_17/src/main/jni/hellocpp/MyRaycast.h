#ifndef _MyRaycast_H_
#define _MyRaycast_H_

#include "../Bullet/LinearMath/btVector3.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../myEncapsulation/geometry/DrawLine.h"

class MyRaycast
{
	DrawLine* dl;
	btCollisionWorld* m_cw;//��������ָ��

	btVector3* m_vector;//��������   ����˳����Ҫ�󣺵�һ��͵ڶ�����һ�飬����һ���ߡ�������͵��ĵ���һ�飬����һ����......
	int nums_m_vector;//�������鳤��		����Ϊż��

	btVector3* hit;//��ײ�Ӵ���		����Ӧ��ΪvCountLine
	btVector3* normals;//������	����Ӧ��ΪvCountLine
	btVector3* reflectedRays;

	int vCountLine;//�ߵ�����
public :
	MyRaycast(
			btVector3* m_vector, //��������   ����˳����Ҫ�󣺵�һ��͵ڶ�����һ�飬����һ���ߡ�������͵��ĵ���һ�飬����һ����
			int nums_m_vector, //�������鳤��		����Ϊż��
			btCollisionWorld* m_cw//��������ָ��
			);
//	~MyRaycast();

	void cast();//Ͷ�䷽��

	btVector3 reflectedRay(btVector3 n1,btVector3 n2);//n1������   n2������

	void drawSelf();//�˷���Ӧ��cast����֮�����

};

#endif
