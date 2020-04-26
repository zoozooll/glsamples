package com.bn.Sample10_1;//声明包
import java.util.ArrayList;

import android.opengl.GLES30;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


import android.content.Context;

class MySurfaceView extends GLSurfaceView 
{
	private final float TOUCH_SCALE_FACTOR = 180.0f/320;//角度缩放比例
    private SceneRenderer mRenderer;//场景渲染器    
    
    private float mPreviousY;//上次的触控位置Y坐标
    private float mPreviousX;//上次的触控位置X坐标
	//关于摄像机的变量
	float cx=0;//摄像机x位置
	float cy=0;//摄像机y位置
	float cz=60;//摄像机z位置
	
	float tx=0;//目标点x位置
	float ty=0;//目标点y位置
	float tz=0;//目标点z位置
	public float currSightDis=100;//摄像机和目标的距离
	float angdegElevation=30;//仰角
	public float angdegAzimuth=180;//方位角	
	float left;
    float right;
	float top;
	float bottom;
	float near;
	float far;
	
	//可拾取物体列表
	ArrayList<TouchableObject> lovnList=new ArrayList<TouchableObject>();
	//被选中物体的索引值，即id，没有被选中时索引值为-1
	int checkedIndex=-1;
	public MySurfaceView(Context context) {
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
        float y = e.getY();//获取触控点的x坐标
        float x = e.getX();//获取触控点的y坐标
        switch (e.getAction()) {
        case MotionEvent.ACTION_DOWN://若是按下动作
        	//计算仿射变换后A、B两点在世界坐标系中的坐标
			float[] AB=IntersectantUtil.calculateABPosition
			(
				x, y, //触控点x、y坐标
				Sample10_1_Activity.screenWidth, //屏幕宽度
				Sample10_1_Activity.screenHeight, //屏幕高度
				//近平面参数
				left, //视角left、top值
				top,
				near, //视角near、far值
				far
			);
			//射线AB
			Vector3f start = new Vector3f(AB[0], AB[1], AB[2]);//拾取射线线段起点A坐标
			Vector3f end = new Vector3f(AB[3], AB[4], AB[5]);//拾取射线线段起点B坐标
			
			
			/*
			 * 计算AB线段与每个物体包围盒的最佳交点(与A点最近的交点)，
			 * 并记录有最佳交点的物体在列表中的索引值
			 */
			//记录列表中时间最小的索引值
    		checkedIndex = -1;//设置选中物体索引(-1表示没有选中物体)
    		int tmpIndex=-1;//用于记录距离A点最近物体的索引值
    		float minTime=1;//用于记录列表中物体与拾取射线线段相交的最短时间
    		for(int i=0;i<lovnList.size();i++){//遍历列表中的物体
    			OBB box = lovnList.get(i).getCurrBox(); //获得物体OBB包围盒   
    			//将射线start->end的起点与终点变换到物体坐标系   	   
    			Vector3f rayStart=MatrixState.fromGToO(start,lovnList.get(i).m);
    			Vector3f rayEnd=MatrixState.fromGToO(end,lovnList.get(i).m);
    			//计算出射线参数方程中的rayDir，即end-start
    			Vector3f rayDir=rayEnd.minus(rayStart);//拾取射线线段的长度和方向
    			
				float t = box.rayIntersect(rayStart, rayEnd,rayDir);//计算相交时间
    			if (t <= minTime) {//若时间小于原最短时间
					minTime = t;//更新最短时间
					tmpIndex = i;//更新最近物体索引
				}
    		}
    		checkedIndex=tmpIndex;//记录最近(选中)物体索引 		
    		changeObj(checkedIndex);//改变被选中物体	
       	break;
        case MotionEvent.ACTION_MOVE://若是移动动作
            float dy = y - mPreviousY;//计算触控笔Y位移
            float dx = x - mPreviousX;//计算触控笔X位移
            //不超过阈值不移动摄像机
            if(Math.abs(dx)<7f && Math.abs(dy)<7f){
            	break;
            }            
            angdegAzimuth += dx * TOUCH_SCALE_FACTOR;//设置沿x轴旋转角度
            angdegElevation += dy * TOUCH_SCALE_FACTOR;//设置沿z轴旋转角度
            //将仰角限制在5～90度范围内
            angdegElevation = Math.max(angdegElevation, 5);
            angdegElevation = Math.min(angdegElevation, 90);
            //设置摄像机的位置
            setCameraPostion();
        break;
        }
        //记录触控点x、y坐标
        mPreviousY = y;//记录触控笔位置
        mPreviousX = x;//记录触控笔位置
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
	//根据索引改变选中物体
	public void changeObj(int index){
		if(index != -1){//如果有物体被选中
    		for(int i=0;i<lovnList.size();i++){//遍历所有可拾取物体列表
    			if(i==index){//若为选中物体
    				lovnList.get(i).changeOnTouch(true);//改变物体颜色及放大率到选中状态
    			}
    			else{//若不是选中物体
    				lovnList.get(i).changeOnTouch(false);//改变物体颜色及放大率到未选中状态
    			}
    		}
        }
    	else{//如果没有物体被选中
    		for(int i=0;i<lovnList.size();i++){//遍历所有可拾取物体列表			
    			lovnList.get(i).changeOnTouch(false);//改变物体颜色及放大率到未选中状态
    		}
    	}
	}
	private class SceneRenderer implements GLSurfaceView.Renderer 
    {
    	//从指定的obj文件中加载对象
		LoadedObjectVertexNormalFace pm;//从obj文件中加载的平面
		LoadedObjectVertexNormalFace cft;
		LoadedObjectVertexNormalAverage qt;
		LoadedObjectVertexNormalAverage yh;
		LoadedObjectVertexNormalAverage ch;
    	
        public void onDrawFrame(GL10 gl) 
        { 
        	//清除深度缓冲与颜色缓冲
            GLES30.glClear( GLES30.GL_DEPTH_BUFFER_BIT | GLES30.GL_COLOR_BUFFER_BIT);
            //设置摄像机位置
			MatrixState.setCamera(cx, cy, cz, tx, ty, tz, 0, 1, 0);
			//设置光源位置
            MatrixState.setLightLocation(100, 100, 100);                    
            //绘制物体            
            pm.drawSelf();//绘制平面
        	
            //绘制长方体
            MatrixState.pushMatrix();//保护现场
            MatrixState.translate(-30f, 0, 0);//平移变换
            MatrixState.scale(cft.size, cft.size, cft.size);//缩放变换
            cft.drawSelf();//绘制长方体
            MatrixState.popMatrix();//恢复现场   
            //绘制球体
            MatrixState.pushMatrix();
            MatrixState.translate(30f, 0f, 0);
            MatrixState.scale(qt.size, qt.size, qt.size);
            qt.drawSelf();
            MatrixState.popMatrix();  
            //绘制圆环
            MatrixState.pushMatrix();
            MatrixState.translate(0, 0, -30f);
            MatrixState.scale(yh.size, yh.size, yh.size);
            MatrixState.rotate(45, 0, 1, 0);
            yh.drawSelf();
            MatrixState.popMatrix();  
            //绘制茶壶
            MatrixState.pushMatrix();
            MatrixState.translate(0, 0, 30f);
            MatrixState.scale(ch.size, ch.size, ch.size);
            MatrixState.rotate(30, 0, 1, 0);
            ch.drawSelf();
            MatrixState.popMatrix(); 
        } 

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            //设置视窗大小及位置 
        	GLES30.glViewport(0, 0, width, height); 
        	//计算GLSurfaceView的宽高比
            float ratio = (float) width / height;
            //调用此方法计算产生透视投影矩阵
            left=right=ratio;
            top=bottom=1;
            near=2;
            far=500;
            MatrixState.setProjectFrustum(-left, right, -bottom, top, near, far);
            //计算摄像机的位置
            setCameraPostion();
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
            
            //加载平面
            pm=LoadUtil.loadFromFileVertexOnlyFace("pm.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
                        
            ch=LoadUtil.loadFromFileVertexOnlyAverage("ch.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
    		cft=LoadUtil.loadFromFileVertexOnlyFace("cft.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
    		qt=LoadUtil.loadFromFileVertexOnlyAverage("qt.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
    		yh=LoadUtil.loadFromFileVertexOnlyAverage("yh.obj", MySurfaceView.this.getResources(),MySurfaceView.this);
    		//将可拾取物体加入列表
    		lovnList.add(ch);
            lovnList.add(cft);
            lovnList.add(qt);
            lovnList.add(yh);
        }
    }
}