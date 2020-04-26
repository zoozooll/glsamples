package com.bn.Sample11_3;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.GLES30;

//纹理三角形
public class GrassObject 
{	
	int mProgram;//自定义渲染管线程序id
    int muVPMatrixHandle;//摄像机、投影组合矩阵引用id
    int muMMatrixHandle;//基本变换矩阵引用id
    int maPositionHandle; //顶点位置属性引用id  
    int maTexCoorHandle; //顶点纹理坐标属性引用id  
    String mVertexShader;//顶点着色器    	 
    String mFragmentShader;//片元着色器
	
	FloatBuffer   mVertexBuffer;//顶点坐标数据缓冲
	FloatBuffer   mTexCoorBuffer;//顶点纹理坐标数据缓冲
    int vCount=0;
    int uTexHandle;//小草纹理属性引用id  
    int uRDTexHandle;//扰动纹理属性引用id
    int uJBTexHandle;//颜色渐变纹理引用id
    int uTotalHandle;//总的小草棵数引用id
    
   
    public GrassObject(MySurfaceView mv,float[] vertices,float texCoors[])
    {
    	//初始化顶点数据的方法
    	initVertexData(vertices,texCoors);
    	//初始化着色器      
    	initShader(mv);
    }
    
    //初始化顶点数据的方法
    public void initVertexData(float[] vertices,float texCoors[])
    {
    	//顶点坐标数据的初始化================begin============================
    	vCount=vertices.length/3;   
		
        //创建顶点坐标数据缓冲
        //vertices.length*4是因为一个整数四个字节
        ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer.put(vertices);//向缓冲区中放入顶点坐标数据
        mVertexBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点坐标数据的初始化================end============================
        
        //顶点纹理坐标数据的初始化================begin============================  
        ByteBuffer tbb = ByteBuffer.allocateDirect(texCoors.length*4);
        tbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTexCoorBuffer = tbb.asFloatBuffer();//转换为Float型缓冲
        mTexCoorBuffer.put(texCoors);//向缓冲区中放入顶点纹理坐标数据
        mTexCoorBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点纹理坐标数据的初始化================end============================
    }

    //初始化着色器
    public void initShader(MySurfaceView mv)
    {
    	//加载顶点着色器的脚本内容
        mVertexShader=ShaderUtil.loadFromAssetsFile("vertex.sh", mv.getResources());
        //加载片元着色器的脚本内容
        mFragmentShader=ShaderUtil.loadFromAssetsFile("frag.sh", mv.getResources());  
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用id  
        maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点纹理坐标属性引用id  
        maTexCoorHandle= GLES30.glGetAttribLocation(mProgram, "aTexCoor");
        //获取程序中摄像机、投影组合矩阵引用id
        muVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uVPMatrix");  
        //获取程序中基本变换矩阵引用id
        muMMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMMatrix"); 
        //获取小草、扰动两个纹理引用id
        uTexHandle=GLES30.glGetUniformLocation(mProgram, "ssTexture");  
        uRDTexHandle=GLES30.glGetUniformLocation(mProgram, "sTexture");
        //获取程序中总的小草棵数的id
        uTotalHandle=GLES30.glGetUniformLocation(mProgram, "totalNum");
        //颜色渐变纹理引用id
        uJBTexHandle=GLES30.glGetUniformLocation(mProgram, "jbTexture");
    }
    
    public void drawSelf(int texId,int texRDId,int jbTexId,int totalNum)
    {        
    	 //指定使用某套着色程序
    	 GLES30.glUseProgram(mProgram);        
    	 //初始化变换矩阵
    	 MatrixState.setInitStack();
         //将摄像机、投影组合矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muVPMatrixHandle, 1, false, MatrixState.getVPMatrix(), 0); 
         //将基本变换矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState.getmMatrix(), 0); 
         //将顶点位置数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
         		maPositionHandle,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer
         );
         //将顶点纹理坐标数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
        		maTexCoorHandle, 
         		2, 
         		GLES30.GL_FLOAT, 
         		false,
                2*4,   
                mTexCoorBuffer
         );
         //启用顶点位置、纹理坐标数据数组
         GLES30.glEnableVertexAttribArray(maPositionHandle);  
         GLES30.glEnableVertexAttribArray(maTexCoorHandle);  
         
         //绑定小草纹理
         GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);
         //绑定扰动纹理
         GLES30.glActiveTexture(GLES30.GL_TEXTURE1);
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texRDId);
         //绑定颜色渐变纹理
         GLES30.glActiveTexture(GLES30.GL_TEXTURE2);
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, jbTexId);
         
         GLES30.glUniform1i(uTexHandle, 0);//通过引用指定小草纹理
         GLES30.glUniform1i(uRDTexHandle, 1);   //通过引用指定扰动纹理
         GLES30.glUniform1i(uJBTexHandle, 2);   //通过引用指定颜色纹理
         //小草的总棵数
         GLES30.glUniform1f(uTotalHandle, totalNum);
         //进行实例渲染
         GLES30.glDrawArraysInstanced(GLES30.GL_TRIANGLES, 0, vCount,totalNum);
    }
}