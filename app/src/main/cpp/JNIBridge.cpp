#include <jni.h>

#include "NativeBridge.h"

extern "C" void JNICALL Java_com_example_ogretest_JavaBridge_InitNative ( JNIEnv *env, jobject /* this */ )
{
	NativeBridge::GetInstance().Init();
}