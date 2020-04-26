package com.bn.Sample11_6;//声明包

import static com.bn.Sample11_6.ShaderUtil.createRenderProgram;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import android.opengl.GLES31;

//创建水及绘制水类
public class Water 
{	
	int mProgram;//自定义渲染管线程序id
    int muMVPMatrixHandle;//总变换矩阵引用id
    int muMMatrixHandle;//位置、旋转变换矩阵引用id
    int muLightLocationHandle;//光源位置属性引用id
    int muCameraHandle; //摄像机位置属性引用 id
    int mutexCoorOffsetHandle;//每帧的纹理坐标偏移引用id
    
    int mComputeProgramBD;//自定义渲染管线计算程序id--波动
	int mComputeProgramNormal;//自定义渲染管线计算程序id--法向量
    //第1个正弦波四个参数的引用id
    int muBx1Handle;//第1个正弦波的波心引用id
    int muBc1Handle;//第1个正弦波的波长引用id
    int muzf1Handle;//第1个正弦波的振幅引用id
    int muqsj1Handle;//第1个正弦波的起始角引用id
    
    //第2个正弦波四个参数的引用id
    int muBx2Handle;
    int muBc2Handle;
    int muzf2Handle;
    int muqsj2Handle;
    
    //第3个正弦波四个参数的引用id
    int muBx3Handle;
    int muBc3Handle;
    int muzf3Handle;
    int muqsj3Handle;
    
    int maPositionHandle; 	//顶点位置属性引用id  
    int maNormalHandle; 	//顶点法向量属性引用id  
    int maTexCoorHandle; 	//顶点纹理坐标属性引用id  
    String mVertexShader;	//顶点着色器    	 
    String mFragmentShader;	//片元着色器
    String mComputeShaderBD;//计算着色器--波动
    String mComputeShaderNormal;//计算着色器--法向量
	
	FloatBuffer  mVertexBuffer;//顶点坐标数据缓冲
	FloatBuffer  mNormalBuffer;//顶点法向量数据缓冲
	FloatBuffer  mTexCoorBuffer;//顶点纹理坐标数据缓冲
	IntBuffer    mIndexBuffer;//组装索引缓冲
	
	int vertexDataBufferId;//顶点数据缓冲id
	int normalDataBufferId;//法向量数据缓冲id
	
	//顶点数
    int vCount=0;  
    //索引数
    int iCount=0;
    
    //各个正弦波形的起始角
    float qsj1=0;
    float qsj2=90;
    float qsj3=45;
    
    //每帧的纹理坐标偏移量
    float texCoorOffset=0.0f;
    
    public Water(MySurfaceView mv)
    {    	
    	//初始化顶点坐标与着色数据
    	initVertexData();
    	//初始化着色器        
    	initShader(mv);
    }
    
