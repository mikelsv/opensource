MString android_datadir, android_id, android_uniq_id, android_market_id;

void InitAndroidId(VString path, VString id, VString uniq, VString market){
	android_datadir.Add(path, "/");
	android_id=id;
	android_uniq_id=uniq;
	android_market_id=market;

	//print("android_datadir: ", android_datadir);
	return ;
}

/*

//import MSVCoreAndroid
import android.telephony.TelephonyManager;
import android.provider.Settings;
import android.provider.Settings.Secure;
import android.os.Build;
import android.database.Cursor;
import android.content.Context;


	public void InitAndroidId(Context context){
		//Context context;

		TelephonyManager tManager = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);		
		String aid = Settings.Secure.getString(context.getContentResolver(), Settings.Secure.ANDROID_ID);
		String market_id="";

		String m_szDevIDShort = "35" + //we make this look like a valid IMEI
        		Build.BOARD.length()%10+ Build.BRAND.length()%10 +
        		Build.CPU_ABI.length()%10 + Build.DEVICE.length()%10 +
        		Build.DISPLAY.length()%10 + Build.HOST.length()%10 +
        		Build.ID.length()%10 + Build.MANUFACTURER.length()%10 +
        		Build.MODEL.length()%10 + Build.PRODUCT.length()%10 +
        		Build.TAGS.length()%10 + Build.TYPE.length()%10 +
        		Build.USER.length()%10 ; //13 digits
  
		//String[] arrayOfString = { "android_id" };
	    //    Cursor localCursor = getContentResolver().query(a, null, null, arrayOfString, null);
        //	if((localCursor.moveToFirst()) && (localCursor.getColumnCount() >= 2)){
        //		  market_id = Long.toHexString(Long.parseLong(localCursor.getString(1))).toUpperCase();		          
	    //    }

		NativeInitAndroidId(context.getFilesDir().toString(), aid, m_szDevIDShort, market_id);
		
		return;
	}


	public native void NativeInitAndroidId(String path, String id, String uid, String mid);


JNIEXPORT void JNICALL Java_com_msv_WPS_WPS_NativeInitAndroidId
(JNIEnv *env, jobject, jstring path, jstring id, jstring uniq, jstring market){
	InitAndroidId(JVSTRING(path), JVSTRING(id), JVSTRING( uniq), JVSTRING(market));
}
*/


#ifdef USEMSV_ANDROID
// JVString
class JVString{
	JNIEnv *env;
	jstring &js;
	char *line;

public:
	JVString(JNIEnv *e, jstring &s) : js(s){
		env=e;
		//js=s;
		line=0;
	}

	operator VString(){
		if(!line)
			line = (char*)env->GetStringUTFChars(js, 0);
		return VString(line);			
	}

	~JVString(){
		if(line)
			env->ReleaseStringUTFChars(js, line);
		line=0;
	}

};

#define JVSTRING(val) JVString(env, val)

#else
#define ANDROID_LOG_INFO 0
int __android_log_print(int, VString o, VString t){
	print(o, t);
	return 1;
}
#endif



#ifndef USEMSV_ANDROID
// Empty Android Class
#define jclass	int
#define jmethodID	int
#define jobjectArray int
#define jobject	int

class JNIEnv{
public:
	jclass FindClass(VString cls){
		return 0;
	}

	jobjectArray NewObjectArray(int size, jclass, int null){
		return 0;
	}

	jmethodID GetMethodID(jclass, VString method, VString vars){
		return 0;
	}

	void DeleteLocalRef(jclass){
		return ;
	}

	jobject NewObjectV(jclass, jmethodID, va_list &va){
		return 0;
	}

	int SetObjectArrayElement(jobjectArray, int, jobject){
		return 0;
	}

	jobject NewStringUTF(VString line){
		return 0;
	}

};


#define va_copy(a, b)


#endif



//#ifdef USEMSV_ANDROID
class JniReturnObject{
private:
public:
	JNIEnv *env;
	jobjectArray result;
	jclass menuItemClass;
	jmethodID cid;

public:
	JniReturnObject(JNIEnv *e, VString cls, VString method, VString vars, int size){
		env = e;

		menuItemClass = env->FindClass(cls);
		if(menuItemClass == NULL){
			return ; // exception thrown
		}

		result = env->NewObjectArray(size, menuItemClass, NULL);
		if(result == NULL){
			return ; // out of memory error thrown
		}

		cid = env->GetMethodID(menuItemClass, method, vars);
		if(cid == NULL){
			return ;
		}
	}

	~JniReturnObject(){
		if(menuItemClass)
			env->DeleteLocalRef(menuItemClass);
	}

	jmethodID& GetCid(){
		return cid;
	}

	jclass& GetItem(){
		return menuItemClass;
	}

	jobjectArray& GetResult(){
		return result;
	}

	void Insert(int pos, ...){
		if(!cid)
			return ;

		va_list vars, cvarc;
		va_start(vars, pos);
		va_copy(cvarc, vars);
	
		jobject item = env->NewObjectV(menuItemClass, cid, cvarc);
		//jobject item = env->NewObject(menuItemClass, cid, pos, env->NewStringUTF("login"), env->NewStringUTF("pass"));

		env->SetObjectArrayElement(result, pos, item);
		env->DeleteLocalRef(item);
	
		va_end(vars);
		va_end(cvarc);
	}

};
//#endif

class JString{
	JNIEnv *env;
	jobject jline;

public:

	JString(JNIEnv *e, VString line){
		env = e;
		jline = env->NewStringUTF(line);
	}

	operator jobject(){
		return jline;
	}

	~JString(){
		env->DeleteLocalRef(jline);
	}

};

#define TOJSTRING(val) (jobject)JString(env, val)