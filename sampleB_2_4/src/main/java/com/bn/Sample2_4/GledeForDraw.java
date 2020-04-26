package com.bn.Sample2_4;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import android.opengl.GLES30;

//加载后的物体――仅携带顶点信息，颜色随机
public class GledeForDraw
{	
	int mProgram;//自定义渲染管线着色器程序id  
    int muMVPMatrixHandle;//总变换矩阵引用
    int maPositionHandle1; //顶点位置属性引用  
    int maPositionHandle2; //顶点位置属性引用  
    int maPositionHandle3; //顶点位置属性引用  
    int maTexCoorHandle; //顶点纹理坐标属性引用  
    int muBfbHandle;//变化百分比引用
    String mVertexShader;//顶点着色器代码脚本    	 
    String mFragmentShader;//片元着色器代码脚本   
	
	FloatBuffer   mVertexBuffer1;//顶点坐标数据缓冲  
	FloatBuffer   mVertexBuffer2;//顶点坐标数据缓冲  
	FloatBuffer   mVertexBuffer3;//顶点坐标数据缓冲  
	FloatBuffer   mTexCoorBuffer;//顶点纹理坐标数据缓冲
	
	float[][] glede_one;
	float[] glede_two;
	float[] glede_three;
	
    int vCount=0;  
    
    int operator=1;
    float span=0.15f;
    float bfbCurr=0f;
    
    public GledeForDraw(MySurfaceView mv)
    {    	
    	//初始化顶点数据
    	initVertexData(mv);
    	//初始化着色器        
    	intShader(mv);
    	new Thread()
    	{
    		@Override
    		public void run()
    		{
    			while(true)
    			{
    				bfbCurr=bfbCurr+operator*span;
    				if(bfbCurr>2.0f)
    				{
    					bfbCurr=2.0f;
    					operator=-operator;
    				}
    				else if(bfbCurr<0)
    				{
    					bfbCurr=0;
    					operator=-operator;
    				}
    				try
    				{
    					Thread.sleep(50);
    				}
    				catch(Exception e)
    				{
    					e.printStackTrace();
    				}
    			}
    		}
    	}.start();
    }
    
    //初始化顶点数据的方法
    public void initVertexData(MySurfaceView mv)
    {
    	//加载雄鹰模型
    	glede_one=LoadUtil.loadFromFileVertexOnly("laoying01.obj",mv);
    	glede_two=LoadUtil.loadFromFileVertexOnly("laoying02.obj",mv)[0]; 
    	glede_three=LoadUtil.loadFromFileVertexOnly("laoying03.obj",mv)[0]; 
    	
    	//========================1=========================================
    	//创建第一个顶点坐标数据缓冲
    	vCount=glede_one.length/3;
        //vertices.length*4是因为一个整数四个字节
        ByteBuffer vbb = ByteBuffer.allocateDirect(glede_one[0].length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer1 = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer1.put(glede_one[0]);//向缓冲区中放入顶点坐标数据
        mVertexBuffer1.position(0);//设置缓冲区起始位置
        //====================2==========================
        //创建第二个顶点坐标数据缓冲
        vCount=glede_two.length/3;
        //vertices.length*4是因为一个整数四个字节
        vbb = ByteBuffer.allocateDirect(glede_two.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer2 = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer2.put(glede_two);//向缓冲区中放入顶点坐标数据
        mVertexBuffer2.position(0);//设置缓冲区起始位置
        //---------------------------------3-----------------------------
        //创建第三个顶点坐标数据缓冲
        vCount=glede_three.length/3;
        //vertices.length*4是因为一个整数四个字节
        vbb = ByteBuffer.allocateDirect(glede_three.length*4);
        vbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mVertexBuffer3 = vbb.asFloatBuffer();//转换为Float型缓冲
        mVertexBuffer3.put(glede_three);//向缓冲区中放入顶点坐标数据
        mVertexBuffer3.position(0);//设置缓冲区起始位置
        
        //------------------纹理-----------------------------------------
        //创建纹理坐标数据缓冲
        ByteBuffer tbb = ByteBuffer.allocateDirect(glede_one[1].length*4);
        tbb.order(ByteOrder.nativeOrder());//设置字节顺序
        mTexCoorBuffer = tbb.asFloatBuffer();//转换为Float型缓冲
        mTexCoorBuffer.put(glede_one[1]);//向缓冲区中放入顶点纹理坐标数据
        mTexCoorBuffer.position(0);//设置缓冲区起始位置
        //特别提示：由于不同平台字节顺序不同数据单元不是字节的一定要经过ByteBuffer
        //转换，关键是要通过ByteOrder设置nativeOrder()，否则有可能会出问题
        //顶点纹理坐标数据的初始化================end============================
    }
    //初始化着色器
    public void intShader(MySurfaceView mv)
    {
    	//加载顶点着色器的脚本内容
        mVertexShader=ShaderUtil.loadFromAssetsFile("vertex.sh", mv.getResources());
        //加载片元着色器的脚本内容
        mFragmentShader=ShaderUtil.loadFromAssetsFile("frag.sh", mv.getResources());  
        //基于顶点着色器与片元着色器创建程序
        mProgram = ShaderUtil.createProgram(mVertexShader, mFragmentShader);
        //获取程序中顶点位置属性引用  
        maPositionHandle1 = GLES30.glGetAttribLocation(mProgram, "aPosition");
        maPositionHandle2 = GLES30.glGetAttribLocation(mProgram, "bPosition");
        maPositionHandle3 = GLES30.glGetAttribLocation(mProgram, "cPosition");
        //获取程序中总变换矩阵引用
        muMVPMatrixHandle = GLES30.glGetUniformLocation(mProgram, "uMVPMatrix");  
        //获取程序中顶点纹理坐标属性引用  
        maTexCoorHandle= GLES30.glGetAttribLocation(mProgram, "aTexCoor"); 
        //变化百分比引用
        muBfbHandle= GLES30.glGetUniformLocation(mProgram, "uBfb");
    }
    
    public void drawSelf(int texId)
    { 
    	 //指定使用某套着色器程序
    	 GLES30.glUseProgram(mProgram);
         //将最终变换矩阵传入渲染管线
         GLES30.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, MatrixState.getFinalMatrix(), 0); 
         //将变化百分比传入渲染管线
         GLES30.glUniform1f(muBfbHandle, bfbCurr);
         //将顶点位置数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
         		maPositionHandle1,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer1
         );    
         //将顶点位置数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
         		maPositionHandle2,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer2
         ); 
         //将顶点位置数据传入渲染管线
         GLES30.glVertexAttribPointer  
         (
         		maPositionHandle3,   
         		3, 
         		GLES30.GL_FLOAT, 
         		false,
                3*4,   
                mVertexBuffer3
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
         GLES30.glEnableVertexAttribArray(maPositionHandle1);  
         GLES30.glEnableVertexAttribArray(maPositionHandle2);
         GLES30.glEnableVertexAttribArray(maPositionHandle3);
         GLES30.glEnableVertexAttribArray(maTexCoorHandle); 
         //绑定纹理
         GLES30.glActiveTexture(GLES30.GL_TEXTURE0);
         GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texId);
         //绘制加载的物体
         GLES30.glDrawArrays(GLES30.GL_TRIANGLES, 0, vCount); 
    }
}