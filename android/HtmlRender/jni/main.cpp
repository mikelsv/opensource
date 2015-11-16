#define USEMSV_ANDROID
#define USEMSV_ANDROID_LOGPRINT "HtmlRender:"
//#define USEMSV_FILEPRINT
#define USEMSV_CXIMAGE_EMULATE

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>
#include <android/log.h>

//#include "com_msv_skylee_skylee.h"
#include "ru_centerix_HtmlRender_HtmlRender.h"

#include "../../../HtmlRender/htmlrender.cpp"


//JNIEXPORT jbyteArray JNICALL Java_ru_centerix_HtmlRender_HtmlRender_NativeHtmlRender
JNIEXPORT jbyteArray JNICALL Java_ru_centerix_HtmlRender_HtmlRender_NativeHtmlRender
  (JNIEnv *env, jobject, jstring js){

	// Input html file
	JVString jhtml(env, js);
	//VString html = jhtml;

	HtmlRender hren;
	TString data = hren.RenderToMem(LoadFile(jhtml));

	// Not good
	//TString data = LoadFile("/sdcard/tmp_hren.bmp");

	jbyteArray result = env->NewByteArray(data);

    if(result == NULL)
        return NULL; //  out of memory error thrown

    env->SetByteArrayRegion(result, 0, data.size(), (jbyte*)data.rchar());

    return result;
}

JNIEXPORT jint JNICALL Java_ru_centerix_HtmlRender_HtmlRender_NativeTest
  (JNIEnv *, jobject){
print("NativeTest");
return 1;
}


