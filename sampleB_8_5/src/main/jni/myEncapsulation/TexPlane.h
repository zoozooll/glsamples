#ifndef TexPlane__h
#define TexPlane__h

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "TexBody.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/LinearMath/btVector3.h"
#include "geometry/TexturePlane.h"

class TexPlane:public TexBody{
    int vCount;
    float UNIT_SIZE;
	btRigidBody *body;	//对应的刚体对象
	int texId;
	TexturePlane* tp;
public:
	TexPlane(float aSize,//地面是正方形的，这个参数表示边长的一半
			int texId,
			btCollisionShape *planeShape,
			btDynamicsWorld *dynamicsWorld,
			btScalar cx,btScalar cy,btScalar cz,
			btScalar restitutionIn,
			btScalar frictionIn);
    void drawSelf();
	btRigidBody* getBody();
};


#endif
