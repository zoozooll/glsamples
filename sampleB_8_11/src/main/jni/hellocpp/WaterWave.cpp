#include "WaterWave.h"
#include "../myEncapsulation/MatrixState.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

WaterWave::WaterWave(
			btDynamicsWorld *dynamicsWorldIn,//物理世界指针
			float waveheightIn,//地形起伏的高度
			float triangleUnitSizeIn, //每一个三角形的边长的单位大小
			float numsXIn,//x方向上点的数目
			float numsYIn,//y方向上点的数目
			int texIdIn,//纹理id
			float restitutionIn,//恢复系数
			float frictionIn,//摩擦力
			btVector3 pos//位置
			)
{
	this->m_dynamicsWorld = dynamicsWorldIn;//记录物理世界对战指针
	this->waveheight = waveheightIn;//记录水纹波动的高度
	this->TRIANGLE_SIZE = triangleUnitSizeIn;//记录三角形单位大小
	//记录列数
	this->NUM_VERTS_X = numsXIn;//x方向的点的数目	大于等于2
	//记录行数
	this->NUM_VERTS_Y = numsYIn;//y方向的点的数目	大于等于2
	this->texId = texIdIn;//记录纹理坐标

	int totalVerts = NUM_VERTS_X*NUM_VERTS_Y;//总的顶点数，不含重复顶点
	int totalTriangles = 2*(NUM_VERTS_X-1)*(NUM_VERTS_Y-1);//三角形总数
	gVertices = new btVector3[totalVerts];//顶点数组（无重复顶点）
	gIndices = new int[totalTriangles*3];//存储顶点的索引顺序，有 三角形总数*3 个数
	m_vertices = new float[totalVerts*3];//创建顶点数组

	setVertexPositions(waveheightIn,0.f);//对顶点数组进行赋值
	//对顶点索引数组赋值	注意：逆时针
	int index=0;
	for (int i=0;i<NUM_VERTS_X-1;i++)//遍历每一列
	{
		for (int j=0;j<NUM_VERTS_Y-1;j++)//遍历每一行
		{
			gIndices[index++] = j*NUM_VERTS_X+i;//第一个三角形的第一个索引值
			gIndices[index++] = j*NUM_VERTS_X+i+1;//第一个三角形的第二个索引值
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;//第一个三角形的第三个索引值

			gIndices[index++] = j*NUM_VERTS_X+i;//第二个三角形的第一个索引值
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;//第二个三角形的第二个索引值
			gIndices[index++] = (j+1)*NUM_VERTS_X+i;//第二个三角形的第三个索引值
		}
	}

	//创建三角形索引组对象
	btTriangleIndexVertexArray* m_indexVertexArrays = new btTriangleIndexVertexArray(
			totalTriangles,					//三角形数
			gIndices,						//索引数组
			3*sizeof(int),					//索引步长   3*4（3个数 * int类型所占的字节数4）
			totalVerts,						//所有的顶点数
			(btScalar*) &gVertices[0].x(),	//顶点数组的首地址
			sizeof(btVector3));					//顶点步长    4*4（一个顶点包含 4个坐标 * 一个坐标所占的字节数4）

	bool useQuantizedAabbCompression = true;
	btVector3 aabbMin(-1000,-1000,-1000),aabbMax(1000,1000,1000);//包围盒
	//创建曲面地形形状对象
	trimeshShape  = new btBvhTriangleMeshShape(m_indexVertexArrays,useQuantizedAabbCompression,aabbMin,aabbMax);
	float mass = 0.f;//质量参数,该类对象主要用来表示形状比较复杂的地形、山岗等。需要注意的是，它描述的物体质量应为零。
	btTransform	startTransform;//变换对象
	startTransform.setIdentity();//变换对象初始化
	startTransform.setOrigin(btVector3(pos.x(),pos.y(),pos.z()));//设置原点位置
	btVector3 localInertia(0,0,0);//惯性向量
	//创建刚体的运动状态对象
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	//创建刚体信息对象
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,(btCollisionShape*)trimeshShape,localInertia);
	rbInfo.m_restitution = restitutionIn;//设置反弹系数
	rbInfo.m_friction = frictionIn;//设置摩擦系数
	body = new btRigidBody(rbInfo);//创建刚体
	m_dynamicsWorld->addRigidBody(body);//将刚体添加进物理世界

	//设置碰撞检测标志
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);//STATIC_OBJECT);
	//enable custom material callback
	//设置碰撞检测标志
	body->setCollisionFlags(body->getCollisionFlags()  | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	body->setActivationState(DISABLE_DEACTIVATION);//设置刚体为动态的

	//创建绘制对象
	asbi = new AnyShapeByVectices(
			m_vertices,
			totalVerts*3,//顶点总数（无重复值）
			gIndices,//索引数组	(unsigned int*)gIndices
			totalTriangles*3,//索引总数
			-1.0f
			);
}

