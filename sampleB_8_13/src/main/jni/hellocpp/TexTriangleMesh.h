#ifndef _TexTriangleMesh_H_
#define _TexTriangleMesh_H_

#include "../myEncapsulation/TexBall.h"
#include "../myEncapsulation/MatrixState.h"
#include "../Bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "../Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "../Bullet/LinearMath/btVector3.h"

#include "../myEncapsulation/geometry/AnyShapeByVectices.h"

class TexTriangleMesh : public TexBody
{
	btDynamicsWorld* m_dynamicsWorld;//物理世界指针
	btRigidBody* body;//刚体对象指针
	AnyShapeByVectices* ctm;//绘制对象
	int triangleTexId;//纹理id
public :
	TexTriangleMesh(
		btScalar mass,//质量

	float* vertices,//顶点坐标数组    无重复值
	int numsVer,//顶点坐标数组的长度
	int* indices,
	int numsInd,
	float* texs,
	int numsTex,

	float scale,//缩放因子

		btVector3 pos,//位置
		btScalar restitutionIn,//恢复系数
		btScalar frictionIn,//摩擦系数
		btDynamicsWorld *dynamicsWorld,//物理世界指针
		int triangleTexId//三角形纹理id
		);//构造函数
//	~TexTriangleMesh();
	void drawSelf();//绘制自身的方法
	btRigidBody* getBody();//获取刚体指针方法
};

#endif