    //初始化顶点相关数据的方法
    public void initVertexData()
    {
    	//顶点坐标数据的初始化================begin============================
    	vCount=(Constant.WATER_WIDTH+1)*(Constant.WATER_HEIGHT+1);
    	float vertices[]=new float[vCount*4];
    	int tempCount=0;
    	
    	for(int j=0;j<=Constant.WATER_HEIGHT;j++)
    	{
    		for(int i=0;i<=Constant.WATER_WIDTH;i++)
    		{
    			float x=Constant.WATER_UNIT_SIZE*i;
    			float z=Constant.WATER_UNIT_SIZE*j;
    			float y=0;
    			vertices[tempCount*4]=x;
    			vertices[tempCount*4+1]=y;
    			vertices[tempCount*4+2]=z;
    			vertices[tempCount*4+3]=1;
    			tempCount++; 
    		}
    	} 	
    	
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
        
        //顶点法向量数据的初始化================begin============================
        float normals[]=new float[vCount*4];
    	tempCount=0;
    	
    	for(int j=0;j<=Constant.WATER_HEIGHT;j++)
    	{
    		for(int i=0;i<=Constant.WATER_WIDTH;i++)
    		{    			
    			normals[tempCount*4]=0;
    			normals[tempCount*4+1]=1;
    			normals[tempCount*4+2]=0;
    			normals[tempCount*4+3]=1;
    			tempCount++; 
    		}
    	} 	
    	
        //创建顶点坐标数据缓冲
        //vertices.length*4是因为一个整数四个字节
        ByteBuffer nbb = ByteBuffer.allocateDirect(normals.length*4);
        nbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mNormalBuffer = nbb.asFloatBuffer();//转换为Float型缓冲
        mNormalBuffer.put(normals);//向缓冲区中放入顶点坐标数据
        mNormalBuffer.position(0);//设置缓冲区起始位置        
        //顶点法向量数据的初始化================end============================
        
        
        //顶点纹理坐标数据的初始化================begin============================
        float texCoor[]=new float[vCount*2];
        tempCount=0;
        
        for(int j=0;j<=Constant.WATER_HEIGHT;j++)
    	{
        	for(int i=0;i<=Constant.WATER_WIDTH;i++)
    		{
    			float s=(3.0f/Constant.WATER_WIDTH)*i;
    			float t=(3.0f/Constant.WATER_HEIGHT)*j;

    			texCoor[tempCount*2]=s;
    			texCoor[tempCount*2+1]=t;
    			tempCount++;
    		}
    	}
          
        //创建顶点纹理坐标数据缓冲
        ByteBuffer cbb = ByteBuffer.allocateDirect(texCoor.length*4);
        cbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTexCoorBuffer = cbb.asFloatBuffer();//转换为Float型缓冲
        mTexCoorBuffer.put(texCoor);//向缓冲区中放入顶点着色数据
        mTexCoorBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点纹理坐标数据的初始化================end============================
        
        //顶点组装索引数据的初始化================begin============================
        iCount=Constant.WATER_WIDTH*Constant.WATER_HEIGHT*6;
        int[] indexs=new int[iCount];
        tempCount=0;
        for(int i=0;i<Constant.WATER_WIDTH;i++)
    	{
    		for(int j=0;j<Constant.WATER_HEIGHT;j++)
    		{
    			//0---1
    			//| / |
    			//3---2
    			int widthTemp=Constant.WATER_WIDTH+1;
    			int index0=j*widthTemp+i;   
    			int index1=index0+1; 
    			int index2=index0+1+widthTemp;
    			int index3=index0+widthTemp;
    			
    			//0-3-1
    			indexs[tempCount*6]=index0;
    			indexs[tempCount*6+1]=index3;
    			indexs[tempCount*6+2]=index1;
    			
    			//1-3-2
				indexs[tempCount*6+3]=index1;
    			indexs[tempCount*6+4]=index3;
    			indexs[tempCount*6+5]=index2;
    			
    			tempCount++;
    		}
    	}
        
        //创建索引数据缓冲
        ByteBuffer ibb = ByteBuffer.allocateDirect(indexs.length*4);
        ibb.order(ByteOrder.nativeOrder());//设置字节顺序        
        mIndexBuffer=ibb.asIntBuffer();
        mIndexBuffer.put(indexs);
        mIndexBuffer.position(0);

        //生成顶点数据缓冲id与法向量数据缓冲id
        int[] bufferIds=new int[2];//存放顶点数据缓冲id和法向量数据缓冲id的数组      
        GLES31.glGenBuffers(2,bufferIds,0);//生成缓冲id
        vertexDataBufferId=bufferIds[0];//获取顶点数据缓冲id
        normalDataBufferId=bufferIds[1];//获取法向量数据缓冲id
        //绑定顶点数据缓冲
        GLES31.glBindBuffer(GLES31.GL_SHADER_STORAGE_BUFFER, vertexDataBufferId);        
        //为顶点数据缓冲填充数据
        GLES31.glBufferData(GLES31.GL_SHADER_STORAGE_BUFFER, vCount*4*4, mVertexBuffer, GLES31.GL_STATIC_DRAW);
        //绑定法向量数据缓冲
        GLES31.glBindBuffer(GLES31.GL_SHADER_STORAGE_BUFFER, normalDataBufferId);        
        //为法向量数据缓冲填充数据
        GLES31.glBufferData(GLES31.GL_SHADER_STORAGE_BUFFER, vCount*4*4, mNormalBuffer, GLES31.GL_STATIC_DRAW);
    }