btScalar WaterWave::getDeltaTimeMicroseconds()
{
	btScalar dt = (btScalar)m_clock.getTimeMicroseconds();//获取时间间隔
	m_clock.reset();//重置时钟
	return dt;//返回时间间隔
}

void WaterWave::drawSelf()
{
	float dt = getDeltaTimeMicroseconds() * 0.000001f;//位移增量

	static float offset=0.f;
	offset+=dt;//位移

	btVector3 aabbMin(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
	btVector3 aabbMax(-BT_LARGE_FLOAT,-BT_LARGE_FLOAT,-BT_LARGE_FLOAT);

	for (int i=0;i<NUM_VERTS_X;i++)//遍历每一列
	{
		for (int j=0;j<NUM_VERTS_Y;j++)//遍历每一行
		{

			aabbMax.setMax(gVertices[i+j*NUM_VERTS_X]);
			aabbMin.setMin(gVertices[i+j*NUM_VERTS_X]);

			gVertices[i+j*NUM_VERTS_X].setValue((i-NUM_VERTS_X*0.5f)*TRIANGLE_SIZE,
				//0.f,
				waveheight*sinf((float)i+offset)*cosf((float)j+offset),
				(j-NUM_VERTS_Y*0.5f)*TRIANGLE_SIZE);

			aabbMin.setMin(gVertices[i+j*NUM_VERTS_X]);
			aabbMax.setMax(gVertices[i+j*NUM_VERTS_X]);

		}
	}
	trimeshShape->partialRefitTree(aabbMin,aabbMax);//对形状局部优化


	for(int i = 0; i<NUM_VERTS_X*NUM_VERTS_Y; i++)//遍历所有的顶点
	{
		m_vertices[i*3+0] = gVertices[i].x();//顶点的x坐标
		m_vertices[i*3+1] = gVertices[i].y();//顶点的y坐标
		m_vertices[i*3+2] = gVertices[i].z();//顶点的z坐标
	}

	MatrixState::pushMatrix();//保护现场

	btTransform trans;
	trans = body->getWorldTransform();				//获取刚体的变换信息对象
	trans.getOpenGLMatrix(MatrixState::currMatrix);		//将当前的矩阵设置给变换信息对象

	MatrixState::pushMatrix();//保护现场
	asbi->drawSelf(texId);//绘制水纹
	MatrixState::popMatrix();//恢复现场


	MatrixState::popMatrix();//恢复现场
}

//设置顶点的位置
void WaterWave::setVertexPositions(float waveheight, float offset)//offset位移
{
	for (int i=0;i<NUM_VERTS_X;i++)//遍历每一列
	{
		for (int j=0;j<NUM_VERTS_Y;j++)//遍历每一行
		{
			//给每个点赋三维坐标
			gVertices[i+j*NUM_VERTS_X].setValue(
					(i-NUM_VERTS_X*0.5f)*TRIANGLE_SIZE,
					waveheight*sinf((float)i+offset)*cosf((float)j+offset),
					(j-NUM_VERTS_Y*0.5f)*TRIANGLE_SIZE
				);
		}
	}
	for(int i = 0; i<NUM_VERTS_X*NUM_VERTS_Y; i++)//遍历所有的顶点
	{
		m_vertices[i*3+0] = gVertices[i].x();//顶点的x坐标
		m_vertices[i*3+1] = gVertices[i].y();//顶点的y坐标
		m_vertices[i*3+2] = gVertices[i].z();//顶点的z坐标
	}
}

btRigidBody* WaterWave::getBody()
{
	return body;
}
