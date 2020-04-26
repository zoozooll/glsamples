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

//MS3D文件头信息
class MS3DHeader{
private:
	string id;//固定的标志字符串
	int version;//版本号

public:
	MS3DHeader();//构造器
	~MS3DHeader();

	string getId();//获取固定的标志字符串的方法
	int getVersion();//获取版本号的方法

};

#endif /* MS3DHEADER_H_ */
