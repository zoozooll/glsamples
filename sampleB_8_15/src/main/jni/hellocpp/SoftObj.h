/*
 * SoftObj.h
 *
 *  Created on: 2015年1月31日
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

	btSoftBody *psb;//软体指针
	btSoftRigidDynamicsWorld *m_dynamicsWorld;//软体物理世界指针
	btSoftBodyWorldInfo m_softBodyWorldInfo;//软体物理世界信息
	AnyShapeNoTexture* asbv;//绘制对象指针
	float* m_vertices;//顶点数组
	int m_texId;//纹理id
	int m_numT;//三角形个数
	float* m_gVertices;//顶点数组
	int m_nums_ver;//顶点数组长度
	int* m_gIndices;//顶点索引数组
	int m_nums_ind;//顶点索引数组长度
	int m_numsTriangles;//三角形个数

	btVector3 m_pos;//位置坐标

public:
	SoftObj(btSoftRigidDynamicsWorld* m_dynamicsWorld,
			btSoftBodyWorldInfo& m_softBodyWorldInfo,
			btVector3 pos,
			float* m_gVertices,
			int m_nums_ver,
			int* m_gIndices,
			int m_nums_ind,
			float scale
			);//构造函数
//	virtual ~SoftObj();

	void drawSelf();//绘制自身方法
	btRigidBody* getBody();//获取刚体指针方法
	void initVertex();//初始化顶点数据方法
	void initObj();//初始化软体方法
	btSoftBody*	createSoftBody(const btVector3& x,const btVector3& a);//创建软体方法
};

#endif /* JNI_HELLOCPP_SoftObj_H_ */

