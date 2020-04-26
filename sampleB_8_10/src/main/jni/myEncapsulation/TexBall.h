#ifndef TexBall__h
#define TexBall__h

#include "TexBody.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/LinearMath/btVector3.h"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "MatrixState.h"
#include "geometry/Ball.h"

class TexBall:public TexBody{
    float r;//��ʾ��İ뾶
	btRigidBody *body;//��ʾ�����ָ��
	int texId;//������id
	Ball* ball;//������ָ��
public:

	TexBall(float r,
			int texId,
			btDynamicsWorld *dynamicsWorld,
			btScalar mass,
			btVector3 pos,
			btScalar restitutionIn,
			btScalar frictionIn);
    void drawSelf();//�����������ķ���
	btRigidBody* getBody();//��ȡ�������ָ��ķ���
};


#endif
