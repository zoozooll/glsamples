package com.bn.Sample10_1;//声明包
/*
 * 可以被触控到的抽象类，
 * 物体继承了该类可以被触控到
 */
public abstract class TouchableObject {
	OBB preBox;//物体OBB包围盒
    float[] m = new float[16];//基本变换矩阵  
	float[] color=new float[]{1,1,1,1};//顶点颜色
	float size = 1.5f;//当前物体的放大率（用于控制选中时放大显示）
	//获得物体OBB包围盒的方法
    public OBB getCurrBox(){
    	return preBox;//获取物体包围盒
    
    }
    //根据选中状态改变物体颜色、尺寸的方法
	public void changeOnTouch(boolean flag){
		if (flag) {//若选中
			color = new float[] { 0, 1, 0, 1 };//改变颜色为绿色
			size = 3f;//改变尺寸
		} else {//若没有选中
			color = new float[] { 1, 1, 1, 1 };	//恢复颜色为白色
			size = 1.5f;//恢复尺寸
		}	
	}
	//复制变换矩阵
    public void copyM(){
    	for(int i=0;i<16;i++){
    		m[i]=MatrixState.getMMatrix()[i];
    	}
    }	
}