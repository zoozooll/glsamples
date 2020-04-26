#ifndef _MyCompoundShape_H_
#define _MyCompoundShape_H_

#include <string.h>
#include <jni.h>
#include <vector>
#include "../Bullet/LinearMath/btAlignedObjectArray.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/LinearMath/btVector3.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "myEncapsulation/MatrixState.h"
#include "../myEncapsulation/TexCuboid.h"
#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/TexPlane.h"
#include "../myEncapsulation/TexBall.h"
#include "../myEncapsulation/TexCapsule.h"
#include "../myEncapsulation/TexCone.h"
#include "../myEncapsulation/TexCylinderShape.h"


class MyCompoundShape : public TexBody
{
	btRigidBody* body;//刚体指针
	TexCapsule* tc;//胶囊物体指针
	TexCylinderShape* tcs;//圆柱形物体指针
public:
	MyCompoundShape(
			btDynamicsWorld* dynamicsWorld,
			btScalar mass,
			btVector3 pos,
			float restitutionIn,
			float frictionIn,
			int texId_capusleSide,//胶囊侧边纹理id
			int texId_buttomBall,//底部圆纹理id
			int texId_topBall,//顶部圆纹理id
			int texId_bottomCircle,//底部圆形的纹理id
			int texId_topCircle,//顶部圆形的纹理id
			int texId_cylinderSide//圆柱侧面的纹理id
			);
    void drawSelf();
    btRigidBody* getBody();
};

#endif
