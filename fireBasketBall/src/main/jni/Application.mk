APP_STL := stlport_static
APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -std=c++11


NDK_TOOLCHAIN_VERSION=clang

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -std=c++11 -fsigned-char
APP_LDFLAGS := -latomic

NDK_TOOLCHAIN_VERSION=4.8
