/*
 * MS3DGroup.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DGROUP_H_
#define MS3DGROUP_H_

//����Ϣ
class MS3DGroup{
private:
	int* indicies;		//���ڵ������εĶ�Ӧ����
	int materialIndex;	//��������
	int count_ind;//��������������
public:
	MS3DGroup();
	~MS3DGroup();

	int* getIndicies();
	int getMaterialIndex();
	int getCount_ind();
};

#endif /* MS3DGROUP_H_ */
