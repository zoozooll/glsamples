LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := BNJNI

LOCAL_SRC_FILES := \
        myEncapsulation/MatrixState.cpp \
        myEncapsulation/ShaderUtil.cpp \
        myEncapsulation/FileUtil.cpp \
        element/Vector3f.cpp \
        element/Vector4f.cpp \
        element/Mat4.cpp \
        draw/MS3DHeader.cpp \
        draw/MS3DVertex.cpp \
        draw/MS3DTriangle.cpp \
        draw/MS3DGroup.cpp \
        draw/MS3DMaterial.cpp \
        draw/MS3DKeyFramePosition.cpp \
        draw/MS3DKeyFrameRotate.cpp \
        draw/MS3DJoint.cpp \
        draw/MS3DModel.cpp \
        hellocpp/main.cpp \
        hellocpp/jni_code.c

LOCAL_ARM_MODE   := arm 
#LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -O3 -DANDROID_NDK -DDISABLE_IMPORTGL 
#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -ldl -llog -lGLESv3 -landroid

include $(BUILD_SHARED_LIBRARY)
