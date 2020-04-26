package com.bn.Sample11_6a;
import java.io.IOException;
import java.io.InputStream;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.view.MotionEvent;
import android.opengl.GLES31;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

class MySurfaceView extends GLSurfaceView 
{
	private final float TOUCH_SCALE_FACTOR = 180.0f/320;//角度缩放比例
    private SceneRenderer mRenderer;//场景渲染器
	
	private float mPreviousY;//上次的触控位置Y坐标
    private float mPreviousX;//上次的触控位置X坐标
    
    int textureId;//系统分配的纹理id
	
	public MySurfaceView(Context context){
        super(context);
        this.setEGLContextClientVersion(3); //设置使用OPENGL ES3.1
        mRenderer = new SceneRenderer();	//创建场景渲染器
        setRenderer(mRenderer);				//设置渲染器		        
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式为主动渲染   
    }
	
	//触摸事件回调方法
    @SuppressLint("ClickableViewAccessibility")
	@Override 
    public boolean onTouchEvent(MotionEvent e) 
    {
        float y = e.getY();
        float x = e.getX();
        switch (e.getAction()) {
        case MotionEvent.ACTION_MOVE:
            float dy = y - mPreviousY;//计算触控笔Y位移
            float dx = x - mPreviousX;//计算触控笔X位移
            //根据触控修改摄像机朝向
            Constant.direction += dx * TOUCH_SCALE_FACTOR;
            Constant.direction=Constant.direction%360;
            //根据触控修改摄像机仰角
            Constant.yj+=dy;
            if(Constant.yj>90)
            {
            	Constant.yj=90;
            }
            if(Constant.yj<20)
            {
            	Constant.yj=20;
            }
            //计算摄像机9参数
            Constant.calCamera();            
        }
        mPreviousY = y;//记录触控位置
        mPreviousX = x;//记录触控位置
        return true;
    }

	private class SceneRenderer implements GLSurfaceView.Renderer 
    {   
    	Water water;//纹理矩形
    	
    	long start=System.nanoTime();
    	int count=0;
        public void onDrawFrame(GL10 gl) 
        { 
        	//计算FPS=========================================================
        	count++;
        	if(count==150)
        	{
        		count=0;
        		long end=System.nanoTime();
        		System.out.println("FPS="+(1000000000.0*150/(end-start)));
        		start=end;
        	}
        	//计算FPS=========================================================
        	
        	//清除深度缓冲与颜色缓冲
            GLES31.glClear( GLES31.GL_DEPTH_BUFFER_BIT | GLES31.GL_COLOR_BUFFER_BIT);
            
          //调用此方法产生摄像机9参数位置矩阵
            MatrixState.setCamera
            (
            	Constant.cx,Constant.cy,Constant.cz,
            	Constant.tx,Constant.ty,Constant.tz,
            	Constant.upx,Constant.upy,Constant.upz
            );
            
            //绘制纹理矩形
            water.drawSelf(textureId);             
        }  

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES31.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 1, 1000);
            //初始化变换矩阵
            MatrixState.setInitStack();
            //初始化灯光
            MatrixState.setLightLocation(400, 400, 400); 
            //初始化摄像机
            Constant.calCamera();
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //设置屏幕背景色RGBA
            GLES31.glClearColor(0.5f,0.5f,0.5f, 1.0f);              
            //创建三角形对对象 
            water=new Water(MySurfaceView.this);        
            //打开深度检测
            GLES31.glEnable(GLES31.GL_DEPTH_TEST);
            //初始化纹理
            initTexture();
            //关闭背面剪裁   
            GLES31.glDisable(GLES31.GL_CULL_FACE);
        }
    }
	
	public void initTexture()//textureId
	{
		//生成纹理ID
		int[] textures = new int[1];
		GLES31.glGenTextures
		(
				1,          //产生的纹理id的数量
				textures,   //纹理id的数组
				0           //偏移量
		);    
		textureId=textures[0];    
		GLES31.glBindTexture(GLES31.GL_TEXTURE_2D, textureId);
		GLES31.glTexParameterf(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_MIN_FILTER,GLES31.GL_NEAREST);
		GLES31.glTexParameterf(GLES31.GL_TEXTURE_2D,GLES31.GL_TEXTURE_MAG_FILTER,GLES31.GL_LINEAR);
		GLES31.glTexParameterf(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_WRAP_S,GLES31.GL_CLAMP_TO_EDGE);
		GLES31.glTexParameterf(GLES31.GL_TEXTURE_2D, GLES31.GL_TEXTURE_WRAP_T,GLES31.GL_CLAMP_TO_EDGE);
        
     
        //通过输入流加载图片===============begin===================
        InputStream is = this.getResources().openRawResource(R.drawable.haimian);
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
        		GLES31.GL_TEXTURE_2D,   //纹理类型，在OpenGL ES中必须为GL10.GL_TEXTURE_2D
        		0, 					  //纹理的层次，0表示基本图像层，可以理解为直接贴图
        		bitmapTmp, 			  //纹理图像
        		0					  //纹理边框尺寸
        );
        bitmapTmp.recycle(); 		  //纹理加载成功后释放图片
	}
}