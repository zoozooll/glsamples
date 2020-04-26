#include <string.h>
#include <jni.h>
#include <android/log.h>

#include <stdio.h>
#include <math.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))




JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_step
  (JNIEnv *env, jclass jc)
{
	renderFrame();
}

JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_onSurfaceChanged
  (JNIEnv *env, jclass jc, jint width, jint height)
{
	onSurfaceChanged(width, height);
}


JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_onSurfaceCreated
  (JNIEnv *env, jclass jc, jobject obj)

{
	onSurfaceCreated(env,obj);
}

JNIEXPORT void JNICALL Java_com_bn_bullet_JNIPort_addBody
  (JNIEnv *env, jclass jc, jint id)
{

	setAddBodyId(id);
}






