#ifndef __TexBody_h
#define __TexBody_h
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
class TexBody{
public:
	virtual void drawSelf() = 0;
	virtual btRigidBody* getBody() = 0;
};
#endif
