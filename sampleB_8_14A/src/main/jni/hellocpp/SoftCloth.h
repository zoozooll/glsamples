/*
 * SoftCloth.h
 *
 *  Created on: 2015年1月31日
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

    int m_numT;//三角形个数
    float *m_vertices;//顶点数组
	btSoftBody *psb;//软体指针
	btSoftRigidDynamicsWorld *m_dynamicsWorld;//软体物理世界对象指针
	btSoftBodyWorldInfo m_softBodyWorldInfo;//软体物理世界信息
	AnyShapeNoTexture* m_asbv;//绘制对象指针
	int m_texId;//纹理id
	btVector3 m_pos;//软布的中心位置

public:
	SoftCloth(btSoftRigidDynamicsWorld* m_dynamicsWorld,
			btSoftBodyWorldInfo& m_softBodyWorldInfo,
			btVector3 pos
			);//构造函数
//	virtual ~SoftCloth();

	void drawSelf();//绘制软布的方法
	btRigidBody* getBody();//返回刚体指针
	void initVertex();//初始化顶点位置
	void initCloth();//初始化软布方法
};

#endif /* JNI_HELLOCPP_SOFTCLOTH_H_ */

