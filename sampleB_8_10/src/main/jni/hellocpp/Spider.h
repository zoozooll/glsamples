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

//��ż
class Spider : public TexBody
{

	btDynamicsWorld* m_ownerWorld;

	TexBody* tbTemp;		//��ʱ�����װ�����

	btRigidBody*		m_bodies[BODYPART_COUNT];
	btTypedConstraint*	m_joints[JOINT_COUNT];
	vector<TexBody*> tca;	//�洢�����װ������vector������ �����壬���壬���棩

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

