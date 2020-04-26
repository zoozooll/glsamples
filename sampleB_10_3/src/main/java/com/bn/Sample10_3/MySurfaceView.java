package com.bn.Sample10_3;

import java.util.HashMap;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;

public class MySurfaceView extends GLSurfaceView 
{
    private SceneRenderer mRenderer;//场景渲染器
    //茶壶的偏移量
    static float xOffset=0;
    //茶壶转动的角度
    static float yAngle=0;
	HashMap<Integer,BNPoint> hm=new HashMap<Integer,BNPoint>();
	//模拟键盘监听线程
	KeyThread kt;
	
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{//触控事件处理方法
		if(kt==null)return false;	//若键位处理线程对象不存在则返回
		//获取触控的动作编号
		int action=event.getAction()&MotionEvent.ACTION_MASK;//获取触控的动作编号
		//获取主、辅点id（down时主辅点id皆正确，up时辅点id正确，主点id要查询Map中剩下的一个点的id）
		int index=(event.getAction()&MotionEvent.ACTION_POINTER_INDEX_MASK)
				>>>MotionEvent.ACTION_POINTER_INDEX_SHIFT; //获取触控点索引
		int id=event.getPointerId(index);//获取触控点ID
		switch(action)
		{
			case MotionEvent.ACTION_DOWN: //主点按下
			case MotionEvent.ACTION_POINTER_DOWN: //辅点按下	
				//不论是主点还是辅点按下皆向Map中放入一个新点
				hm.put(id, new BNPoint(event.getX(index),event.getY(index)));
				kt.keyPress(event.getX(index), event.getY(index));//更改对应比特位的值
			break;
			case MotionEvent.ACTION_POINTER_UP://辅点抬起	
				float x=hm.get(id).x;//获取抬起触控点的x坐标
				float y=hm.get(id).y;//获取抬起触控点的y坐标
				hm.remove(id);//移除抬起的触控点
				kt.keyUp(x, y);//更改对应比特位的值
			break;
			case MotionEvent.ACTION_UP://主点抬起
				hm.clear();//清除所有触控点
				kt.clearKeyState();//置所有比特位为0
			break;
		}
		return true;
	}
	
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
    	//从指定的obj文件中加载对象
		LoadedObjectVertexNormal ch;
		LoadedObjectVertexNormal pm;
    	
        public void onDrawFrame(GL10 gl) 
        {
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear(GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
            MatrixState.copyMVMatrix();
            MatrixState.pushMatrix();//保护现场
            //若加载的物体不为空则绘制物体
            if(pm!=null)
            {
            	pm.drawSelf();
            }
            MatrixState.popMatrix();
            MatrixState.pushMatrix();
            MatrixState.translate(xOffset, 0, 0);
            MatrixState.rotate(yAngle, 0, 1, 0);
            if(ch!=null)
            {
            	ch.drawSelf();
            }
            MatrixState.popMatrix();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 2, 100);
            MatrixState.setCamera(0,5,25,0,0,0,0,1,0);
            //初始化光源位置
        	MatrixState.setLightLocation(70, 30, 70);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //设置屏幕背景色RGBA
        	GLES30.glClearColor(1.0f,1.0f,1.0f,1.0f);
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);
            //打开背面剪裁   
            GLES30.glEnable(GLES30.GL_CULL_FACE);
            //初始化变换矩阵
            MatrixState.setInitStack();
            //加载要绘制的物体
            ch=LoadUtil.loadFromFile("ch.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
            pm=LoadUtil.loadFromFile("pm.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
            kt=new KeyThread();
            kt.start();
        }
    }
}