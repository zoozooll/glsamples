/*
 * DrawRope.cpp
 *
 *  Created on: 2015年2月4日
 *      Author: Nr
 */

#include "DrawRope.h"

DrawRope::DrawRope(btVector3* pos, int nums,int texId) {
	// TODO Auto-generated constructor stub
	this->m_texId = texId;
	this->m_nums = nums;
	this->m_pos = pos;

	m_bottom_circle = new Circle(
			texId,
			0.1f,//半径
			30,//圆被切割的份数    30
			m_pos,
			nums-1
			);
	m_top_circle = new Circle(
			texId,
			0.1f,//半径
			30,//圆被切割的份数    30
			m_pos,
			0
			);

	csi = new CylinderSideInclined[nums-1];
	for(int i = 0; i < nums-1; i++)
	{
		csi[i] =  CylinderSideInclined(texId,0.1f,30,
					m_pos,i
					);
	}
}

DrawRope::~DrawRope() {
	// TODO Auto-generated destructor stub

}
void DrawRope::drawSelf()
{
	m_top_circle->drawSelf(m_texId);
	m_bottom_circle->drawSelf(m_texId);
	for(int i = 0; i < m_nums-1; i++)
	{
		csi[i].drawSelf(m_texId);
	}
}

