#ifndef _MyRaycast_H_
#define _MyRaycast_H_

#include "../Bullet/LinearMath/btVector3.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../myEncapsulation/geometry/DrawLine.h"

class MyRaycast
{
	DrawLine* dl;
	btCollisionWorld* m_cw;//物理世界指针

	btVector3* m_vector;//顶点数组   顶点顺序有要求：第一点和第二点是一组，构成一条线。第三点和第四点是一组，构成一条线......
	int nums_m_vector;//顶点数组长度		必须为偶数

	btVector3* hit;//碰撞接触点		长度应该为vCountLine
	btVector3* normals;//法向量	长度应该为vCountLine
	btVector3* reflectedRays;

	int vCountLine;//线的总数
public :
	MyRaycast(
			btVector3* m_vector, //顶点数组   顶点顺序有要求：第一点和第二点是一组，构成一条线。第三点和第四点是一组，构成一条线
			int nums_m_vector, //顶点数组长度		必须为偶数
			btCollisionWorld* m_cw//物理世界指针
			);
//	~MyRaycast();

	void cast();//投射方法

	btVector3 reflectedRay(btVector3 n1,btVector3 n2);//n1入射线   n2法向量

	void drawSelf();//此方法应在cast方法之后调用

};

#endif
