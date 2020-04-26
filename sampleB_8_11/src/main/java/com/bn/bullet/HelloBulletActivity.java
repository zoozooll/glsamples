package com.bn.bullet;
import com.bn.bullet.GL2JNIView;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.Window;
import android.view.WindowManager;

public class HelloBulletActivity extends Activity 
{
//    @Override
//    public void onCreate(Bundle savedInstanceState) 
//    {    
//        super.onCreate(savedInstanceState);
//        setContentView(R.layout.main);        
//        System.out.println(450);
//        for(int i=0;i<450;i++)
//        {
//        	System.out.print("##$"+JNIPort.calJNI()+","+(((i+1)%6==0)?"\n":""));  
//        }        
//    }
    GL2JNIView mView;
	//屏幕对应的宽度和高度
	static float WIDTH;
	static float HEIGHT;
    @Override
    protected void onCreate(Bundle savedInstanceState) {    
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, 
        		WindowManager.LayoutParams.FLAG_FULLSCREEN);//去掉标头
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);//强制横屏
		//获得系统的宽度以及高度
        DisplayMetrics dm=new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(dm);
        if(dm.widthPixels>dm.heightPixels)
        {
        	WIDTH=dm.widthPixels;
        	HEIGHT=dm.heightPixels;
        }
        else
        {
        	WIDTH=dm.heightPixels;
        	HEIGHT=dm.widthPixels;
        }
        mView = new GL2JNIView(this);
		mView.requestFocus();					//获取焦点
		mView.setFocusableInTouchMode(true); 	//设置为可触控
        setContentView(mView);
        
        JNIPort.nativeSetAssetManager(this.getAssets());//初始化
    }
    
    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}