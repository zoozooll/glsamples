/*
 * SoftObj.h
 *
 *  Created on: 2015��1��31��
 *      Author: Nr
 */

#ifndef _SoftObj_H_
#define _SoftObj_H_

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

class SoftObj : public TexBody{

	btSoftBody *psb;//����ָ��
	btSoftRigidDynamicsWorld *m_dynamicsWorld;//������������ָ��
	btSoftBodyWorldInfo m_softBodyWorldInfo;//��������������Ϣ
	AnyShapeNoTexture* asbv;//���ƶ���ָ��
	float* m_vertices;//��������
	int m_texId;//����id
	int m_numT;//�����θ���
	float* m_gVertices;//��������
	int m_nums_ver;//�������鳤��
	int* m_gIndices;//������������
	int m_nums_ind;//�����������鳤��
	int m_numsTriangles;//�����θ���

	btVector3 m_pos;//λ������

public:
	SoftObj(btSoftRigidDynamicsWorld* m_dynamicsWorld,
			btSoftBodyWorldInfo& m_softBodyWorldInfo,
			btVector3 pos,
			float* m_gVertices,
			int m_nums_ver,
			int* m_gIndices,
			int m_nums_ind,
			float scale
			);//���캯��
//	virtual ~SoftObj();

	void drawSelf();//����������
	btRigidBody* getBody();//��ȡ����ָ�뷽��
	void initVertex();//��ʼ���������ݷ���
	void initObj();//��ʼ�����巽��
	btSoftBody*	createSoftBody(const btVector3& x,const btVector3& a);//�������巽��
};

#endif /* JNI_HELLOCPP_SoftObj_H_ */

