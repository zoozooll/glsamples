/*
 * MS3DHeader.h
 *
 *  Created on: 2015-9-4
 *      Author: Administrator
 */

#ifndef MS3DHEADER_H_
#define MS3DHEADER_H_

#include <string>

using namespace std;

//MS3D�ļ�ͷ��Ϣ
class MS3DHeader{
private:
	string id;//�̶��ı�־�ַ���
	int version;//�汾��

public:
	MS3DHeader();//������
	~MS3DHeader();

	string getId();//��ȡ�̶��ı�־�ַ����ķ���
	int getVersion();//��ȡ�汾�ŵķ���

};

#endif /* MS3DHEADER_H_ */
