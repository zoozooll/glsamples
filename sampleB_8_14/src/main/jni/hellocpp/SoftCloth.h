/*
 * SoftCloth.h
 *
 *  Created on: 2015��1��31��
 *      Author: Nr
 */

#ifndef JNI_HELLOCPP_SOFTCLOTH_H_
#define JNI_HELLOCPP_SOFTCLOTH_H_

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

#include "../myEncapsulation/geometry/AnyShapeNoTexture.h"
#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/TexCuboid.h"

#include "../myEncapsulation/geometry/DrawRope.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

class SoftCloth : public TexBody{

    int m_numT;//�����θ���
    float *m_vertices;//��������
	btSoftBody *psb;//����ָ��
	btSoftRigidDynamicsWorld *m_dynamicsWorld;//���������������ָ��
	btSoftBodyWorldInfo m_softBodyWorldInfo;//��������������Ϣ
	AnyShapeNoTexture* m_asbv;//���ƶ���ָ��
	int m_texId;//����id
	btVector3 m_pos;//��������λ��

public:
	SoftCloth(btSoftRigidDynamicsWorld* m_dynamicsWorld,
			btSoftBodyWorldInfo& m_softBodyWorldInfo,
			btVector3 pos
			);//���캯��
//	virtual ~SoftCloth();

	void drawSelf();//�������ķ���
	btRigidBody* getBody();//���ظ���ָ��
	void initVertex();//��ʼ������λ��
	void initCloth();//��ʼ��������
};

#endif /* JNI_HELLOCPP_SOFTCLOTH_H_ */

