#include "MyCompoundShape.h"

MyCompoundShape::MyCompoundShape(
		btDynamicsWorld* dynamicsWorld,
		btScalar mass,
		btVector3 pos,
		float restitutionIn,
		float frictionIn,
		int texId_capusleSide,//胶囊侧边纹理id
		int texId_buttomBall,//底部圆纹理id
		int texId_topBall,//顶部圆纹理id
		int texId_bottomCircle,//底部圆形的纹理id
		int texId_topCircle,//顶部圆形的纹理id
		int texId_cylinderSide//圆柱侧面的纹理id
		)
{

	int scale = 4;//设置缩放比
	btVector3 halfExtents(1,0.125,1);//圆柱的半区域
	halfExtents = halfExtents * scale;
	float radio = 0.05f * scale;//表示胶囊两侧球的半径
	float height = 1.5f * scale;//表示胶囊中间圆柱的高度

	btCollisionShape* top  = new btCylinderShape(halfExtents);//创建圆柱
	btCollisionShape* pin  = new btCapsuleShape(radio,height);//创建胶囊
	top->setMargin(0.01);//设置圆柱的边缘值
	pin->setMargin(0.01);//设置胶囊的边缘值
	btCompoundShape* compound = new btCompoundShape();//创建复合形状对象
	compound->addChildShape(btTransform::getIdentity(),top);//将圆柱形状添加到复合形状中
	compound->addChildShape(btTransform::getIdentity(),pin);//将胶囊形状添加到复合形状中
	btVector3 localInertia;//惯性向量
	top->calculateLocalInertia(mass,localInertia);//计算惯性
	body = new btRigidBody(mass,0,compound,localInertia);//创建刚体
	btTransform tr;
	tr.setIdentity();//变换对象初始化
	tr.setOrigin(pos);//设置位置
	body->setCenterOfMassTransform(tr);//设置中心点
	body->setAngularVelocity(btVector3(0,10,0));//设置角速度
	body->setLinearVelocity(btVector3(0,0,0.2));//设置线速度
	body->setFriction(btSqrt(1));//设置摩擦系数
	dynamicsWorld->addRigidBody(body);//将刚体添加到物理世界中
	body->setDamping(0.00001f,0.0001f);//设置阻尼

	tc = new TexCapsule(
			radio,//半径
			height,//高度
			texId_capusleSide,//胶囊侧边纹理id
			texId_buttomBall,//底部圆纹理id
			texId_topBall,//顶部圆纹理id
			30//被切分的份数  n
			);//创建绘制胶囊对象
	tcs = new TexCylinderShape(
			texId_bottomCircle,//底部圆形的纹理id
			texId_topCircle,//顶部圆形的纹理id
			texId_cylinderSide,//圆柱侧面的纹理id
			30,//切分份数
			halfExtents//半区域
			);//初始圆柱侧面平行于y轴，圆形是平行xoz界面上的//创建绘制圆柱对象

}

void MyCompoundShape::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = body->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

	if(body->isActive())//判断刚体是否可动
	{
		tc->capuleSide->mFlagK = 0;//设置胶囊纹理属性
		tc->bottomBall->mFlagK = 0;//设置胶囊纹理属性

		tcs->bottomCircle->mFlagK = 0;//设置圆柱纹理属性
		tcs->cylinderSide->mFlagK = 0;//设置圆柱纹理属性
	}
	else
	{
		tc->capuleSide->mFlagK = 1;//设置胶囊纹理属性
		tc->bottomBall->mFlagK = 1;//设置胶囊纹理属性

		tcs->bottomCircle->mFlagK = 1;//设置圆柱纹理属性
		tcs->cylinderSide->mFlagK = 1;//设置圆柱纹理属性
	}

	tc->drawSelfNoBody();//绘制胶囊
	tcs->drawSelfNoBody();//绘制圆柱

	MatrixState::popMatrix();//恢复现场
}

btRigidBody* MyCompoundShape::getBody()
{
	return body;//返回自身刚体指针
}
