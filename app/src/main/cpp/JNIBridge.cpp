#include <jni.h>

#include "NativeBridge.h"
#include "Event/SysEvent.h"

#include <android/asset_manager_jni.h>
#include <android/native_window_jni.h>

extern "C" void JNICALL Java_com_example_ogretest_JavaBridge_InitNative( JNIEnv *env, jobject /* this */,
																		 jobject jAssetMgr)
{
	auto assetManager = AAssetManager_fromJava(env, jAssetMgr);
	NativeBridge::GetInstance().Init(assetManager);
}

extern "C" void JNICALL Java_com_example_ogretest_JavaBridge_OnSurfaceCreated( JNIEnv *env, jobject /* this */,
																		 jobject surface)
{
	auto sysEvt = SysEVT_SufaceCreated::Create();
	sysEvt->NativeWindow = ANativeWindow_fromSurface(env, surface);

	NativeBridge::GetInstance().PostEvent(sysEvt);
}