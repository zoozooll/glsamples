package com.bn.Sample12_1;
import java.io.IOException;
import java.io.InputStream;
import android.opengl.GLSurfaceView;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.os.Build;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

class MySurfaceView extends GLSurfaceView
{
    private SceneRenderer mRenderer;//场景渲染器
	
    public static final int drawSample=3;//遮挡查询的数量
    
	//矩形的位置
	static float rectX;
	static float rectY;
	static final float moveSpan = 0.1f;
	public MySurfaceView(Context context) {
        super(context);
        this.setEGLContextClientVersion(3); //设置使用OPENGL ES3.0
        mRenderer = new SceneRenderer();	//创建场景渲染器
        setRenderer(mRenderer);				//设置渲染器		        
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式为主动渲染   
    }
	private class SceneRenderer implements GLSurfaceView.Renderer 
    {
    	//从指定的obj文件中加载对象
		LoadedObjectVertexNormalFace cft;//长方体绘制对象
		LoadedObjectVertexNormalAverage qt;//球体绘制对象
		LoadedObjectVertexNormalAverage ch;//茶壶绘制对象
		
        @SuppressLint("InlinedApi")
		@TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
		public void onDrawFrame(GL10 gl)
        {
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
            
            int[] tia=new int[drawSample];//创建存放指定查询id的数组
            GLES30.glGenQueries(drawSample, tia, 0);//生成指定数量的查询对象
            
            MatrixState.pushMatrix();//保护现场
            MatrixState.scale(1.5f, 1.5f, 1.5f);//缩放物体
            
            MatrixState.pushMatrix();//保护现场
            MatrixState.translate(0, 0, 10f);//平移
            ch.drawSelf();//绘制茶壶
            MatrixState.popMatrix();//恢复现场
            
            GLES30.glBeginQuery(GLES30.GL_ANY_SAMPLES_PASSED, tia[0]);//开启遮挡查询
            MatrixState.pushMatrix();//保护现场
            MatrixState.translate(-10f, 0f, 0);//平移
            cft.drawSelf();//绘制长方体
            MatrixState.popMatrix(); //恢复现场
            GLES30.glEndQuery(GLES30.GL_ANY_SAMPLES_PASSED);//结束遮挡查询
            
            GLES30.glBeginQuery(GLES30.GL_ANY_SAMPLES_PASSED, tia[1]);//开启遮挡查询
            MatrixState.pushMatrix();//保护现场
            MatrixState.translate(0f, 0f, 0);//平移
            qt.drawSelf();//绘制球体
            MatrixState.popMatrix();//恢复现场
            GLES30.glEndQuery(GLES30.GL_ANY_SAMPLES_PASSED);//结束遮挡查询

            GLES30.glBeginQuery(GLES30.GL_ANY_SAMPLES_PASSED, tia[2]);//开启遮挡查询
            MatrixState.pushMatrix();//保护现场
            MatrixState.translate(15f, 0f, 0);//平移
            qt.drawSelf();//绘制球体
            MatrixState.popMatrix();//恢复现场
            GLES30.glEndQuery(GLES30.GL_ANY_SAMPLES_PASSED); //结束遮挡查询
            MatrixState.popMatrix();//恢复现场

        	for(int i=0;i<tia.length;i++)
        	{//循环提取遮挡查询结果
        		int[] result=new int[1];
        		//查询物体是否被遮挡 0-完全被遮挡 1-不完全被遮挡
        		////提取结果，查看物体是否被遮挡
        		GLES30.glGetQueryObjectuiv(tia[i],GLES30.GL_QUERY_RESULT,result,0);
        		System.out.println("i:"+i+" result:"+result[0]);
        	}

            GLES30.glDeleteQueries(drawSample, tia, 0);//删除查询对象
        }
        @SuppressLint("NewApi")
		public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 2, 100);
            //设置camera位置
            MatrixState.setCamera
            (
            		0,//人眼位置的X
            		0,//人眼位置的Y
            		50,//人眼位置的Z
            		0,//人眼球看的点X
            		0,//人眼球看的点Y
            		0,//人眼球看的点Z
            		0,//up位置
            		1, 
            		0
            );
            //初始化光源位置
            MatrixState.setLightLocation(100, 100, 100);
        }
        @SuppressLint("NewApi")
		@Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //设置屏幕背景色RGBA
            GLES30.glClearColor(0.3f,0.3f,0.3f,1.0f);    
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            //打开背面剪裁   
            GLES30.glEnable(GLES30.GL_CULL_FACE);
            //初始化变换矩阵
            MatrixState.setInitStack();
            //加载要绘制的物体
            ch=LoadUtil.loadFromFileVertexOnlyAverage("ch.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
    		cft=LoadUtil.loadFromFileVertexOnlyFace("cft.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
    		qt=LoadUtil.loadFromFileVertexOnlyAverage("qt.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
        }
    }

	
	@SuppressLint("NewApi")
	public int initTexture(int drawableId)//textureId
	{
		//生成纹理ID
		int[] textures = new int[1];
		GLES30.glGenTextures
		(
				1,          //产生的纹理id的数量
				textures,   //纹理id的数组
				0           //偏移量
		);    
		int textureId=textures[0];    
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_CLAMP_TO_EDGE);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_CLAMP_TO_EDGE);
        
        //通过输入流加载图片===============begin===================
        InputStream is = this.getResources().openRawResource(drawableId);
        Bitmap bitmapTmp;
        try 
        {
        	bitmapTmp = BitmapFactory.decodeStream(is);
        } 
        finally
        {
            try
            {
                is.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }
        //通过输入流加载图片===============end=====================  
        
        //实际加载纹理
        GLUtils.texImage2D
        (
        		GLES30.GL_TEXTURE_2D,	//纹理类型，在OpenGL ES中必须为GLES30.GL_TEXTURE_2D
        		0,						//纹理的层次，0表示基本图像层，可以理解为直接贴图
        		bitmapTmp,				//纹理图像
        		0						//纹理边框尺寸
        );
        bitmapTmp.recycle();			//纹理加载成功后释放图片
        
        return textureId;
	}
}