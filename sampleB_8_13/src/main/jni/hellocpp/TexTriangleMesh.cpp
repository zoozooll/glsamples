#include "TexTriangleMesh.h"
#include "../myEncapsulation/ShaderUtil.h"
#include "../Bullet/BulletCollision/Gimpact/btGImpactShape.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/BulletCollision/CollisionShapes/btShapeHull.h"

#include "../Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"


#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

TexTriangleMesh::TexTriangleMesh(
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
	)
{
	for(int i = 0; i<numsVer; i++)//遍历顶点数组
	{
		vertices[i] = vertices[i]*scale;//更改顶点数据
	}

	this->triangleTexId = triangleTexId;//记录纹理id

	int totalTriangles = numsInd / 3;//计算三角形总数 (一个顶点包含三个坐标xyz,一个三角形是由三个顶点组成的)
	int numsVertices = numsInd;//记录顶点总数（  有重复值）

	int vertStride = 3*sizeof(float);//顶点数组步长//一个顶点所占的字节数  4*4 提示:xyzw四个坐标值 * 每一个坐标所在字节数
	int indexStride = 3*sizeof(int);//索引步长  //三个int类型所占字节数  3*4   提示：gIndices数组是int类型的数组

	//创建三角形索引数组类对象
	btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(
			totalTriangles,					//三角形数
			indices,						//索引数组
			indexStride,					//索引步长   3*4（3个数 * int类型所占的字节数4）
			numsVertices,						//所有的顶点数
			(btScalar*) &vertices[0],	//顶点数组的首地址
			vertStride);					//顶点步长    4*4（一个顶点包含 4个坐标 * 一个坐标所占的字节数4）


	//创建固定三角形形状类对象
	btGImpactMeshShape * gims = new btGImpactMeshShape(m_indexVertexArrays);
	gims->updateBound();//更新范围
	btCollisionShape* finalShape = gims;

	btCollisionDispatcher * dispatcher = static_cast<btCollisionDispatcher *>(dynamicsWorld ->getDispatcher());//获取调度器
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);//注册算法

	//创建绘制对象
	ctm = new AnyShapeByVectices(
			vertices,//顶点坐标数组    无重复值
			numsVer,//顶点坐标数组的长度
			indices,
			numsInd,
			texs,
			numsTex,
			false,
			1.0f
			);//三角形问题


	bool isDynamic = (mass != 0);//物体是否可以运动
	btVector3 localInertia = btVector3(0, 0, 0);//惯性向量
	if(isDynamic)
	{
		finalShape->calculateLocalInertia(mass, localInertia);//计算惯性向量
	}
	btTransform transform = btTransform();//创建刚体的初始变换对象
	transform.setIdentity();//变换对象初始化
	transform.getOrigin().setValue(pos.x(),pos.y(),pos.z());//设置变换对象的位置
	//创建刚体运动状态对象
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);
	//创建刚体信息对象
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, finalShape, localInertia);
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	body = new btRigidBody(rbInfo);//创建刚体
	dynamicsWorld->addRigidBody(body);//将刚体添加进物理世界

}

void TexTriangleMesh::drawSelf()
{
	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = body->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

	MatrixState::pushMatrix();//保护现场
	ctm->drawSelf(triangleTexId);//绘制固定形状类对象
	MatrixState::popMatrix();//恢复现场


	MatrixState::popMatrix();//恢复现场
}

btRigidBody* TexTriangleMesh::getBody()
{
	return body;
}
