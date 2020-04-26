package com.bn.Sample12_2;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import android.opengl.GLES11Ext;
import android.opengl.GLSurfaceView;
import android.opengl.GLES30;
import android.opengl.GLUtils;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.bn.Sample12_2.R;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.hardware.Camera;
import android.hardware.Camera.Size;

class MySurfaceView extends GLSurfaceView 
{
    private SceneRenderer mRenderer;//场景渲染器
    int textureIdCamera;//系统分配的纹理id
    int textureId;
    SurfaceTexture st;//摄像头纹理辅助工具，作用是从一个图像流中捕获图像帧作为OpenGL ES纹理
    Object lock=new Object();//同步锁
    boolean canUpdate=false;//摄像头纹理是否需要更新
    Camera gCamera;
    float angle=0;
	
	public MySurfaceView(Context context) 
	{
        super(context);
        this.setEGLContextClientVersion(3); //设置使用OPENGL ES3.0
        mRenderer = new SceneRenderer();	//创建场景渲染器
        setRenderer(mRenderer);				//设置渲染器
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式为主动渲染
    }
	
	private class SceneRenderer implements GLSurfaceView.Renderer
    {
    	CameraBackDrawer backDrawer;//绘制摄像头实时画面的类
    	LoadedObjectVertexNormalTexture lovo;//茶壶绘制对象
    	
        public void onDrawFrame(GL10 gl)
        {
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
            
            synchronized(lock)
            {
            	if(canUpdate)
            	{
            		/*updateTexImage方法可以得到帧信息的回调，可以在任何线程被调用，
            		 * 因此在没有做必要的保护的情况下，
            		 * updateTexImage()不应该直接从回调函数中调用。
            		 */
            		st.updateTexImage(); //更新纹理图像为从图像流中提取的最近一帧。
            		canUpdate=false;
            	}
            }
            //绘制摄像头实时背景
            MatrixState.pushMatrix();
            MatrixState.translate(0,0,-20);
            backDrawer.drawSelf(textureIdCamera);  
            MatrixState.popMatrix();
            
            //绘制茶壶
            MatrixState.pushMatrix();
            MatrixState.translate(0, -0.2f, 0);
            MatrixState.scale(0.02f, 0.02f, 0.02f);
            MatrixState.rotate(18, 1, 0, 0);
            MatrixState.rotate(angle, 0, 1, 0);
            lovo.drawSelf(textureId);
            MatrixState.popMatrix();
            angle=angle+0.5f;
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height);
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectOrtho(-ratio, ratio, -1, 1, 20, 1000);
            //调用此方法产生摄像机9参数位置矩阵
            MatrixState.setCamera(0,0,30,0f,0f,0f,0f,1.0f,0.0f);
            //初始化变换矩阵
            MatrixState.setInitStack();
            //初始化光源位置
            MatrixState.setLightLocation(-40, 10, 20);
            //初始化摄像头并开始预览
            initCamera(width,height);
            //创建摄像机内容绘制者对对象 
            backDrawer=new CameraBackDrawer(MySurfaceView.this,ratio,1);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config)
        {
        	//打开背面剪裁   
            GLES30.glEnable(GLES30.GL_CULL_FACE);
            //设置屏幕背景色RGBA
            GLES30.glClearColor(0.5f,0.5f,0.5f, 1.0f); 
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            //初始化摄像头专用纹理
            textureIdCamera=initSurfaceTexture();
            //初始化摄像头纹理工具
            st=genSurfaceTexture(textureIdCamera);
            //加载纹理
            textureId=initTexture(R.drawable.ghxp);
            //加载要绘制的物体
            lovo=LoadUtil.loadFromFile("ch_t.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
        }
    }
	
 	public int initTexture(int drawableId)
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
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_REPEAT);
		GLES30.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_REPEAT);
        
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
	   	GLUtils.texImage2D
	    (
	    		GLES30.GL_TEXTURE_2D, //纹理类型
	     		0, 
	     		GLUtils.getInternalFormat(bitmapTmp), 
	     		bitmapTmp, //纹理图像
	     		GLUtils.getType(bitmapTmp), 
	     		0 //纹理边框尺寸
	     );
	    bitmapTmp.recycle(); 		  //纹理加载成功后释放图片
        return textureId;
	}
	
  	public int initSurfaceTexture()
  	{
  		//生成纹理id
		int[] textures = new int[1];
		GLES30.glGenTextures
		(
				1,          //产生的纹理id的数量
				textures,   //纹理id的数组
				0           //偏移量
		);
		int textureId=textures[0];
		GLES30.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureId);
		GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES30.GL_TEXTURE_MIN_FILTER,GLES30.GL_NEAREST);
		GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,GLES30.GL_TEXTURE_MAG_FILTER,GLES30.GL_LINEAR);
		GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES30.GL_TEXTURE_WRAP_S,GLES30.GL_REPEAT);
		GLES30.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES30.GL_TEXTURE_WRAP_T,GLES30.GL_REPEAT);
		
		return textureId;
  	}

  	public SurfaceTexture genSurfaceTexture(int texId)
  	{
  		SurfaceTexture st=new SurfaceTexture(texId);  
  		st.setDefaultBufferSize(512, 512);//设置默认的图像缓冲区大小
  		st.setOnFrameAvailableListener
  		(//注册一个回调函数，当新一帧图像对SurfaceTexture可用时调用
  		 new OnFrameAvailableListener()
  		 {
			@Override
			public void onFrameAvailable(SurfaceTexture surfaceTexture) 
			{
				synchronized(lock)
				{
					canUpdate=true;
				}
			}  			 
  		 }
  		);
  		return st;
  	}

  	public void initCamera(int screenWidth,int screenHeight)
  	{
    	if(gCamera!=null)
    	{
    		gCamera.release();//释放照相机对象	
    		gCamera=null;//清空照相机引用	
    	}
	
    	gCamera = Camera.open(Camera.CameraInfo.CAMERA_FACING_BACK);		//打开照相机

    	try 
        {
			gCamera.setPreviewTexture(st);
		}
        catch (IOException e)
		{
			e.printStackTrace();
		}
    	if(gCamera != null)
    	{
    		//调用Camera的getParameters方法获取拍照参数
    	   	Camera.Parameters parameters = gCamera.getParameters();
    	   	//获取可支持的预览图片的大小
        	List<Size> preSize = parameters.getSupportedPreviewSizes();
        	int previewWidth = preSize.get(0).width;//获取宽度
        	int previewHeight = preSize.get(0).height;//获取高度
        	for (int i = 1; i < preSize.size(); i++) {
        		double similarity = Math.abs(((double) preSize.get(i).height / screenHeight)-
        				((double) preSize.get(i).width / screenWidth));
        	 	if (similarity < Math.abs(((double) previewHeight / screenHeight)- ((double) previewWidth / screenWidth))) {
        	 			previewWidth = preSize.get(i).width;
        	 			previewHeight = preSize.get(i).height;
        	 	}
        	}
        	parameters.setPreviewSize(previewWidth, previewHeight); //设置预览大小
        	parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);//设置焦点模式为连续拍摄
        	gCamera.setParameters(parameters);      //将参数设置入Camera
            gCamera.startPreview();		//开始预览
    	}
  	}
}