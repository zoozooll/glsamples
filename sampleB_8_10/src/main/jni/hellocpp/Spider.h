#ifndef _Spider_H_
#define _Spider_H_

#include <vector>
#include "../myEncapsulation/TexBody.h"
#include "../Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "../Bullet/BulletDynamics/ConstraintSolver/btTypedConstraint.h"

using namespace std;

#define NUM_LEGS 6
#define BODYPART_COUNT 2 * NUM_LEGS + 1
#define JOINT_COUNT BODYPART_COUNT - 1

//布偶
class Spider : public TexBody
{

	btDynamicsWorld* m_ownerWorld;

	TexBody* tbTemp;		//临时刚体封装类对象

	btRigidBody*		m_bodies[BODYPART_COUNT];
	btTypedConstraint*	m_joints[JOINT_COUNT];
	vector<TexBody*> tca;	//存储刚体封装类对象的vector（包括 立方体，球体，地面）

public:


	Spider(btDynamicsWorld* ownerWorld,
				btVector3 positionOffset,//const
				btScalar scale,
				int boxTexId,
				int ballTexId
				);
	void drawSelf();
	btRigidBody* getBody();
//	btTypedConstraint** GetJoints() {return &m_joints[0];}

//	~Spider ();
};

#endif

