package com.bn.Sample7_8;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import com.bn.Sample7_8.util.MatrixState;
import com.bn.Sample7_8.util.ShaderManager;

import android.opengl.GLES30;


public class TextFloor {
	
	private int muMVPMatrixHandle;
	private int maTexCoorHandle;
	private int maPositionHandle;
	private FloatBuffer mVertexBuffer;
	private FloatBuffer mTextureBuffer;
    
	private int vCount;
	private float yOffset;
	private int mProgram;
    private MySurfaceView mv;

	public TextFloor(int unit_size, float yOffset,
			MySurfaceView gameSurfaceView) {
		this.yOffset=yOffset;
		this.mv = gameSurfaceView;
	initVertexData(unit_size);						//初始化顶点坐标与着色数据
	initShader();								//初始化shader
}
//初始化顶点坐标与着色数据的方法
private void initVertexData(final float unit_size) {
	vCount = 6;
	float vertices[] = new float[]{					//顶点数组
		1 * unit_size, yOffset, 1 * unit_size,
		-1 * unit_size, yOffset, -1 * unit_size, 
		-1 * unit_size,yOffset, 1 * unit_size,
		
		1 * unit_size, yOffset, 1 * unit_size, 
		1 * unit_size, yOffset,-1 * unit_size,
		-1 * unit_size, yOffset, -1 * unit_size, };
	//创建顶点坐标数据缓冲
	ByteBuffer vbb = ByteBuffer.allocateDirect(vertices.length * 4);
	vbb.order(ByteOrder.nativeOrder());				//设置字节顺序
	mVertexBuffer = vbb.asFloatBuffer();				//转换为int型缓冲
	mVertexBuffer.put(vertices);					//向缓冲区中放入顶点坐标数据
	mVertexBuffer.position(0);						//设置缓冲区起始位置
	float textures[] = new float[]{					//纹理数组
		unit_size/2, unit_size/2,
		0,0, 
		0,unit_size/2,
		
		unit_size/2, unit_size/2,
		unit_size/2,0,
		0,0
	};
	//创建顶点纹理数据缓冲
	ByteBuffer tbb =ByteBuffer
	.allocateDirect(textures.length * 4);
	tbb.order(ByteOrder.nativeOrder());				//设置字节顺序
	mTextureBuffer = tbb.asFloatBuffer();			//转换为int型缓冲
	mTextureBuffer.put(textures);					//向缓冲区中放入顶点着色数据
	mTextureBuffer.position(0);					//设置缓冲区起始位置
}
//初始化着色器
private void initShader()
{
	 //加载顶点着色器的脚本内容
	 String mVertexShader=ShaderManager.loadFromAssetsFile("vertex.sh", 
													mv.getResources());
	 //加载片元着色器的脚本内容
	 String mFragmentShader=ShaderManager.loadFromAssetsFile("frag.sh",  
													  mv.getResources());  
	 //基于顶点着色器与片元着色器创建程序
	 mProgram = ShaderManager.createProgram(mVertexShader,mFragmentShader);
	 //获取程序中顶点位置属性引用id
	 maPositionHandle = GLES30.glGetAttribLocation(mProgram, "aPosition");
	 //获取程序中顶点纹理坐标属性引用id
	 maTexCoorHandle=GLES30.glGetAttribLocation(mProgram, "aTexCoor");  
	 //获取程序中总变换矩阵引用id
	 muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");

}
//绘制方法
public void drawSelf(int texId){        
  	 //制定使用某套shader程序 
  	 GLES30.glUseProgram(mProgram);
  	 //将最终变换矩阵传入shader程序 
  	 GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false,
  						   MatrixState.getFinalMatrix(), 0); 

  	 //为画笔指定顶点位置数据
    GLES30.glVertexAttribPointer(maPositionHandle,
      		                  3,GLES30.GL_FLOAT,false, 3*4,mVertexBuffer);       
    //为画笔指定顶点纹理坐标数据
    GLES30.glVertexAttribPointer(maTexCoorHandle,2,
      						GLES30.GL_FLOAT,false, 2*4,mTextureBuffer);   
    //启用顶点位置、纹理坐标数据
    GLES30.glEnableVertexAttribArray(maPositionHandle);  
    GLES30.glEnableVertexAttribArray(maTexCoorHandle);  
    //绑定纹理
    GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
    GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);     

    //绘制纹理矩形
    GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount); 
  }
}