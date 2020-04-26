package com.bn.Sample9_1;
import java.util.ArrayList;
import android.opengl.Matrix;
public class BodyPart{						//身体部件
	LoadedObjectVertexNormalTexture lovnt;	//绘制者
	int index; 								//部件索引 
	MySurfaceView msv;						//对应的GLSurfaceView
	float[] mFather=new float[16];  		//此骨骼在父骨骼坐标系中的实时变换矩阵
	float[] mWorld=new float[16];			//此骨骼在世界坐标系中的实时变换矩阵
    float[] mFatherInit=new float[16];		//此骨骼在父骨骼坐标系中的初始变换矩阵
	float[] mWorldInitInver=new float[16];	//此骨骼初始情况下在世界坐标系中的变换矩阵的逆矩阵
	float[] finalMatrix=new float[16];		//最终变换矩阵
	float fx,fy,fz;//此骨骼不动点在世界坐标系中的原始坐标
    //此骨骼自己的子骨骼列表
    ArrayList<BodyPart> childs=new ArrayList<BodyPart>();
    BodyPart father;//指向父骨骼的引用
    Robot robot;
    //fx,fy,fz为子骨骼不动点在世界坐标系的坐标    
    public BodyPart(float fx,float fy,float fz,LoadedObjectVertexNormalTexture lovnt,int index,MySurfaceView msv,Robot robot){    
    	this.index=index;
    	this.msv=msv;    	
    	this.lovnt=lovnt;
    	this.fx=fx;
    	this.fy=fy;
    	this.fz=fz;    	    
    	this.robot=robot;
    }
     //将最终矩阵内容拷贝进绘制用的最终矩阵
    public void copyMatrixForDraw(){
    	for(int i=0;i<16;i++){
    		
    		robot.fianlMatrixForDrawArray[index][i]=finalMatrix[i];
    	}}
    public void drawSelf(float[][] tempMatrixArray){
    	if(lovnt!=null){
    		MatrixState.pushMatrix();
    		MatrixState.setMatrix(tempMatrixArray[index]);   //插入新矩阵     	
        	lovnt.drawSelf();
        	MatrixState.popMatrix();   
    	}
    	for(BodyPart bp:childs){   	//绘制所有的孩子
    		bp.drawSelf(tempMatrixArray);
    	}}
     //级联计算每个子骨骼在父骨骼坐标系中的原始坐标
    public void initFatherMatrix(){
    	float tx=fx;
    	float ty=fy;
    	float tz=fz;  
    	//若父骨骼不为空，则子骨骼在父骨骼坐标系中的原始坐标
   	     	if(father!=null){
    		tx=fx-father.fx;
    		ty=fy-father.fy;
    		tz=fz-father.fz;
    	}    	
    	//生成初始的此骨骼在父骨骼坐标系中的初始变换矩阵
    	Matrix.setIdentityM(mFather, 0);
    	Matrix.translateM(mFather, 0, tx, ty, tz);   
    	for(int i=0;i<16;i++){
    		mFatherInit[i]=mFather[i];
    	}  	
    		for(BodyPart bc:childs){//然后更新自己的所有孩子
    		bc.initFatherMatrix();
    	}}
      //层次级联计算子骨骼初始情况下在世界坐标系中的变换矩阵的逆矩阵
    public void calMWorldInitInver(){
    	Matrix.invertM(mWorldInitInver, 0, mWorld, 0);
    	for(BodyPart bc:childs){//然后更新自己的所有孩子
    		bc.calMWorldInitInver();
    	}}
    public void updateBone(){//层次级联更新骨骼矩阵的方法
    	//首先根据父矩阵更新自己在世界坐标系中的变换矩阵
    	if(father!=null){
    	 //若父骨骼不为空则此骨骼在世界坐标系中的变换矩阵
          Matrix.multiplyMM(mWorld, 0, father.mWorld, 0, mFather, 0);
    	}
    	else{
    		//若父骨骼为空，则此骨骼在世界坐标系中的变换矩阵为自己在父骨骼坐标系中的变换矩阵
    		for(int i=0;i<16;i++){
    			mWorld[i]=mFather[i];
    		}}
    	calFinalMatrix();    	
    	for(BodyPart bc:childs){//然后更新自己的所有孩子
    		bc.updateBone();
    	}}
     //获取最终的绑定到此骨骼的顶点受此骨骼影响的从Mesh坐标系到骨骼动画后位置的最终变换矩阵
    public void calFinalMatrix(){
    	Matrix.multiplyMM(finalMatrix, 0, mWorld, 0, mWorldInitInver, 0);
    }
    public void backToIInit(){//清除状态
    	for(int i=0;i<16;i++){
    		mFather[i]=mFatherInit[i];
    	}  	
    	for(BodyPart bc:childs){//然后更新自己的所有孩子
    		bc.backToIInit();
    	}} 
    public void transtate(float x,float y,float z){//在父坐标系中平移自己
    	Matrix.translateM(mFather, 0, x, y, z);
    }
    public void rotate(float angle,float x,float y,float z){//在父坐标系中旋转自己
    	Matrix.rotateM(mFather,0,angle,x,y,z);
    }    
    public void addChild(BodyPart child){//添加子骨骼
    	childs.add(child);
    	child.father=this;
    }}