LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := BNbullet

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/ \
		$(LOCAL_PATH)/Bullet \
		$(LOCAL_PATH)/Bullet/BulletCollision \
		$(LOCAL_PATH)/Bullet/BulletCollision/BroadphaseCollision \
		$(LOCAL_PATH)/Bullet/BulletCollision/NarrowPhaseCollision \
		$(LOCAL_PATH)/Bullet/BulletCollision/CollisionDispatch \
		$(LOCAL_PATH)/Bullet/BulletCollision/CollisionShapes \
		$(LOCAL_PATH)/Bullet/BulletCollision/Gimpact \
		$(LOCAL_PATH)/Bullet/BulletSoftBody \
		$(LOCAL_PATH)/Bullet/BulletDynamics \
		$(LOCAL_PATH)/Bullet/BulletDynamics/Vehicle \
		$(LOCAL_PATH)/Bullet/BulletDynamics/ConstraintSolver \
		$(LOCAL_PATH)/Bullet/BulletDynamics/Dynamics \
		$(LOCAL_PATH)/Bullet/BulletDynamics/Character \
		$(LOCAL_PATH)/Bullet/BulletDynamics/Featherstone \
		$(LOCAL_PATH)/Bullet/BulletDynamics/MLCPSolvers \
		$(LOCAL_PATH)/Bullet/LinearMath \
		$(LOCAL_PATH)/Bullet/vectormath

libLinearMath_la_SOURCES := \
		Bullet/LinearMath/BNGen_LinearMath.cpp

libBulletCollision_la_SOURCES := \
        Bullet/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
        Bullet/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
        Bullet/BulletCollision/NarrowPhaseCollision/BNGen_NarrowPhaseCollision.cpp \
        Bullet/BulletCollision/CollisionDispatch/BNGen_CollisionDispatch.cpp \
		Bullet/BulletCollision/CollisionShapes/BNGen_CollisionShapes.cpp \
		Bullet/BulletCollision/BroadphaseCollision/BNGen_BroadphaseCollision.cpp \
		Bullet/BulletCollision/Gimpact/btContactProcessing.cpp \
		Bullet/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
		Bullet/BulletCollision/Gimpact/btGImpactBvh.cpp \
		Bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
		Bullet/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
		Bullet/BulletCollision/Gimpact/btGImpactShape.cpp \
		Bullet/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
		Bullet/BulletCollision/Gimpact/gim_box_set.cpp \
		Bullet/BulletCollision/Gimpact/gim_contact.cpp \
		Bullet/BulletCollision/Gimpact/gim_memory.cpp \
		Bullet/BulletCollision/Gimpact/gim_tri_collision.cpp

libBulletDynamics_la_SOURCES := \
		Bullet/BulletDynamics/Dynamics/BNGen_Dynamics.cpp \
		Bullet/BulletDynamics/ConstraintSolver/BNGen_ConstraintSolver.cpp \
		Bullet/BulletDynamics/Vehicle/BNGen_Vehicle.cpp \
		Bullet/BulletDynamics/Character/BNGen_Character.cpp \
		Bullet/BulletDynamics/Featherstone/BNGen_Featherstone.cpp \
		Bullet/BulletDynamics/MLCPSolvers/BNGen_MLCPSolvers.cpp

libBulletSoftBody_la_SOURCES := \
		Bullet/BulletSoftBody/btSoftBodyHelpers.cpp \
		Bullet/BulletSoftBody/btSoftBody.cpp \
		Bullet/BulletSoftBody/BNGen_BulletSoftBody.cpp		

LOCAL_SRC_FILES := \
		$(libLinearMath_la_SOURCES) \
		$(libBulletCollision_la_SOURCES) \
		$(libBulletDynamics_la_SOURCES) \
		$(libBulletSoftBody_la_SOURCES) \
        myEncapsulation/MatrixState.cpp \
        myEncapsulation/ShaderUtil.cpp \
        myEncapsulation/FileUtil.cpp \
        myEncapsulation/TexPlane.cpp \
        myEncapsulation/TexCuboid.cpp \
        myEncapsulation/TexBall.cpp \
        myEncapsulation/TexCapsule.cpp \
        myEncapsulation/TexCone.cpp \
        myEncapsulation/TexCylinderShape.cpp \
        myEncapsulation/geometry/TextureRect.cpp \
        myEncapsulation/geometry/TexturePlane.cpp \
        myEncapsulation/geometry/CylinderSide.cpp \
        myEncapsulation/geometry/ConeSide.cpp \
        myEncapsulation/geometry/Circle.cpp \
        myEncapsulation/geometry/Ball.cpp \
        myEncapsulation/geometry/AnyShapeByVectices.cpp \
        hellocpp/Vehicle.cpp \
        hellocpp/Landform.cpp \
        hellocpp/main.cpp \
        hellocpp/jni_code.c

LOCAL_ARM_MODE   := arm 
LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -O3 -DANDROID_NDK -DDISABLE_IMPORTGL 
#LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDLIBS := -ldl -llog -lGLESv3 -landroid

include $(BUILD_SHARED_LIBRARY)
