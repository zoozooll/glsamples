#include "MyRaycast.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MyRaycast::MyRaycast(
		btVector3* m_vector, //顶点数组   顶点顺序有要求：第一点和第二点是一组，构成一条线。第三点和第四点是一组，构成一条线。。。。。。。
		int nums_m_vector, //顶点数组长度		必须为偶数
		btCollisionWorld* m_cw//物理世界指针
		)
{
	this->m_vector = m_vector;//记录顶点数组
	this->nums_m_vector = nums_m_vector;//记录顶点数组长度
	this->m_cw = m_cw;//记录物理世界指针
	this->vCountLine = nums_m_vector / 2;//计算光线数量
	this->hit = new btVector3[vCountLine];//创建碰撞接触点数组
	this->normals = new btVector3[vCountLine];//创建法向量数组
	this->reflectedRays = new btVector3[vCountLine];//创建法向量数组

    dl = new DrawLine();//创建光线绘制类对象
}

void MyRaycast::cast()
{
	for (int i = 0; i < vCountLine; i++)//遍历所有的条数
	{
		btCollisionWorld::ClosestRayResultCallback cb(m_vector[i*2], m_vector[i*2+1]);//光线回调类

		m_cw->rayTest (m_vector[i*2], m_vector[i*2+1], cb);//给物理世界设置光线参数

		if (cb.hasHit ())//是否碰撞
		{
			hit[i] = cb.m_hitPointWorld;//获取碰撞接触点
			normals[i] = cb.m_hitNormalWorld;//获取法向量
			normals[i].normalize ();//法向量初始化

			reflectedRays[i] = reflectedRay(m_vector[i*2]-hit[i],normals[i]);
		}
		else
		{
			//若没有碰撞接触
			hit[i] = m_vector[i*2+1];//将碰撞点设置为终点
			normals[i] = btVector3(1.0, 0.0, 0.0);//设置法向量

			reflectedRays[i] =  btVector3(1.0, 0.0, 0.0);//设置法向量
		}
	}
}

btVector3 MyRaycast::reflectedRay(btVector3 n1,btVector3 n2)//n1入射线   n2法向量
{
	n2 = n2.normalized();
	return 2*n1.dot(n2)*n2 - n1;
}

void MyRaycast::drawSelf()//此方法应在cast方法之后调用
{
	for (int i = 0; i < vCountLine; i++)//遍历所有的条数
	{
		dl->drawSelf(//绘制光线
				m_vector[i*2],//第一个顶点
				hit[i]//第二个顶点
	    		);
		dl->drawSelf(//绘制法向量光线
				hit[i],//第一个顶点
				hit[i] + reflectedRays[i].normalize()*5.0f
	    		);
	}

}
