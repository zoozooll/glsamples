package com.bn.Sample10_2;//声明包
import java.util.HashMap;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.content.Context;

class MySurfaceView extends GLSurfaceView 
{
    private SceneRenderer mRenderer;//场景渲染器   
	//关于摄像机的变量
	float cx=0;//摄像机x位置
	float cy=0;//摄像机y位置
	float cz=60;//摄像机z位置
	
	float tx=0;//目标点x位置
	float ty=0;//目标点y位置
	float tz=0;//目标点z位置
	public float currSightDis=60;//摄像机和目标的距离
	float angdegElevation=30;//仰角
	public float angdegAzimuth=180;//方位角	
	//关于多点触控的量
	HashMap<Integer,BNPoint> hm=new HashMap<Integer,BNPoint>();
	float distance=0;//主辅点距离
	float currScale=2;//初始缩放比例
	float scaleSpeedSpan=100;//缩放步进比例
	float angle=0;//初始旋转角度
	public MySurfaceView(Context context){
        super(context);
        this.setEGLContextClientVersion(3); //设置使用OPENGL ES3.0
        mRenderer = new SceneRenderer();	//创建场景渲染器
        setRenderer(mRenderer);				//设置渲染器		        
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);//设置渲染模式为主动渲染   
    }
	//触控事件处理方法
    @Override 
    public boolean onTouchEvent(MotionEvent e) 
    {
    	//获取触控动作编号
		int action=e.getAction()&MotionEvent.ACTION_MASK;
		//获取主、辅点id（down时主辅点id皆正确，up时辅点id正确，主点id要查询Map中剩下的一个点的id）
		int index=(e.getAction()&MotionEvent.ACTION_POINTER_INDEX_MASK)
				>>>MotionEvent.ACTION_POINTER_INDEX_SHIFT;	//获取触控点索引
		int id=e.getPointerId(index); //获取触控点id
		switch(action)
		{
		case MotionEvent.ACTION_DOWN: //主点按下
		case MotionEvent.ACTION_POINTER_DOWN: //辅点按下
			//不论是主点还是辅点按下皆向Map中放入一个新点
			hm.put(id, new BNPoint(e.getX(index), e.getY(index)));
			//若已经有两个触控点按下，则计算距离
			if (hm.size() == 2){//若已经有两个触控点按下
				BNPoint bpTempA = hm.get(e.getPointerId(0)); //从Map中取出第一个触控点
				BNPoint bpTempB = hm.get(e.getPointerId(1));	//从Map中取出第二个触控点
				distance = BNPoint.calDistance(bpTempA, bpTempB);//计算两触控点之间的距离
			}
		break;
		case MotionEvent.ACTION_MOVE: //主/辅点移动
			//不论主/辅点Move都更新其位置
			int count=e.getPointerCount();//获取触控点数量
			for(int i=0;i<count;i++)//遍历所有触控点
			{
				int tempId=e.getPointerId(i);//获取触控点id
				hm.get(tempId).setLocation(e.getX(i), e.getY(i));//更新触控点位置
			}
			
			//若当前有两个触控点按下则计算触控点距离并换算为缩放系数
			//同时计算旋转角度
			if (hm.size() == 2) {//若当前有两个触控点按下
				BNPoint bpTempA = hm.get(e.getPointerId(0)); //从Map中取出第一个触控点
				BNPoint bpTempB = hm.get(e.getPointerId(1)); //从Map中取出第二个触控点
				//计算触控点距离并换算为缩放系数
				float currDis = BNPoint.calDistance(bpTempA, bpTempB);//计算两点距离
				currScale = currScale + (currDis - distance) / scaleSpeedSpan;//计算缩放系数
				if (currScale > 4 || currScale < 1) {//若缩放系数超过范围进行恢复
					currScale = currScale - (currDis - distance) / scaleSpeedSpan;
				}
				distance = currDis;//记录两点距离
				//计算旋转角度
				if (bpTempA.hasOld || bpTempB.hasOld) {//若两个触控点都有上一位置
					double alphaOld = Math.atan2((bpTempA.oldY - bpTempB.oldY),
							(bpTempA.oldX - bpTempB.oldX));//计算上一位置对应的角度
					double alphaNew = Math.atan2((bpTempA.y - bpTempB.y),
							(bpTempA.x - bpTempB.x));//计算当前位置对应的角度
					//两个角度相减得到旋转变化角度，并将旋转变化角度叠加到总旋转角度中
					angle = angle - (float) Math.toDegrees(alphaNew - alphaOld);
				}
			}
		break;
		case MotionEvent.ACTION_UP: //主点抬起
			//在本应用中主点UP则只需要清空Map即可，在其他一些应用中需要操作的
			//则取出Map中唯一剩下的点操作即可
			hm.clear();//清空触控点Map
		break;
		case MotionEvent.ACTION_POINTER_UP: //辅点抬起
			//从Map中删除对应id的辅点
			hm.remove(id);//从Map中删除对应id的辅点
		break;
		} 			
		return true;
	}
    
    //设置摄像机位置的方法
	public void setCameraPostion() {
		//计算摄像机的位置
		double angradElevation = Math.toRadians(angdegElevation);//仰角（弧度）
		double angradAzimuth = Math.toRadians(angdegAzimuth);//方位角
		cx = (float) (tx - currSightDis * Math.cos(angradElevation)	* Math.sin(angradAzimuth));
		cy = (float) (ty + currSightDis * Math.sin(angradElevation));
		cz = (float) (tz - currSightDis * Math.cos(angradElevation) * Math.cos(angradAzimuth));
	}
	private class SceneRenderer implements GLSurfaceView.Renderer 
    {
		LoadedObjectVertexNormalAverage ch;
    	
        public void onDrawFrame(GL10 gl) 
        { 
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
            //绘制茶壶
            MatrixState.pushMatrix();
            MatrixState.translate(0, -10, 0);
            MatrixState.scale(currScale, currScale, currScale);
            MatrixState.rotate(angle, 0, 0, 1);
            ch.drawSelf();
            MatrixState.popMatrix(); 
        } 

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            MatrixState.setProjectFrustum(-ratio, ratio, -1, 1, 2, 100);
            //计算摄像机的位置
            setCameraPostion();       

			//设置camera位置
			MatrixState.setCamera(cx, cy, cz, tx, ty, tz, 0, 1, 0);            
            //初始化光源位置
            MatrixState.setLightLocation(100, 100, 100);       
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            //设置屏幕背景色RGBA
            GLES30.glClearColor(0.3f,0.3f,0.3f,1.0f);    
            //打开深度检测
            GLES30.glEnable(GLES30.GL_DEPTH_TEST);  
            //关闭背面剪裁
            GLES30.glDisable(GLES30.GL_CULL_FACE);
            //初始化变换矩阵
            MatrixState.setInitStack();            
            //加载要绘制的物体
            ch=LoadUtil.loadFromFileVertexOnlyAverage("ch.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
        }
    }
}