    //初始化着色器
    public void initShader(MySurfaceView mv)
    {
    	//加载绘制着色器-------------------------------------------------------------------------
    	//加载顶点着色器的脚本内容
        mVertexShader=ShaderUtil.loadFromAssetsFile("vertex.sh", mv.getResources());
        //加载片元着色器的脚本内容
        mFragmentShader=ShaderUtil.loadFromAssetsFile("frag.sh", mv.getResources());  
        //基于顶点着色器与片元着色器创建程序
        mProgram = createRenderProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用id  
        maPositionHandle = GLES31.glGetAttribLocation(mProgram, "aPosition");
        //获取程序中顶点法向量属性引用id  
        maNormalHandle = GLES31.glGetAttribLocation(mProgram, "aNormal");
        //获取程序中顶点纹理坐标属性引用id  
        maTexCoorHandle= GLES31.glGetAttribLocation(mProgram, "aTexCoor");
        //获取程序中总变换矩阵引用id
        muMVPMatrixHandle = GLES31.glGetUniformLocation(mProgram, "uMVPMatrix");  
        //获取程序中位置、旋转变换矩阵引用id
        muMMatrixHandle = GLES31.glGetUniformLocation(mProgram, "uMMatrix");
        //获取程序中光源位置属性引用id
        muLightLocationHandle = GLES31.glGetUniformLocation(mProgram, "uLightLocation");
        //获取程序中摄像机位置属性引用 id
        muCameraHandle = GLES31.glGetUniformLocation(mProgram, "uCamera"); 
        //获取程序中纹理坐标偏移属性引用 id
        mutexCoorOffsetHandle = GLES31.glGetUniformLocation(mProgram, "utexCoorOffset"); 
        
        //加载计算着色器--波动-------------------------------------------------------------------
        mComputeShaderBD=ShaderUtil.loadFromAssetsFile("computeBD.sh", mv.getResources());
        mComputeProgramBD=ShaderUtil.createComputeProgram(mComputeShaderBD);
        
        muBx1Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "bx1"); 
        muBc1Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "bc1"); 
        muzf1Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "zf1");
        muqsj1Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "qsj1");
        
        muBx2Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "bx2"); 
        muBc2Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "bc2"); 
        muzf2Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "zf2");
        muqsj2Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "qsj2");
        
        muBx3Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "bx3"); 
        muBc3Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "bc3"); 
        muzf3Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "zf3");
        muqsj3Handle=GLES31.glGetUniformLocation(mComputeProgramBD, "qsj3");
        
        //加载计算着色器--法向量--------------------------------------------------------------------
        mComputeShaderNormal=ShaderUtil.loadFromAssetsFile("computeNormal.sh", mv.getResources());
        mComputeProgramNormal=ShaderUtil.createComputeProgram(mComputeShaderNormal);
    }
    //绘制方法     
    public void drawSelf(int texId)
    {        
    	 //计算着色器---计算逆向FFT---------------------------------------begin---------------
    	
    	 //绑定顶点数据缓冲
    	 GLES31.glBindBufferBase(GLES31.GL_SHADER_STORAGE_BUFFER,4,vertexDataBufferId);
    	 //绑定法向量数据缓冲
    	 GLES31.glBindBufferBase(GLES31.GL_SHADER_STORAGE_BUFFER,5,normalDataBufferId);
    	 //使用顶点扰动计算着色器程序
   	     GLES31.glUseProgram(mComputeProgramBD);   
   	     
   	     //将第1个波的波心传入渲染管线
   	     GLES31.glUniform2f(muBx1Handle, 50, 150);
   	     //将第1个波的波长传入渲染管线
   	     GLES31.glUniform1f(muBc1Handle, 32);
   	     //将第1个波的振幅传入渲染管线
   	     GLES31.glUniform1f(muzf1Handle, 0.8f);
   	     //第1个波的起始角
   	     qsj1=(qsj1+9)%360;
   	     //将第1个波的起始角传入渲染管线
   	     GLES31.glUniform1f(muqsj1Handle,(float)Math.toRadians(qsj1));
   	     
   	     //第2个波的波心
   	     GLES31.glUniform2f(muBx2Handle, 10, 40);
   	     //第2个波的波长
   	     GLES31.glUniform1f(muBc2Handle, 24);
   	     //第2个波的振幅
   	     GLES31.glUniform1f(muzf2Handle, 1f);
   	     //第2个波的起始角
   	     qsj2=(qsj2+9)%360;
   	     GLES31.glUniform1f(muqsj2Handle,(float)Math.toRadians(qsj2));
   	     
   	     //第3个波的波心
   	     GLES31.glUniform2f(muBx3Handle, 200, 200);
   	     //第3个波的波长
   	     GLES31.glUniform1f(muBc3Handle, 60);
   	     //第3个波的振幅
   	     GLES31.glUniform1f(muzf3Handle, 2.0f);
   	     //第3个波的起始角
   	     qsj3=(qsj3+4.0f)%360;
   	     GLES31.glUniform1f(muqsj3Handle,(float)Math.toRadians(qsj3));
   	     
   	     //执行逆向FFT计算着色器
   	     GLES31.glDispatchCompute
   	     (
    		(Constant.WATER_WIDTH+1),	//计算工作组X维度尺寸
    		(Constant.WATER_HEIGHT+1),	//计算工作组Y维度尺寸
    		1							//计算工作组Z维度尺寸
   	     );
   	     //同步内存--内存屏障
   	     GLES31.glMemoryBarrier(GLES31.GL_SHADER_STORAGE_BARRIER_BIT);
   	     //计算着色器---计算逆向FFT---------------------------------------end------------------
    	
   	     //计算着色器---计算法向量---------------------------------------begin---------------
   	     //绑定顶点数据缓冲
    	 GLES31.glBindBufferBase(GLES31.GL_SHADER_STORAGE_BUFFER,4,vertexDataBufferId);
    	 //绑定法向量数据缓冲
    	 GLES31.glBindBufferBase(GLES31.GL_SHADER_STORAGE_BUFFER,5,normalDataBufferId);
    	 //使用法向量计算着色器程序
   	     GLES31.glUseProgram(mComputeProgramNormal);   
   	     //执行计算法向量计算着色器   	     
   	     GLES31.glDispatchCompute
	     (
	    	(Constant.WATER_WIDTH+1),	//计算工作组X维度尺寸
	    	(Constant.WATER_HEIGHT+1),	//计算工作组Y维度尺寸
	    	1							//计算工作组Z维度尺寸
	     );
   	     //同步内存
	     GLES31.glMemoryBarrier(GLES31.GL_SHADER_STORAGE_BARRIER_BIT);
   	     //计算着色器---计算法向量---------------------------------------end-----------------
    	
    	 //指定使用某套shader程序
    	 GLES31.glUseProgram(mProgram);  
         //将最终变换矩阵传入shader程序
         GLES31.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0); 
         //将位置、旋转变换矩阵传入着色器程序
         GLES31.glUniformMatrix4fv(muMMatrixHandle, 1, false, MatrixState.getMMatrix(), 0);
         //将光源位置传入着色器程序   
         GLES31.glUniform3fv(muLightLocationHandle, 1, MatrixState.lightPositionFB);
         //将摄像机位置传入着色器程序   
         GLES31.glUniform3fv(muCameraHandle, 1, MatrixState.cameraFB);    
         //将纹理坐标偏移传入着色器程序
         texCoorOffset=(texCoorOffset+0.001f)%10.0f;
         GLES31.glUniform1f(mutexCoorOffsetHandle,texCoorOffset);         
         
         //将顶点坐标数据送入渲染管线
         GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,vertexDataBufferId);
         GLES31.glVertexAttribPointer(maPositionHandle, 4, GLES31.GL_FLOAT, false, 4*4, 0);
         
         //将顶点法向量数据送入渲染管线
         GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,normalDataBufferId);
         GLES31.glVertexAttribPointer(maNormalHandle, 4, GLES31.GL_FLOAT, false, 4*4, 0);
         GLES31.glBindBuffer(GLES31.GL_ARRAY_BUFFER,0);
                
         //将顶点纹理坐标数据送入渲染管线
         GLES31.glVertexAttribPointer  
         (
        		maTexCoorHandle, 
         		2, 
         		GLES31.GL_FLOAT, 
         		false,
                2*4,   
                mTexCoorBuffer
         );
         //允许各个数据数组
         GLES31.glEnableVertexAttribArray(maPositionHandle);  
         GLES31.glEnableVertexAttribArray(maNormalHandle);  
         GLES31.glEnableVertexAttribArray(maTexCoorHandle);  
         
         //绑定纹理
         GLES31.glActiveTexture(GLES31.GL_TEXTURE0);
         GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, texId);

         //执行绘制
         GLES31.glDrawElements(GLES31.GL_TRIANGLES, iCount, GLES31.GL_UNSIGNED_INT, mIndexBuffer);
    }
}