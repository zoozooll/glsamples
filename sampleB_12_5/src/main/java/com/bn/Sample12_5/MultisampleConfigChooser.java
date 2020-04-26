package com.bn.Sample12_5;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGL11;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import android.opengl.GLSurfaceView;
import android.util.Log;


public class MultisampleConfigChooser implements GLSurfaceView.EGLConfigChooser 
{
    static private final String kTag = "GDC11";
    private int[] mValue;
    @Override
    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) 
    {
        mValue = new int[1];
        //configSpec数组指定了选择配置时需要参照的属性，通常以id,value依次存放
        int[] configSpec = {
        		EGL11.EGL_RED_SIZE, 5,//指定颜色缓冲区中的红色分量的位数
        		EGL11.EGL_GREEN_SIZE, 6,//指定颜色缓冲区中的绿色分量的位数
        		EGL11.EGL_BLUE_SIZE, 5,//指定颜色缓冲区中的蓝色分量的位数
        		EGL11.EGL_DEPTH_SIZE, 16,//指定深度缓冲中每个像素深度的位数
        		EGL11.EGL_RENDERABLE_TYPE, 4,//指定可以渲染成一幅画面的客户端API的类型
        		EGL11.EGL_SAMPLE_BUFFERS, 1,//指定多重采样缓冲区的个数
        		EGL11.EGL_SAMPLES, 2,//指定每像素的样本数量
        		EGL11.EGL_NONE         //属性列表以该常量为结束符
        };
        //获得系统所支持的配置信息的数量
        /*
         *  第二个参数attrib_list    事先定义的属性数组
         * 第三个参数configs  图形系统将返回若干满足条件的配置到该数组
         * 第四个参数config_size   configs数组的长度
         * 第四个参数num_config  图形系统返回的可用的配置个数
         */
        if (!egl.eglChooseConfig(display, configSpec, null, 0,mValue)) 
        {
            throw new IllegalArgumentException("eglChooseConfig fail");
        }
        int numConfigs = mValue[0];//记录配置信息的数量
        if (numConfigs <= 0) 
        {//没有找到合适的多重采样配置，创建一个覆盖多重采样配置
            final int EGL_COVERAGE_BUFFERS_NV = 0x30E0;
            final int EGL_COVERAGE_SAMPLES_NV = 0x30E1;
          //定义配置信息的属性列表
            configSpec = new int[]{
            		EGL11.EGL_RED_SIZE, 5,
            		EGL11.EGL_GREEN_SIZE, 6,
            		EGL11.EGL_BLUE_SIZE, 5,
            		EGL11.EGL_DEPTH_SIZE, 16,
            		EGL11.EGL_RENDERABLE_TYPE, 4,
                    EGL_COVERAGE_BUFFERS_NV, 1,
                    EGL_COVERAGE_SAMPLES_NV, 2,
                    EGL11.EGL_NONE//属性列表以该常量为结束符 
            };
            //获得系统所支持的配置信息的数量
            if (!egl.eglChooseConfig(display, configSpec, null, 0,mValue))
            {
                throw new IllegalArgumentException("2nd eglChooseConfig  fail");
            }
            numConfigs = mValue[0];

            if (numConfigs <= 0) 
            {
                configSpec = new int[]{
                		EGL11.EGL_RED_SIZE, 5,
                		EGL11.EGL_GREEN_SIZE, 6,
                		EGL11.EGL_BLUE_SIZE, 5,
                		EGL11.EGL_DEPTH_SIZE, 16,
                		EGL11.EGL_RENDERABLE_TYPE, 4 ,
                		EGL11.EGL_NONE//属性列表以该常量为结束符 
                };
                //获得系统所支持的配置信息的数量
                if (!egl.eglChooseConfig(display, configSpec, null, 0,mValue)) 
                {
                    throw new IllegalArgumentException("3rd eglChooseConfig fail");
                }
                numConfigs = mValue[0];//获取配置的数量
                if (numConfigs <= 0)
                {
                    throw new IllegalArgumentException("No configs match configSpec");
                }
            }
        }

        EGLConfig[] configs = new EGLConfig[numConfigs];//创建配置数组
        //获得配置信息EGLConfig数组
        if (!egl.eglChooseConfig(display, configSpec, configs, numConfigs,mValue)) 
        {
        	throw new IllegalArgumentException("data eglChooseConfig failed");
        }
        int index = -1;//声明索引值变量
        for (int i = 0; i < configs.length; ++i)
        {//查找多重采样的配置id
            if (findConfigAttrib(egl, display, configs[i], EGL10.EGL_RED_SIZE, 0) == 5) 
            {
                index = i;//记录查找到多重采样配置的索引值
                break;//退出循环
            }
        }
        if (index == -1) 
        {
            Log.w(kTag, "Did not find sane config, using first");
        }
        //获取多重采样的配置对象
        EGLConfig config = configs.length > 0 ? configs[index] : null;
        if (config == null)
        {
            throw new IllegalArgumentException("No config chosen");
        }
        return config;
    }

    private int findConfigAttrib(EGL10 egl, EGLDisplay display,EGLConfig config, int attribute, int defaultValue) {
    	//查询某个配置的某个属性id
        if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) 
        {
            return mValue[0];
        }
        return defaultValue;
    }

}