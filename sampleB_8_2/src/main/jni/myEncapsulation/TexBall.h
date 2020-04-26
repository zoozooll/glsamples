#ifndef TexBall__h
#define TexBall__h

#include "TexBody.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/LinearMath/btVector3.h"
#include "Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "MatrixState.h"
#include "geometry/Ball.h"

class TexBall:public TexBody{
    float r;//表示球的半径
	btRigidBody *body;//表示球刚体指针
	int texId;//球纹理id
	Ball* ball;//绘制球指针
public:

	TexBall(float r,
			int texId,
			btDynamicsWorld *dynamicsWorld,
			btScalar mass,
			btVector3 pos,
			btScalar restitutionIn,
			btScalar frictionIn);
    void drawSelf();//绘制自身刚体的方法
	btRigidBody* getBody();//获取自身刚体指针的方法
};


#endif

//{
//	tbTemp=new TexBall(UNIT_SIZE,ballTexId,ballShape,m_dynamicsWorld,10,0,0,0, 0.6, 0.8);
//	tca.push_back(tbTemp);
//}
