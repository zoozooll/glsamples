/*
 * MS3DGroup.h
 *
 *  Created on: 2015-9-5
 *      Author: Administrator
 */

#ifndef MS3DGROUP_H_
#define MS3DGROUP_H_

//组信息
class MS3DGroup{
private:
	int* indicies;		//组内的三角形的对应索引
	int materialIndex;	//材质索引
	int count_ind;//组内三角形数量
public:
	MS3DGroup();
	~MS3DGroup();

	int* getIndicies();
	int getMaterialIndex();
	int getCount_ind();
};

#endif /* MS3DGROUP_H_ */
