#include "MyRaycast.h"

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

MyRaycast::MyRaycast(
		btVector3* m_vector, //��������   ����˳����Ҫ�󣺵�һ��͵ڶ�����һ�飬����һ���ߡ�������͵��ĵ���һ�飬����һ���ߡ�������������
		int nums_m_vector, //�������鳤��		����Ϊż��
		btCollisionWorld* m_cw//��������ָ��
		)
{
	this->m_vector = m_vector;//��¼��������
	this->nums_m_vector = nums_m_vector;//��¼�������鳤��
	this->m_cw = m_cw;//��¼��������ָ��
	this->vCountLine = nums_m_vector / 2;//�����������
	this->hit = new btVector3[vCountLine];//������ײ�Ӵ�������
	this->normals = new btVector3[vCountLine];//��������������
	this->reflectedRays = new btVector3[vCountLine];//��������������

    dl = new DrawLine();//�������߻��������
}

void MyRaycast::cast()
{
	for (int i = 0; i < vCountLine; i++)//�������е�����
	{
		btCollisionWorld::ClosestRayResultCallback cb(m_vector[i*2], m_vector[i*2+1]);//���߻ص���

		m_cw->rayTest (m_vector[i*2], m_vector[i*2+1], cb);//�������������ù��߲���

		if (cb.hasHit ())//�Ƿ���ײ
		{
			hit[i] = cb.m_hitPointWorld;//��ȡ��ײ�Ӵ���
			normals[i] = cb.m_hitNormalWorld;//��ȡ������
			normals[i].normalize ();//��������ʼ��

			reflectedRays[i] = reflectedRay(m_vector[i*2]-hit[i],normals[i]);
		}
		else
		{
			//��û����ײ�Ӵ�
			hit[i] = m_vector[i*2+1];//����ײ������Ϊ�յ�
			normals[i] = btVector3(1.0, 0.0, 0.0);//���÷�����

			reflectedRays[i] =  btVector3(1.0, 0.0, 0.0);//���÷�����
		}
	}
}

btVector3 MyRaycast::reflectedRay(btVector3 n1,btVector3 n2)//n1������   n2������
{
	n2 = n2.normalized();
	return 2*n1.dot(n2)*n2 - n1;
}

void MyRaycast::drawSelf()//�˷���Ӧ��cast����֮�����
{
	for (int i = 0; i < vCountLine; i++)//�������е�����
	{
		dl->drawSelf(//���ƹ���
				m_vector[i*2],//��һ������
				hit[i]//�ڶ�������
	    		);
		dl->drawSelf(//���Ʒ���������
				hit[i],//��һ������
				hit[i] + reflectedRays[i].normalize()*5.0f
	    		);
	}

}
