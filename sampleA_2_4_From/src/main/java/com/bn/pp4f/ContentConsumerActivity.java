package com.bn.pp4f;//包声明
import android.app.Activity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
public class ContentConsumerActivity extends Activity {
    ContentResolver cr;// ContentResolver的引用
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);//继承父类的onCreate方法
        setContentView(R.layout.main);//跳转到主界面
        cr=this.getContentResolver();//获取ContentResolver的对象
        //初始化查询按钮
        Button b=(Button)this.findViewById(R.id.Button01);// Button类的引用
        b.setOnClickListener(//设置按钮监听
          new OnClickListener(){
			@Override
			public void onClick(View v) {//重写onClick方法
				String stuname="Android";				
				Cursor cur=cr.query(//设置查询的字符串
				   Uri.parse("content://com.bn.pp4.provider.student/stu"), 
				   new String[]{"sno","stuname","sage","sclass"}, 
				   "stuname=?", //查询条件
				   new String[]{stuname}, 
				   "sage ASC"
				);	
	        	while(cur.moveToNext()){
	        		String sno=cur.getString(0);//获取学号
	        		String sname=cur.getString(1);//获取名称
	        		int sage=cur.getInt(2);//获取年龄
	        		String sclass=cur.getString(3);//获取班级
	        		appendMessage(sno+"\t"+sname+"\t\t"+sage+"\t"+sclass);
	        	}
	        	cur.close();//关闭ContentResolver
	}});}
    public void appendMessage(String msg){    //向文本区中添加文本
    	EditText et=(EditText)this.findViewById(R.id.EditText02);//获取EditText的对象
    	et.append(msg+"\n");//添加显示的字符串
}}