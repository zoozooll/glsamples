#ifndef _TexCuboid_H_
#define _TexCuboid_H_

#include "TexBody.h"
#include "../Bullet/BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "geometry/TextureRect.h"

//长方体物体类
class TexCuboid : public TexBody
{
	btRigidBody* body;//刚体对象指针

	float offset_x;//x方向上的位移
	float offset_y;//y方向上的位移
	float offset_z;//z方向上的位移

	TextureRect* frontBackRect;//前后的长方形
	TextureRect* topBottomRect;//上下的长方形
	TextureRect* leftRightRect;//左右的长方形

	int frontTexId;//前侧纹理图id
	int backTexId;//后侧纹理图id
	int topTexId;//上侧纹理图id
	int bottomTexId;//下侧纹理图id
	int leftTexId;//左侧纹理图id
	int rightTexId;//右侧纹理图id

public:
	TexCuboid(
			btDynamicsWorld* m_dynamicsWorldIn,//物理世界对象
			btVector3 boxHalfExtentsIn,//长方体的半区域
			btScalar mass,//长方体的质量
			btVector3 pos,//刚体的位置
			btScalar restitutionIn,//恢复系数
			btScalar frictionIn,//摩擦系数
			float UNIT_SIZE,//单位长度
			int frontTexIdIn,//前侧纹理图id
			int backTexIdIn,//后侧纹理图id
			int topTexIdIn,//上侧纹理图id
			int bottomTexIdIn,//下侧纹理图id
			int leftTexIdIn,//左侧纹理图id
			int rightTexIdIn//右侧纹理图id
	);
//	~TexCuboid();
	void drawSelf();
	btRigidBody* getBody();
};

#endif
