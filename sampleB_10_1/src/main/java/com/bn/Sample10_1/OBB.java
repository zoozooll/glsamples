package com.bn.Sample10_1;//声明包

//物体的OBB包围盒
public class OBB
{
	Vector3f min;//OBB包围盒在x轴、y轴、z轴上的最小值x、y、z值
	Vector3f max;//OBB包围盒在x轴、y轴、z轴上的最大值x、y、z值
	
	//参数为顶点数组的构造器
	OBB(float[] vertices){
		min = new Vector3f();//X、Y、Z最小值向量
		max = new Vector3f();//X、Y、Z最大值向量
		empty();//清空OBB
		for(int i=0; i<vertices.length; i+=3){//循环遍历所有顶点
			this.add(vertices[i], vertices[i+1], vertices[i+2]);//将所有的点加入OBB包围盒
		}
	}
	//清空OBB
	public void empty(){
		min.x = min.y = min.z = Float.POSITIVE_INFINITY;//将最小值初始化为Float最大值
		max.x = max.y = max.z = Float.NEGATIVE_INFINITY;//将最大值初始化为Float最小值
	}

	//将所有的点加入OBB包围盒
	public void add(float x, float y, float z){
		if (x < min.x) { min.x = x; }//更新x轴最小值
		if (x > max.x) { max.x = x; }//更新x轴最大值
		if (y < min.y) { min.y = y; }//更新y轴最小值
		if (y > max.y) { max.y = y; }//更新y轴最大值
		if (z < min.z) { min.z = z; }//更新z轴最小值
		if (z > max.z) { max.z = z; }//更新z轴最大值
	}
	/*
	 * Woo提出的方法，先判断矩形边界框的哪个面会相交，
	 * 再检测射线与包含这个面的平面的相交性。
	 * 如果交点在盒子中，那么射线与矩形边界框相交，
	 * 否则不存在相交 
	 */
	//和参数射线的相交性测试，如果不相交则返回值是一个非常大的数(大于1)
	//如果相交，返回相交时间t
	//t为0-1之间的值
	public float rayIntersect(
			Vector3f rayStart,//射线起点
			Vector3f rayEnd,//射线终点
			Vector3f rayDir
			){
		//如果未相交则返回这个大数
		final float kNoIntersection = Float.POSITIVE_INFINITY;
		//检查点在矩形边界内的情况，并计算到每个面的距离
		float xt;
		
		if(rayStart.x<min.x){
			xt = min.x - rayStart.x;
			if(xt>rayDir.x){ return kNoIntersection; }
			xt /= rayDir.x;
		}
		else if(rayStart.x>max.x){
			xt = max.x - rayStart.x;
			if(xt<rayDir.x){ return kNoIntersection; }
			xt /= rayDir.x;
		}
		else{
			xt = -1.0f;
		}
		
		float yt;
		if(rayStart.y<min.y){
			yt = min.y - rayStart.y;
			if(yt>rayDir.y){ return kNoIntersection; }
			yt /= rayDir.y;
		}
		else if(rayStart.y>max.y){
			yt = max.y - rayStart.y;
			if(yt<rayDir.y){ return kNoIntersection; }
			yt /= rayDir.y;
		}
		else{
			yt = -1.0f;
		}
		
		float zt;
		if(rayStart.z<min.z){
			zt = min.z - rayStart.z;
			if(zt>rayDir.z){ return kNoIntersection; }
			zt /= rayDir.z;
		}
		else if(rayStart.z>max.z){
			zt = max.z - rayStart.z;
			if(zt<rayDir.z){ return kNoIntersection; }
			zt /= rayDir.z;
		}
		else{
			zt = -1.0f;
		}
		
		//选择最远的平面――――发生相交的地方
		int which = 0;
		float t = xt;
		if(yt>t){
			which = 1;
			t=yt;
		}
		if(zt>t){
			which = 2;
			t=zt;
		}
		switch(which){
			case 0://和yz平面相交
			{
				float y=rayStart.y+rayDir.y*t;
				if(y<min.y||y>max.y){return kNoIntersection;}
				float z=rayStart.z+rayDir.z*t;
				if(z<min.z||z>max.z){return kNoIntersection;}
			}
			break;
			case 1://和xz平面相交
			{
				float x=rayStart.x+rayDir.x*t;
				if(x<min.x||x>max.x){return kNoIntersection;}
				float z=rayStart.z+rayDir.z*t;
				if(z<min.z||z>max.z){return kNoIntersection;}
			}
			break;
			case 2://和xy平面相交
			{
				float x=rayStart.x+rayDir.x*t;
				if(x<min.x||x>max.x){return kNoIntersection;}
				float y=rayStart.y+rayDir.y*t;
				if(y<min.y||y>max.y){return kNoIntersection;}
			}
			break;
		}
		return t;//返回相交点参数值
	}
}