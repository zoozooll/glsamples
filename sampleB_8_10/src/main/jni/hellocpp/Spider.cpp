#include "Spider.h"
#include "../myEncapsulation/TexCapsule.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif

Spider::Spider(btDynamicsWorld* ownerWorld,
			btVector3 positionOffset,//const
			btScalar scale,
			int boxTexId,
			int ballTexId
			)
{
	this->m_ownerWorld = ownerWorld;//给全局的物理世界指针赋值

	float fBodySize  = 0.25f * scale;//宽度
	float fLegLength = 0.45f * scale;//腿的长度
	float fForeLegLength = 0.75f * scale;//前腿的长度
	int i;
	float fHeight = 0.5 * scale;//高度
	btTransform offset;//变换对象
	offset.setIdentity();//变换对象初始化
	offset.setOrigin(positionOffset);//给变换对象设置起点

	// root
	btVector3 vRoot = btVector3(btScalar(0.), btScalar(fHeight), btScalar(0.));//中间刚体的位置
	btTransform transform;//中间刚体的变换对象
	transform.setIdentity();//变换对象初始化
	transform.setOrigin(vRoot);//给变换对象设置位置
	tbTemp = new TexCapsule(
		1.0f,//质量
		fBodySize,//半径
		0.10 * scale,//高度
		offset*transform,//位置
		0.0f,//恢复系数
		0.0f,//摩擦系数
		ownerWorld,//物理世界指针
		boxTexId,//胶囊侧边纹理id
		ballTexId,//底部圆纹理id
		ballTexId,//顶部圆纹理id
		30//被切分的份数  n
		);//创建中间刚体对象
	tca.push_back(tbTemp);//将中间刚体加入到列表中
	m_bodies[0] = tbTemp->getBody();//记录刚体指针
	// legs
	for ( i=0; i<NUM_LEGS; i++)
	{
		float fAngle = 2 * M_PI * i / NUM_LEGS;
		float fSin = sin(fAngle);
		float fCos = cos(fAngle);

		transform.setIdentity();
		btVector3 vBoneOrigin = btVector3(btScalar(fCos*(fBodySize+0.5*fLegLength)), btScalar(fHeight), btScalar(fSin*(fBodySize+0.5*fLegLength)));
		transform.setOrigin(vBoneOrigin);

		// thigh
		btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
		btVector3 vAxis = vToBone.cross(btVector3(0, 1, 0));
		transform.setRotation(btQuaternion(vAxis, M_PI_2));
//		m_bodies[1+2*i] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[1+2*i]);
		tbTemp = new TexCapsule(
			1.0f,//质量
			0.10f * scale,//半径
			fLegLength,//高度
			offset*transform,//位置
			0.5f,//恢复系数
			0.5f,//摩擦系数
			ownerWorld,//物理世界指针
			boxTexId,//胶囊侧边纹理id
			ballTexId,//底部圆纹理id
			ballTexId,//顶部圆纹理id
			30//被切分的份数  n
			);
		tca.push_back(tbTemp);//将新立方体加入到列表中

		m_bodies[1+2*i] = tbTemp->getBody();

		// shin
		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(fHeight-0.5*fForeLegLength), btScalar(fSin*(fBodySize+fLegLength))));
//		m_bodies[2+2*i] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[2+2*i]);
		tbTemp = new TexCapsule(
			1.0f,//质量
			0.08f * scale,//半径
			fForeLegLength,//高度
			offset*transform,//位置
			0.5f,//恢复系数
			0.5f,//摩擦系数
			ownerWorld,//物理世界指针
			boxTexId,//胶囊侧边纹理id
			ballTexId,//底部圆纹理id
			ballTexId,//顶部圆纹理id
			30//被切分的份数  n
			);
		tca.push_back(tbTemp);//将新立方体加入到列表中

		m_bodies[2+2*i] = tbTemp->getBody();
	}

	// Setup some damping on the m_bodies
	for (i = 0; i < BODYPART_COUNT; ++i)//遍历所有的刚体
	{
		m_bodies[i]->setDamping(0.05, 0.85);//设置刚体的阻尼
		m_bodies[i]->setDeactivationTime(0.8);//设置刚体的变形时间
		//m_bodies[i]->setSleepingThresholds(1.6, 2.5);
		m_bodies[i]->setSleepingThresholds(0.5f, 0.5f);//设置睡眠阈值
	}


	//
	// Setup the constraints
	//
	btGeneric6DofConstraint * joint6DOF;//六自由度关节指针

	btTransform localA, localB, localC;//变化对象

	bool useLinearReferenceFrameA = true;//对应关系标志

	for ( i=0; i<NUM_LEGS; i++)//遍历所有的腿刚体
	{
		float fAngle = 2 * M_PI * i / NUM_LEGS;//计算角度
		float fSin = sin(fAngle);//角度的正弦值
		float fCos = cos(fAngle);//角度的余弦值

		// hip joints
		localA.setIdentity(); //变换对象初始化
		localB.setIdentity();//变换对象初始化
		localA.getBasis().setEulerZYX(0,-fAngle,0);	//设置欧拉角
		localA.setOrigin(btVector3(btScalar(fCos*fBodySize), btScalar(0.), btScalar(fSin*fBodySize)));//设置原点
		localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;//计算变换对象
		joint6DOF = new btGeneric6DofConstraint(*m_bodies[0], *m_bodies[1+2*i], localA, localB,useLinearReferenceFrameA);//创建六自由度关节对象
		joint6DOF->setAngularLowerLimit(btVector3(-SIMD_PI*0.05f,-SIMD_EPSILON,-SIMD_PI*0.05f));//设置六自由度关节的转动角下限
		joint6DOF->setAngularUpperLimit(btVector3(SIMD_PI*0.05f,SIMD_EPSILON,SIMD_PI*0.051));//设置六自由度关节的转动角上限
		m_joints[2*i] = joint6DOF;//记录关节对象
		m_ownerWorld->addConstraint(m_joints[2*i], true);//将关节对象添加到物理世界中

		// knee joints
		localA.setIdentity(); //变换对象初始化
		localB.setIdentity(); //变换对象初始化
		localC.setIdentity();//变换对象初始化
		localA.getBasis().setEulerZYX(0,-fAngle,0);	localA.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(0.), btScalar(fSin*(fBodySize+fLegLength))));//设置欧拉角
		localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;//计算变换对象
		localC = m_bodies[2+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;//计算变换对象
		joint6DOF = new btGeneric6DofConstraint(*m_bodies[1+2*i], *m_bodies[2+2*i], localB, localC,useLinearReferenceFrameA);//创建六自由度关节对象
		joint6DOF->setAngularLowerLimit(btVector3(-SIMD_PI*0.05f,-SIMD_EPSILON,-SIMD_PI*0.05f));//设置六自由度关节的转动角下限
		joint6DOF->setAngularUpperLimit(btVector3(SIMD_PI*0.05f,SIMD_EPSILON,SIMD_PI*0.05f));//设置六自由度关节的转动角上限
		m_joints[1+2*i] = joint6DOF;//记录关节对象
		m_ownerWorld->addConstraint(m_joints[1+2*i], true);//将关节对象添加到物理世界中
	}

}


btRigidBody* Spider::getBody(){}

void Spider::drawSelf()
{
    for ( int i = 0; i < tca.size(); ++i )
    {
    	tca[i]->drawSelf();//回调刚体的绘制方法
    }
}

