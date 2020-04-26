/*
 * SoftRope.h
 *
 *  Created on: 2015年1月31日
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


	btSoftBody *psb;//软体指针
	btSoftRigidDynamicsWorld *m_dynamicsWorld;//软体物理世界指针
	btSoftBodyWorldInfo m_softBodyWorldInfo;//软体物理世界信息
	int m_ropeId;
	int m_ballId;
	int m_boxId;

	DrawRope* dr;//绘制绳索类指针

	TexBody* tbBall;//小球类指针
	TexBody* tbCuboid;//木块类指针
	btVector3* m_vec_rope;//绳索数据数组
	btVector3 m_pos;//小球位置坐标

public:
	SoftRope(btSoftRigidDynamicsWorld* m_dynamicsWorld,
			btSoftBodyWorldInfo& m_softBodyWorldInfo,
			btVector3 m_pos,
			int ropeId,//纹理id
			int ballId,
			int boxId
			);//构造函数
//	virtual ~SoftRope();

	void drawSelf();//绘制绳索、小球和木块的方法
	btRigidBody* getBody();//获取刚体指针方法
	void initVertex();//初始化顶点数据方法
	void init();//创建绳索、小球和木块的方法
};

#endif /* JNI_HELLOCPP_SOFTROPE_H_ */


//		{
//			sd = new SoftRope(m_dynamicsWorld,
//					m_softBodyWorldInfo,
//					boxTexId//纹理id
//					);
//			tca.push_back(sd);
//		}
