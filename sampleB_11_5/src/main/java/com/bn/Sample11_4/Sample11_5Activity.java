package com.bn.Sample11_4;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;

public class Sample11_5Activity extends Activity
{
	MyTDView mview;
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        //夾浬果沆央丁抄
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        mview=new MyTDView(this);
        mview.requestFocus();
        mview.setFocusableInTouchMode(true);
        setContentView(mview);
    }
    @Override
    public void onResume()
    {
    	super.onResume();
    	mview.onResume();
    }
    @Override
    public void onPause()
    {
    	super.onPause();
    	mview.onPause();
    }
}