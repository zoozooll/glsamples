#ifndef _Landform_H_
#define _Landform_H_

#include "../myEncapsulation/TexBody.h"
#include "../myEncapsulation/geometry/AnyShapeByVectices.h"
#include "../Bullet/btBulletDynamicsCommon.h"
#include "../Bullet/btBulletCollisionCommon.h"

#include "../Bullet/LinearMath/btQuickprof.h"


class Landform : public TexBody
{

	AnyShapeByVectices* asbi;//绘制指针

	btDynamicsWorld *m_dynamicsWorld;//物理世界指针
	btBvhTriangleMeshShape* trimeshShape;//形状指针   trimesh不规则三角网格
	btRigidBody* body;

	float waveheight;//波动高度
	float TRIANGLE_SIZE;//三角形边长的单位大小
	int texId;//纹理id

	int NUM_VERTS_X;//x方向上点的数目。有x-1列	 取值大于等于2
	int NUM_VERTS_Y;//y方向上点的数目。有y-1行     取值大于等于2

	btVector3*	gVertices;		//顶点数组指针
	int* gIndices;		//索引数组指针

	float* tempVer;

public:
	Landform(
			btDynamicsWorld *dynamicsWorldIn,//物理世界指针
			float waveheightIn,//地形起伏的高度
			float triangleUnitSizeIn, //每一个三角形的边长的单位大小
			float numsXIn,//x方向上点的数目
			float numsYIn,//y方向上点的数目
			int texIdIn,//纹理id
			float restitutionIn,//恢复系数
			float frictionIn,//摩擦力
			btVector3 pos//位置
			);
	void drawSelf();
	btRigidBody* getBody();
	void setVertexPositions(float waveheight, float offset);//起伏的高度，位移

};

#endif


//{
//	tbTemp = new Landform(
//			m_dynamicsWorld,//物理世界指针
//			5.0f,//地形起伏的高度
//			8.0f, //每一个三角形的边长的单位大小
//			30,//x方向上点的数目
//			30,//y方向上点的数目
//			planeTexId,//纹理id
//			0.1f,//恢复系数
//			0.5f,//摩擦力
//			btVector3(0.0f, -16.0f, 0.0f)//位置
//			);
//	//将新立方体加入到列表中
//	tca.push_back(tbTemp);
//}
