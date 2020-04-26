/*
 * SoftRope.h
 *
 *  Created on: 2015��1��31��
 *      Author: Nr
 */

#ifndef _SOFTCROPE_H_
#define _SOFTCROPE_H_

#include "Bullet/LinearMath/btAlignedObjectArray.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/LinearMath/btVector3.h"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "Bullet/BulletSoftBody/btSoftBody.h"
#include "Bullet/BulletSoftBody/btSoftBodyHelpers.h"
#include "Bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "Bullet/BulletSoftBody/btSoftBodySolvers.h"
#include "Bullet/BulletSoftBody/btDefaultSoftBodySolver.h"
#include "Bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/TexCuboid.h"
#include "../myEncapsulation/TexBall.h"

#include "../myEncapsulation/geometry/DrawRope.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

class SoftRope : public TexBody{


	btSoftBody *psb;//����ָ��
	btSoftRigidDynamicsWorld *m_dynamicsWorld;//������������ָ��
	btSoftBodyWorldInfo m_softBodyWorldInfo;//��������������Ϣ
	int m_ropeId;
	int m_ballId;
	int m_boxId;

	DrawRope* dr;//����������ָ��

	TexBody* tbBall;//С����ָ��
	TexBody* tbCuboid;//ľ����ָ��
	btVector3* m_vec_rope;//������������
	btVector3 m_pos;//С��λ������

public:
	SoftRope(btSoftRigidDynamicsWorld* m_dynamicsWorld,
			btSoftBodyWorldInfo& m_softBodyWorldInfo,
			btVector3 m_pos,
			int ropeId,//����id
			int ballId,
			int boxId
			);//���캯��
//	virtual ~SoftRope();

	void drawSelf();//����������С���ľ��ķ���
	btRigidBody* getBody();//��ȡ����ָ�뷽��
	void initVertex();//��ʼ���������ݷ���
	void init();//����������С���ľ��ķ���
};

#endif /* JNI_HELLOCPP_SOFTROPE_H_ */


//		{
//			sd = new SoftRope(m_dynamicsWorld,
//					m_softBodyWorldInfo,
//					boxTexId//����id
//					);
//			tca.push_back(sd);
//		}
