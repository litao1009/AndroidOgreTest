#include <android_native_app_glue.h>

#include "AndroidBridge.h"

void android_main(struct android_app* state)
{
	app_dummy();

	AndroidBridge::GetInstance().Init(state);

	AndroidBridge::GetInstance().Run();
}